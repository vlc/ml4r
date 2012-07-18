#include "MachineLearning/DecisionTree/NodeSplitter.h"
#include "MachineLearning/DecisionTree/DecisionTreeExperiment.h"
#include "MachineLearning/DecisionTree/SplitDefinition.h"
#include "MachineLearning/DecisionTree/DecisionTreeNode.h"
#include "MachineLearning/DecisionTree/CategoryInfo.h"
#include "MachineLearning/MLData/MLData.h"
#include "MachineLearning/GBM/GBMEstimator.h"
#include "utils/Utils.h"

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <cmath>
using boost::lexical_cast;

NodeSplitter::NodeSplitter(MLData* data, int minObservations, double scale) 
    : m_data(data), m_minObservations(minObservations),m_scale(scale)
{
    m_missingValueDefined = m_data->missingValueDefined();
    if (m_missingValueDefined)
        m_missingValue = m_data->getMissingValue();
}

NodeSplitter::~NodeSplitter() {}

double NodeSplitter::calculateImprovement(double lhsSumW, double lhsSumZ, double rhsSumW, double rhsSumZ, double missingSumW, double missingSumZ)
{
    double improvement = 0.0;

    if (missingSumW == 0)
    {
        double meanZDifference = lhsSumZ / lhsSumW - rhsSumZ / rhsSumW;
        improvement = lhsSumW * rhsSumW * pow(meanZDifference, 2) / (lhsSumW + rhsSumW);
    }
    else
    {
        double meanLRDifference = lhsSumZ / lhsSumW - rhsSumZ / rhsSumW;
        double meanLMDifference = lhsSumZ / lhsSumW - missingSumZ / missingSumW;
        double meanRMDifference = rhsSumZ / rhsSumW - missingSumZ / missingSumW;

        improvement += lhsSumW * rhsSumW     * pow(meanLRDifference, 2);
        improvement += lhsSumW * missingSumW * pow(meanLMDifference, 2);
        improvement += rhsSumW * missingSumW * pow(meanRMDifference, 2);
        improvement /= (lhsSumW + rhsSumW + missingSumW);
    }

    return improvement;
}

// double NodeSplitter::calculateImprovement(double lhsSumW, double lhsSumZ, double rhsSumW, double rhsSumZ, double missingSumW, double missingSumZ)
// {
//     double improvement = 0.0;
// 
//     if (lhsSumW)
//         improvement += pow(lhsSumZ, 2) / lhsSumW;
// 
//     if (rhsSumW)
//         improvement += pow(rhsSumZ, 2) / rhsSumW;
// 
//     if (missingSumW)
//         improvement += pow(missingSumZ, 2) / missingSumW;
// 
//     return improvement;
// }

shared_ptr<DecisionTreeNode> NodeSplitter::createLhsChild( shared_ptr<SplitDefinition> splitDefinition )
{
    return createChild(splitDefinition, LHS);
}

shared_ptr<DecisionTreeNode> NodeSplitter::createRhsChild( shared_ptr<SplitDefinition> splitDefinition )
{
    return createChild(splitDefinition, RHS);
}

shared_ptr<DecisionTreeNode> NodeSplitter::createMissingChild( shared_ptr<SplitDefinition> splitDefinition )
{
    return createChild(splitDefinition, MISSING);
}

shared_ptr<DecisionTreeNode> NodeSplitter::createChild( shared_ptr<SplitDefinition> splitDefinition, Partition partition )
{
    vector<shared_ptr<DecisionTreeExperiment> > experiments = splitDefinition->getNodeToSplit()->getExperiments();
    vector<shared_ptr<DecisionTreeExperiment> > childExperiments = 
        partitionExperiments(experiments, splitDefinition, partition);

    double sumZ;
    double sumW;
    if (partition == LHS)
    {
        sumZ = splitDefinition->getLhsSumZ();
        sumW = splitDefinition->getLhsSumW();
    }
    else if (partition == RHS)
    {
        sumZ = splitDefinition->getRhsSumZ();
        sumW = splitDefinition->getRhsSumW();
    }
    else
    {
        sumZ = splitDefinition->getMissingSumZ();
        sumW = splitDefinition->getMissingSumW();
    }
    shared_ptr<DecisionTreeNode> child = 
        shared_ptr<DecisionTreeNode>(new DecisionTreeNode(childExperiments, sumZ, sumW, partition, splitDefinition));
    return child;
}

vector<shared_ptr<DecisionTreeExperiment> > NodeSplitter::partitionExperiments(vector<shared_ptr<DecisionTreeExperiment> >& experiments, 
    shared_ptr<SplitDefinition> splitDefinition, Partition partition)
{
    bool rhs = !partition;
    vector<shared_ptr<DecisionTreeExperiment> > partitionExperiments;

    if (partition == LHS)
        partitionExperiments.reserve(splitDefinition->getLhsExperimentCount());
    else if (partition == RHS)
        partitionExperiments.reserve(splitDefinition->getRhsExperimentCount());
    else if (partition == MISSING)
        partitionExperiments.reserve(splitDefinition->getMissingExperimentCount());

    int featureIndex  = splitDefinition->getFeatureIndex();

    if (splitDefinition->isCategorical())
    {
        // categorical
        set<double>& lhsCategories = splitDefinition->getLhsCategories();
        set<double>& rhsCategories = splitDefinition->getRhsCategories();

        BOOST_FOREACH(shared_ptr<DecisionTreeExperiment>& experiment, experiments)
        {
            double featureValue = experiment->getFeatureValue(featureIndex);
            set<double>::const_iterator lhsIt = lhsCategories.find(featureValue);
            set<double>::const_iterator rhsIt = rhsCategories.find(featureValue);

            if ((partition == MISSING && m_missingValueDefined && m_missingValue == featureValue) ||
                (partition == LHS && lhsIt != lhsCategories.end()) ||
                (partition == RHS && rhsIt != rhsCategories.end()))
            {
                partitionExperiments.push_back(experiment);
            }
        }
    }
    else
    {
        // continuous
        double      splitValue    = splitDefinition->getSplitValue();
        BOOST_FOREACH(shared_ptr<DecisionTreeExperiment>& experiment, experiments)
        {
            double featureValue = experiment->getFeatureValue(featureIndex);

            if (m_missingValueDefined && featureValue == m_missingValue)
            {
                // experiment has a missing value
                if (partition == MISSING)
                    partitionExperiments.push_back(experiment);
            }
            else if ((partition == LHS && featureValue < splitValue) ||
                (partition == RHS && featureValue > splitValue))
                partitionExperiments.push_back(experiment);
        }
    }
    return partitionExperiments;    
}

vector<shared_ptr<DecisionTreeNode> > NodeSplitter::splitNode( shared_ptr<DecisionTreeNode> nodeToSplit, vector<int> featuresToConsider )
{
    vector<shared_ptr<DecisionTreeNode> > children;

    if (nodeToSplit->getSumW() == 0)
        return children;

    // find terminal node with best improvement for any of those variables
    shared_ptr<SplitDefinition> bestSplit;
    double                      bestImprovement = 0.0;

    vector<double>                      vecImprovements;
    vector<shared_ptr<SplitDefinition> > vecSplits;

    vecImprovements.reserve(featuresToConsider.size());
    vecSplits.reserve(featuresToConsider.size());

    set<int>& categoricalFeatures = m_data->getCategoricalFeatureIndices();

    BOOST_FOREACH(int featureIndex, featuresToConsider)
    {
        
        shared_ptr<SplitDefinition> split;

        if (Utils::hasElement(categoricalFeatures,featureIndex))
            split = createCategoricalSplitDefinition(nodeToSplit, featureIndex);
        else
            split = createContinuousSplitDefinition(nodeToSplit, featureIndex);

        vecSplits.push_back(split);
        vecImprovements.push_back(split.get() ? split->getImprovement() : 0);

        if (!split.get()) // it returned an invalid
            continue;

        if (split->getImprovement() > bestImprovement)
        {
            bestImprovement = split->getImprovement();
            bestSplit       = split;
        }
    }
    if (bestImprovement == 0.0)
        return children;

    if (m_scale != std::numeric_limits<double>::infinity() && vecImprovements.size() > 1)
    {
        vector<float> exp_u;
        BOOST_FOREACH(double improvement, vecImprovements)
            exp_u.push_back(m_scale * improvement / bestImprovement);

        int bestIndex = OTUtils::chooseCategoryFromPdf(OTUtils::convertHistogramToPdf(exp_u));
        bestImprovement = vecImprovements.at(bestIndex);
        bestSplit       = vecSplits.at(bestIndex);
    }

    int  featureIndex  = bestSplit->getFeatureIndex();
    bool isCategorical = Utils::hasElement(categoricalFeatures,featureIndex);

    shared_ptr<DecisionTreeNode> lhsChild       = createLhsChild(bestSplit);
    shared_ptr<DecisionTreeNode> rhsChild       = createRhsChild(bestSplit);
    shared_ptr<DecisionTreeNode> missingChild   = createMissingChild(bestSplit);

    nodeToSplit->defineSplit(bestSplit,lhsChild,rhsChild,missingChild);

    // if (m_parameters.verbose)
    //    vlcMessage.Write("Split at feature index " + ToString(bestSplit->getFeatureIndex()) + " at value " + ToString(bestSplit->getSplitValue()) + " with improvement " + ToString(bestSplit->getImprovement()));

    // finally, remove the node we just split from the terminal nodes, and add the children
    children.push_back(lhsChild);
    children.push_back(rhsChild);
    children.push_back(missingChild);
    
    return children;
}

struct FeatureSorter 
{
    FeatureSorter() 
        : featureIndexToSort(-1) 
    {}

    int featureIndexToSort;

    bool operator() (shared_ptr<DecisionTreeExperiment> a, shared_ptr<DecisionTreeExperiment> b) 
    {
        if (featureIndexToSort == -1)
            throw std::runtime_error("SortOnFeature object doesn't know which feature to sort on!");

        return a->getFeatureValue(featureIndexToSort) < b->getFeatureValue(featureIndexToSort);
    }
} featureSorter;

shared_ptr<SplitDefinition> NodeSplitter::createContinuousSplitDefinition( shared_ptr<DecisionTreeNode> node, int featureIndex )
{
    vector<shared_ptr<DecisionTreeExperiment> > sortedExperiments = node->getExperiments();
    // vector<shared_ptr<DecisionTreeExperiment> >& sortedExperiments = node->getSortedExperimentsForFeature(featureIndex);

    featureSorter.featureIndexToSort = featureIndex;
    sort(sortedExperiments.begin(), sortedExperiments.end(), featureSorter);

    double rhsSumZ = 0, rhsSumW = 0, lhsSumZ = 0, lhsSumW = 0;
    double missingSumZ = 0, missingSumW = 0; 

    vector<double> vecLhsSumZ;
    vector<double> vecLhsSumW;
    vector<int>    vecLhsCount;
    vector<double> vecRhsSumZ;
    vector<double> vecRhsSumW;
    vector<int>    vecRhsCount;
    vector<double> vecMissingSumZ;
    vector<double> vecMissingSumW;
    vector<int>    vecMissingCount;
    vector<double> vecImprovement;
    vector<int>    vecPosition;

    double bestLhsSumZ;     
    double bestLhsSumW;   
    int    bestLhsCount;    
    double bestRhsSumZ;     
    double bestRhsSumW;     
    int    bestRhsCount;    
    double bestMissingSumZ; 
    double bestMissingSumW; 
    int    bestMissingCount;

    double bestImprovement = 0.0;
    int    bestPosition    = -1;
    int    bestIndex = -1;    

    int    lhsCount = 0, missingCount = 0;
    int    rhsCount = (int) sortedExperiments.size();

    rhsSumZ = node->getSumZ();
    rhsSumW = node->getSumW();
    int position = -1;
    double previousFeatureValue = 0;

    BOOST_FOREACH(shared_ptr<DecisionTreeExperiment> experiment, sortedExperiments)
    {
        double featureValue = experiment->getFeatureValue(featureIndex);

        if (featureValue != previousFeatureValue)
        {
            // vlcMessage.Write("featureValue != previousFeatureValue => " + ToString(featureValue != previousFeatureValue));
            // vlcMessage.Write("lhsSumW => " + ToString(lhsSumW));
            // vlcMessage.Write("lhsSumZ => " + ToString(lhsSumZ));
            // vlcMessage.Write("rhsSumW => " + ToString(rhsSumW));
            // vlcMessage.Write("rhsSumZ => " + ToString(rhsSumZ));
            // vlcMessage.Write("missingSumW => " + ToString(missingSumW));
            // vlcMessage.Write("missingSumZ => " + ToString(missingSumZ));
            // vlcMessage.Write("improvement => " + ToString(improvement));
            // vlcMessage.Write("bestImprovement => " + ToString(bestImprovement));
            // vlcMessage.Write("m_minObservations => " + ToString(m_minObservations));
        }

        if (featureValue != previousFeatureValue &&
            lhsCount >= m_minObservations &&
            rhsCount >= m_minObservations
            )
        {
            double improvement = calculateImprovement(lhsSumW, lhsSumZ, rhsSumW, rhsSumZ, missingSumW, missingSumZ);    
            vecPosition.push_back(position);
            vecImprovement.push_back(improvement);
            vecLhsSumZ.push_back(lhsSumZ);
            vecLhsSumW.push_back(lhsSumW);
            vecLhsCount.push_back(lhsCount);
            vecRhsSumZ.push_back(rhsSumZ);
            vecRhsSumW.push_back(rhsSumW);
            vecRhsCount.push_back(rhsCount);
            vecMissingSumZ.push_back(missingSumZ);
            vecMissingSumW.push_back(missingSumW);
            vecMissingCount.push_back(missingCount);

            if (improvement > bestImprovement)
            {
                bestImprovement = improvement;
                bestPosition    = position;
                bestIndex       = (int) vecPosition.size() - 1;
            }
            //             if (improvement > bestImprovement)
            //             {
            //                 bestImprovement     = improvement;
            //                 bestPosition        = position;
            //                 bestLhsSumZ         = lhsSumZ;
            //                 bestLhsSumW         = lhsSumW;
            //                 bestLhsCount        = lhsCount;
            //                 bestRhsSumZ         = rhsSumZ;
            //                 bestRhsSumW         = rhsSumW;
            //                 bestRhsCount        = rhsCount;
            //                 bestMissingSumZ     = missingSumZ;
            //                 bestMissingSumW     = missingSumW;
            //                 bestMissingCount    = missingCount;
            //             }
            // vlcMessage.Write("improvement => " + ToString(improvement));

        }
        double weight = experiment->getWeight();
        double z      = experiment->getZ();
        rhsSumZ -= weight * z;
        rhsSumW -= weight;
        --rhsCount;

        if (m_missingValueDefined && featureValue == m_missingValue)
        {
            missingSumZ += weight * z;
            missingSumW += weight;
            ++missingCount;
        }
        else
        {
            lhsSumZ += weight * z;
            lhsSumW += weight;
            ++lhsCount;
        }

        previousFeatureValue = featureValue;
        ++position;
    }

    if (bestPosition == -1)
        return shared_ptr<SplitDefinition>();

    if (m_scale != std::numeric_limits<double>::infinity() && vecImprovement.size() > 1)
    {
        vector<float> exp_u;
        exp_u.reserve(vecImprovement.size());
        BOOST_FOREACH(double& improvement, vecImprovement)
        {
            exp_u.push_back(exp(m_scale * improvement / bestImprovement));
        }
        bestIndex = OTUtils::chooseCategoryFromPdf(OTUtils::convertHistogramToPdf(exp_u),"improvements");
    }

    bestLhsSumZ        = vecLhsSumZ.at(bestIndex);
    bestLhsSumW        = vecLhsSumW.at(bestIndex);
    bestLhsCount       = vecLhsCount.at(bestIndex);
    bestRhsSumZ        = vecRhsSumZ.at(bestIndex);
    bestRhsSumW        = vecRhsSumW.at(bestIndex);
    bestRhsCount       = vecRhsCount.at(bestIndex);
    bestMissingSumZ    = vecMissingSumZ.at(bestIndex);
    bestMissingSumW    = vecMissingSumW.at(bestIndex);
    bestMissingCount   = vecMissingCount.at(bestIndex);
    bestImprovement    = vecImprovement.at(bestIndex);
    bestPosition       = vecPosition.at(bestIndex);

    if (bestPosition >= (int) (sortedExperiments.size()-1))
        throw std::runtime_error(string("Unexpected bestPosition: ") + lexical_cast<string>(bestPosition));

    double lhsFeatureValue = sortedExperiments.at(bestPosition)->getFeatureValue(featureIndex);
    double rhsFeatureValue = sortedExperiments.at(bestPosition + 1)->getFeatureValue(featureIndex);

    double splitValue;
    if (m_missingValueDefined && (lhsFeatureValue == m_missingValue))
        splitValue = m_missingValue;
    else
        splitValue = 0.5 * (lhsFeatureValue + rhsFeatureValue);


    shared_ptr<SplitDefinition> splitDefinition = shared_ptr<SplitDefinition>
        (new SplitDefinition(node, featureIndex, splitValue, bestLhsSumZ, bestLhsSumW, bestLhsCount, 
        bestRhsSumZ, bestRhsSumW, bestRhsCount, bestMissingSumZ, bestMissingSumW,
        bestMissingCount, bestImprovement));

    // create SplitDefinition
    return splitDefinition;
}

shared_ptr<SplitDefinition> NodeSplitter::createCategoricalSplitDefinition( shared_ptr<DecisionTreeNode> node, int featureIndex )
{
    vector<shared_ptr<DecisionTreeExperiment> > experiments = node->getExperiments();

    map<double, CategoryInfo> experimentsPerCategory;

    double missingSumZ = 0, missingSumW = 0;
    int    missingCount = 0;
    BOOST_FOREACH(shared_ptr<DecisionTreeExperiment>& experiment, experiments)
    {
        double featureValue = experiment->getFeatureValue(featureIndex);

        if (m_missingValueDefined && m_missingValue == featureValue)
        {
            double w = experiment->getWeight();
            double z = experiment->getZ();
            missingSumZ += w * z;
            missingSumW += w;
            missingCount++;
        }
        else
        {
            CategoryInfo& info = experimentsPerCategory[featureValue];
            info.category = featureValue;
            info.addExperiment(experiment);
        }
    }

    if (experimentsPerCategory.size() == 1)
        return shared_ptr<SplitDefinition>(); // can't split one thing!

    // put them into a vector to make sorting easier!
    vector<CategoryInfo> categoryInfoVector;
    typedef pair<double, CategoryInfo> ElementType;
    BOOST_FOREACH(ElementType e, experimentsPerCategory)
        categoryInfoVector.push_back(e.second);

    sort(categoryInfoVector.begin(), categoryInfoVector.end());

    double rhsSumZ = 0, rhsSumW = 0, lhsSumZ = 0, lhsSumW = 0;
    double bestImprovement = 0.0;
    int    bestPosition    = -1;
    double bestLhsSumZ     = -1;
    double bestLhsSumW     = -1;
    int    bestLhsCount    = -1;
    double bestRhsSumZ     = -1;
    double bestRhsSumW     = -1;
    int    bestRhsCount    = -1;
    int    lhsCount = 0, rhsCount = 0;

    rhsSumZ = node->getSumZ();
    rhsSumW = node->getSumW();
    rhsCount = (int) experiments.size();

    int position = -1;

    BOOST_FOREACH(CategoryInfo& e, categoryInfoVector)
    {
        double improvement = calculateImprovement(lhsSumW, lhsSumZ, rhsSumW, rhsSumZ, missingSumW, missingSumZ);
        if (   improvement >  bestImprovement 
            && lhsCount    >= m_minObservations
            && rhsCount    >= m_minObservations)
        {
            bestImprovement = improvement;
            bestPosition    = position;
            bestLhsSumZ     = lhsSumZ;
            bestLhsSumW     = lhsSumW;
            bestLhsCount    = lhsCount;
            bestRhsSumZ     = rhsSumZ;
            bestRhsSumW     = rhsSumW;
            bestRhsCount    = rhsCount;
        }

        ++position;
        rhsSumW  -= e.sumW;
        rhsSumZ  -= e.sumZ;
        rhsCount -= e.countN;

        lhsSumW  += e.sumW;
        lhsSumZ  += e.sumZ;
        lhsCount += e.countN;
    }

    if (bestPosition == -1 && missingSumW == 0)
        return shared_ptr<SplitDefinition>();

    set<double> lhsCategories;
    set<double> rhsCategories;

    int index = -1;
    BOOST_FOREACH(CategoryInfo& info, categoryInfoVector)
    {
        ++index;
        if (index <= bestPosition)
            lhsCategories.insert(info.category);
        else
            rhsCategories.insert(info.category);
    }

    // we have what we need to create a split definition now
    shared_ptr<SplitDefinition> splitDefinition = shared_ptr<SplitDefinition>
        (new SplitDefinition(node, featureIndex, lhsCategories, rhsCategories, bestLhsSumZ, bestLhsSumW,
        bestLhsCount, bestRhsSumZ, bestRhsSumW, bestRhsCount, missingSumZ, missingSumW, missingCount, bestImprovement));

    return splitDefinition;
}

shared_ptr<SplitDefinition> NodeSplitter::createSplitDefinition( shared_ptr<DecisionTreeNode> node, int featureIndex )
{
    if (Utils::hasElement(m_data->getCategoricalFeatureIndices(),featureIndex))
        return createCategoricalSplitDefinition(node, featureIndex);
    else
        return createContinuousSplitDefinition(node, featureIndex);
}



