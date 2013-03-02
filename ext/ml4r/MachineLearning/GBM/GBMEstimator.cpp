#include "MachineLearning/GBM/GBMEstimator.h"
#include "MachineLearning/GBM/GBMParameters.h"
#include "MachineLearning/GBM/GBMOutput.h"
#include "MachineLearning/GBM/BernoulliCalculator.h"
#include "MachineLearning/GBM/GaussianCalculator.h"
#include "MachineLearning/MLData/MLData.h"
#include "MachineLearning/MLUtils.h"
#include "MachineLearning/DecisionTree/SplitDefinition.h"
#include "MachineLearning/DecisionTree/NodeSplitterCategorical.h"
#include "MachineLearning/DecisionTree/NodeSplitterContinuous.h"
#include "MachineLearning/DecisionTree/DecisionTreeExperiment.h"
#include "MachineLearning/DecisionTree/DecisionTreeNode.h"
#include "MachineLearning/DecisionTree/FeatureInteraction.h"

#include <algorithm>
#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>
#include <boost/lexical_cast.hpp>
using boost::lexical_cast;
using boost::make_shared;
using std::make_pair;

#include "utils/VlcMessage.h"

GBMEstimator::GBMEstimator(MLData* data, vector<shared_ptr<MLExperiment> > experiments, shared_ptr<GBMParameters> parameters)
    : MLEstimator(data, experiments), m_parameters(parameters)
{
    m_decisionTreeExperiments.reserve(experiments.size());
    BOOST_FOREACH(shared_ptr<MLExperiment>& experiment, experiments)
        m_decisionTreeExperiments.push_back(make_shared<DecisionTreeExperiment>(experiment));
    
    vector<int> experimentIndicies;
    experimentIndicies.reserve(experiments.size());
    BOOST_FOREACH(shared_ptr<MLExperiment>& experiment, experiments)
        experimentIndicies.push_back(experiment->getExperimentIndex());

    m_output = shared_ptr<GBMOutput>(new GBMOutput(m_data, experimentIndicies, m_parameters));
    
    initializeEstimator();
}

GBMEstimator::~GBMEstimator() {}

shared_ptr<MLOutput> GBMEstimator::estimate()
{
    initialiseGBMExperimentData();

    for (int iteration = 0; iteration < m_parameters->numIterations; ++iteration)
    {
        if (m_parameters->verbose)
            vlcMessage.Begin((string("Iteration ") + lexical_cast<string>(iteration + 1)).c_str());

        performIteration();

        if (m_parameters->verbose)
            vlcMessage.End();
    }
    return shared_ptr<MLOutput>(m_output);
}


void GBMEstimator::estimateMore(int numTrees)
{
    int numberOfExistingTrees = m_output->getNumTrees();

    for (int iteration = 0; iteration < numTrees; ++iteration)
    {
        if (m_parameters->verbose)
            vlcMessage.Begin(string("Iteration ") + lexical_cast<string>(numberOfExistingTrees + iteration + 1));

        performIteration();

        if (m_parameters->verbose)
            vlcMessage.End();
    }
}


void GBMEstimator::performIteration()
{
    // update Z based on latest F
    vector<shared_ptr<DecisionTreeExperiment> >& experiments = m_decisionTreeExperiments;

    long bagSize = m_parameters->bagFraction * m_decisionTreeExperiments.size();
    
    pair<vector<shared_ptr<DecisionTreeExperiment> >,vector<shared_ptr<DecisionTreeExperiment> > >  inAndOutOfBagExperiments = 
        MLUtils::bagObjectsWithoutReplacement<shared_ptr<DecisionTreeExperiment> >(m_decisionTreeExperiments, (int) bagSize);

    if (m_parameters->verbose)
        vlcMessage.Begin("Constructing decision tree");
    
    if (m_parameters->greedy)
        constructDecisionTree(inAndOutOfBagExperiments.first);
    else
        constructGenerousDecisionTree(inAndOutOfBagExperiments.first, m_parameters->rfToLevel);

    m_output->addHeadDecisionTreeNode(m_decisionTreeHead);

    if (m_parameters->verbose)
        vlcMessage.End();

    // update F
    calculateFIncrementPerDecisionTreeNode();
    m_output->addFIncrements(m_FIncrements);

    // applyFIncrementToInBagExperiments();
    applyFIncrementToExperiments(experiments);

    // update predictions and Z
    updatePredictions(experiments);
    updateZ(experiments);
    reportDeviance(experiments);
    deleteRedundantData();
}

void GBMEstimator::constructFeatureIndices()
{
    BOOST_FOREACH(string& feature, m_parameters->featuresToRun)
    {
        // note that in a given run, we may not "run" with all loaded variables.
        m_featureIndices.push_back(m_data->getFeatureIndex(feature));
    }
}

void GBMEstimator::populateInitialF()
{
    m_gbmCalculator->populateInitialF(m_decisionTreeExperiments, m_data->initialPredictionsDefined());
    
    if (!m_data->initialPredictionsDefined())
        m_output->setMeanY(m_decisionTreeExperiments.front()->getPrediction());
}

void GBMEstimator::updateZ(vector<shared_ptr<DecisionTreeExperiment> >& experiments)
{
    m_gbmCalculator->updateZ(experiments);
}

void GBMEstimator::updatePredictions(vector<shared_ptr<DecisionTreeExperiment> >& experiments)
{
    // convert from F to prediction
    m_gbmCalculator->updatePredictions(experiments);
}

void GBMEstimator::constructDecisionTree(vector<shared_ptr<DecisionTreeExperiment> >& experiments)
{
    // create a head DecisionTreeNode
    double sumZ = 0.0, sumW = 0.0;
    BOOST_FOREACH(shared_ptr<DecisionTreeExperiment>& e, experiments)
    {
        double w = e->getWeight();
        sumW += w;
        sumZ += w * e->getZ();
    }
    m_decisionTreeHead = shared_ptr<DecisionTreeNode>(new DecisionTreeNode(experiments, sumZ, sumW, ROOT, shared_ptr<SplitDefinition>()));

    // m_decisionTreeHead->setSortedExperiments(bagSortedExperiments(experiments));

    m_terminalNodes.clear();
    m_terminalNodes.insert(m_decisionTreeHead);
    set<shared_ptr<DecisionTreeNode> > nodesToSplit;
    nodesToSplit.insert(m_decisionTreeHead);

    // map from a decision tree node and feature index to a potential split definition
    map<pair<shared_ptr<DecisionTreeNode>, int>, shared_ptr<SplitDefinition> > potentialSplitDefinitions;
    set<int>& categoricalFeatures = m_data->getCategoricalFeatureIndices();

    NodeSplitter splitter(m_data, m_parameters->minObservations, m_parameters->scale);

    for (int k = 0; k < m_parameters->growKDecisionTreeNodes; ++k)
    {
        // choose M variables to test splitting on
        // find terminal node with best improvement for any of those variables
        vector<int> featuresToConsider = getRandomFeatureList();
        // pair<shared_ptr<DecisionTreeNode>, int> bestNodeFeature;
        shared_ptr<SplitDefinition> bestSplit;
        double                      bestImprovement = 0.0;

        BOOST_FOREACH(shared_ptr<DecisionTreeNode> node, nodesToSplit)
        {
            if (node->getSumW() == 0)
                continue;

            BOOST_FOREACH(int featureIndex, featuresToConsider)
            {
                pair<shared_ptr<DecisionTreeNode>, int> e = make_pair(node, featureIndex);

                if (potentialSplitDefinitions.find(e) == potentialSplitDefinitions.end())
                {
                    if (Utils::hasElement(categoricalFeatures, featureIndex))
                        potentialSplitDefinitions[e] = splitter.createCategoricalSplitDefinition(node, featureIndex);
                    else
                        potentialSplitDefinitions[e] = splitter.createContinuousSplitDefinition(node, featureIndex);
                }

                shared_ptr<SplitDefinition> splitDefinition = potentialSplitDefinitions[e];

                if (!splitDefinition.get()) // it returned an invalid
                    continue;

                
                if (splitDefinition->getImprovement() > bestImprovement)
                {
                    bestImprovement = splitDefinition->getImprovement();
                    bestSplit       = splitDefinition;
                    // bestNodeFeature = e;
                }
            }
        }

        if (bestImprovement == 0.0)
        {
            if (m_parameters->verbose)
                vlcMessage.Write("Can't split the tree any further.", 1);

            return; // we obviously didn't get any love out of our terminal nodes - probably means they
            // can't split any further.
        }
        // we now have our best split, so do it!!!

        int  featureIndex = bestSplit->getFeatureIndex();
        bool isCategorical = (categoricalFeatures.find(featureIndex) != categoricalFeatures.end());

        shared_ptr<DecisionTreeNode> lhsChild       = splitter.createLhsChild(bestSplit);
        shared_ptr<DecisionTreeNode> rhsChild       = splitter.createRhsChild(bestSplit);
        shared_ptr<DecisionTreeNode> missingChild   = splitter.createMissingChild(bestSplit);

        shared_ptr<DecisionTreeNode> nodeToSplit = bestSplit->getNodeToSplit();

        nodeToSplit->defineSplit(bestSplit, lhsChild,rhsChild,missingChild);

        // finally, remove the node we just split from the terminal nodes, and add the children
        nodesToSplit.erase(nodeToSplit);
        nodesToSplit.insert(lhsChild);
        nodesToSplit.insert(rhsChild);
        nodesToSplit.insert(missingChild);

        // if it's categorical, there is a chance a new category will come along, and we won't be able to split on it.
        // which would make this a potential terminal node.
        // so only erase if it's continuous
        //if (!isCategorical) 
        //    m_terminalNodes.erase(nodeToSplit);

        m_terminalNodes.insert(lhsChild);
        m_terminalNodes.insert(rhsChild);
        m_terminalNodes.insert(missingChild);

        if (m_parameters->verbose)
        {
            vlcMessage.Write("Level " + lexical_cast<string>(k+1) + ": Split on feature " 
                + m_data->getFeatures().at(featureIndex) + " at "
                + lexical_cast<string>(bestSplit->getSplitValue()) + ".  Improvement: "
                + lexical_cast<string>(bestImprovement));
        }
        
    }
}


void GBMEstimator::constructGenerousDecisionTree(vector<shared_ptr<DecisionTreeExperiment> >& experiments, int rfToLevel)
{
    // set Z to Y for RF part
    BOOST_FOREACH(shared_ptr<DecisionTreeExperiment>& experiment, experiments)
    {
        experiment->setZ(experiment->getY());
    }

    double sumZ = 0.0, sumW = 0.0;
    BOOST_FOREACH(shared_ptr<DecisionTreeExperiment>& e, experiments)
    {
        double w = e->getWeight();
        sumW += w;
        sumZ += w * e->getZ();
    }

    m_decisionTreeHead = shared_ptr<DecisionTreeNode>(new DecisionTreeNode(experiments, sumZ, sumW, ROOT, shared_ptr<SplitDefinition>()));

    // m_decisionTreeHead->setSortedExperiments(bagSortedExperiments(experiments));

    m_terminalNodes.clear();
    m_terminalNodes.insert(m_decisionTreeHead);
    vector<shared_ptr<DecisionTreeNode> > nodesToSplit;
    nodesToSplit.push_back(m_decisionTreeHead);
    vector<shared_ptr<DecisionTreeNode> > nextNodesToSplit;

    // map from a decision tree node and feature index to a potential split definition
    set<int>& categoricalFeatures = m_data->getCategoricalFeatureIndices();
     
    NodeSplitter splitter(m_data, m_parameters->minObservations, m_parameters->scale);

    for (int level = 0; level < rfToLevel; ++level)
    {
        BOOST_FOREACH(shared_ptr<DecisionTreeNode>& nodeToSplit, nodesToSplit)
        {
            vector<int> featuresToConsider = getRandomFeatureList();
            vector<shared_ptr<DecisionTreeNode> > children = splitter.splitNode(nodeToSplit, featuresToConsider);
            BOOST_FOREACH(shared_ptr<DecisionTreeNode>& child, children)
            {
                nextNodesToSplit.push_back(child);
                m_terminalNodes.insert(child);
            }
        }
        nodesToSplit = nextNodesToSplit;
        nextNodesToSplit.clear();
    }
    // have successfully built a random forest to depth rfLevels
    // reset Z to residuals
    updateZ(experiments);

    // now gradient boost on the nodesToSplit
    BOOST_FOREACH(shared_ptr<DecisionTreeNode>& nodeToSplit, nodesToSplit)
    {
        nodeToSplit->updateSums();
        vector<int> featuresToConsider = getRandomFeatureList();
        vector<shared_ptr<DecisionTreeNode> > children = splitter.splitNode(nodeToSplit, featuresToConsider);
        BOOST_FOREACH(shared_ptr<DecisionTreeNode>& child, children)
            m_terminalNodes.insert(child);
    }
}

vector<int> GBMEstimator::getRandomFeatureList()
{
    vector<int> randomlySelectedFeatures;
    map<int, bool> featureChosen;

    unsigned int numberToChoose = std::min((int)m_featureIndices.size(), m_parameters->tryMVariables);

    while (randomlySelectedFeatures.size() < numberToChoose)
    {
        long r = rand();
        long index = r * (1.0 / (RAND_MAX + 1L)) * m_featureIndices.size();
        if (!featureChosen[index] == 1)
        {
            featureChosen[index] = 1;
            randomlySelectedFeatures.push_back(m_featureIndices.at(index));
        }

    }
    return randomlySelectedFeatures;
}



void GBMEstimator::calculateFIncrementPerDecisionTreeNode()
{
    m_FIncrements.clear();
    BOOST_FOREACH(shared_ptr<DecisionTreeNode> node, m_terminalNodes)
    {
        vector<shared_ptr<DecisionTreeExperiment> > experiments = node->getExperiments();
        
        double fIncrement = m_gbmCalculator->computeFIncrement(experiments);
        m_FIncrements[node] = fIncrement * m_parameters->shrinkageFactor;
    }
}

void GBMEstimator::applyFIncrementToInBagExperiments()
{
    // THIS IS BAD, because when bagging with replacement, you can increment the same record twice!!
    // we know which experiments are in-bag because they're stored by the terminal nodes!
    BOOST_FOREACH(shared_ptr<DecisionTreeNode> node, m_terminalNodes)
    {
        double increment = m_FIncrements[node];

        BOOST_FOREACH(shared_ptr<DecisionTreeExperiment> experiment, node->getExperiments())
        {
            // vlcMessage.Write("experiment->getExperimentIndex() => " + ToString(experiment->getExperimentIndex()));
            // vlcMessage.Write("increment => " + ToString(increment));
            experiment->incrementF(increment);
        }
    }
}

void GBMEstimator::applyFIncrementToExperiments(vector<shared_ptr<DecisionTreeExperiment> >& experiments)
{
    BOOST_FOREACH(shared_ptr<DecisionTreeExperiment>& experiment, experiments)
    {
        // get appropriate node
        shared_ptr<DecisionTreeNode> terminalNode = 
            (m_decisionTreeHead->isTerminalNode() ? m_decisionTreeHead : m_decisionTreeHead->getTerminalNodeForExperiment(experiment));

        if (terminalNode.get() == 0)
            terminalNode = m_decisionTreeHead;

        if (m_FIncrements.find(terminalNode) == m_FIncrements.end())
            throw std::runtime_error("We have no increment for this terminal node!!");

        double incrementF = m_FIncrements[terminalNode];
        experiment->incrementF(incrementF);
    }
}

void GBMEstimator::initialiseGBMExperimentData()
{
    populateInitialF();
    updatePredictions(m_decisionTreeExperiments);
    updateZ(m_decisionTreeExperiments);
}

void GBMEstimator::reportDeviance(vector<shared_ptr<DecisionTreeExperiment> >& experiments)
{
    if (m_parameters->verbose)
        vlcMessage.Write("Deviance: " + lexical_cast<string>(m_gbmCalculator->calculateDeviance(experiments)));
}

void GBMEstimator::deleteRedundantData()
{
    m_decisionTreeHead->clearExperimentsWithinTree();
}

// map<int, vector<shared_ptr<DecisionTreeExperiment> > > 
//     GBMEstimator::partitionSortedExperiments(shared_ptr<SplitDefinition> splitDefinition, 
//                                              Partition partition)
// {
//     map<int, vector<shared_ptr<DecisionTreeExperiment> > >& sortedExperiments = 
//         splitDefinition->getNodeToSplit()->getSortedExperiments();
// 
//     map<int, vector<shared_ptr<DecisionTreeExperiment> > > partitionSortedExperiments;
//     
//     BOOST_FOREACH(auto& e, sortedExperiments)
//     {
//         int featureIndex = e.first;
//         vector<shared_ptr<DecisionTreeExperiment> >& experimentsForFeature = e.second;
// 
//         if (experimentsForFeature.size() == 0)
//             continue;
//         partitionSortedExperiments[featureIndex] = partitionExperiments(experimentsForFeature, splitDefinition, partition);
//     }
//     return partitionSortedExperiments;
// }



// void GBMEstimator::sortTrainingExperiments()
// {
//     BOOST_FOREACH(auto& featureIndex, m_featureIndices)
//     {
//         vector<shared_ptr<DecisionTreeExperiment> > experiments = m_trainingExperiments;
//         featureSorter.featureIndexToSort = featureIndex;
// 
//         sort(experiments.begin(), experiments.end(), featureSorter);
//         m_sortedTrainingExperiments[featureIndex] = experiments;
//     }
// }

// map<int, vector<shared_ptr<DecisionTreeExperiment> > > GBMEstimator::bagSortedExperiments(vector<shared_ptr<DecisionTreeExperiment> >& baggedExperiments)
// {
//     vector<bool> inBag(m_data->getExperiments().size(), false);
//         
//     BOOST_FOREACH(auto& experiment, baggedExperiments)
//         inBag.at(experiment->getExperimentIndex()) = true;
// 
//     map<int, vector<shared_ptr<DecisionTreeExperiment> > > sortedExperiments;
//     BOOST_FOREACH(auto& e, m_sortedTrainingExperiments)
//     {
//         int featureIndex                                        = e.first;
//         vector<shared_ptr<DecisionTreeExperiment> >& experiments = e.second;
// 
//         vector<shared_ptr<DecisionTreeExperiment> >& baggedSortedExperiments = sortedExperiments[featureIndex];
//         baggedSortedExperiments.reserve(baggedExperiments.size());
//         BOOST_FOREACH(auto& experiment, experiments)
//         {
//             if (inBag.at(experiment->getExperimentIndex()))
//                 baggedSortedExperiments.push_back(experiment);
//         }
//     }
//     return sortedExperiments;
// }

void GBMEstimator::initializeEstimator()
{
    m_missingValueDefined = m_data->missingValueDefined();
    if (m_missingValueDefined)
        m_missingValue        = m_data->getMissingValue();

    constructFeatureIndices();
    // sortTrainingExperiments();

    if (m_parameters->distribution == GAUSSIAN)
        m_gbmCalculator = make_shared<GaussianCalculator>();
    else if (m_parameters->distribution == BERNOULLI)
        m_gbmCalculator = make_shared<BernoulliCalculator>();
}

struct FeatureInteractionSorter 
{
    FeatureInteractionSorter() 
    {}

    bool operator() (FeatureInteraction a, FeatureInteraction b) 
    {
        return a.secondarySplitDefinition->getImprovement() > b.secondarySplitDefinition->getImprovement();
    }
} featureInteractionSorter;

vector<FeatureInteraction> GBMEstimator::findInteractions(int howMany)
{
    vlcMessage.Write("Finding interactions!");

    vector<shared_ptr<DecisionTreeExperiment> >& experiments = m_decisionTreeExperiments;

    vlcMessage.Write("Initialising");

    initialiseGBMExperimentData();
    // now reset Z to be Y
    BOOST_FOREACH(shared_ptr<DecisionTreeExperiment>& experiment, experiments)
    {
        experiment->setZ(experiment->getY());
    }

    double sumZ = 0.0, sumW = 0.0;
    BOOST_FOREACH(shared_ptr<DecisionTreeExperiment>& e, experiments)
    {
        double w = e->getWeight();
        sumW += w;
        sumZ += w * e->getZ();
    }

    vlcMessage.Write("Creating head");
    m_decisionTreeHead = shared_ptr<DecisionTreeNode>(new DecisionTreeNode(experiments, sumZ, sumW, ROOT, shared_ptr<SplitDefinition>()));

    // map from a decision tree node and feature index to a potential split definition
    set<int>& categoricalFeatures = m_data->getCategoricalFeatureIndices();

    NodeSplitter splitter(m_data, m_parameters->minObservations, m_parameters->scale);

    map<int, vector<shared_ptr<DecisionTreeNode> > > topLevelSplits;

    vector<int> singleFeature;
    singleFeature.push_back(0);

    vlcMessage.Write("Creating top level splits");
    vector<string>& featureNames = m_data->getFeatures();
    BOOST_FOREACH(int& featureIndex, m_featureIndices)
    {
        vlcMessage.Write("Top level " + lexical_cast<string>(featureIndex));
        // find the best split definition for this feature index
        singleFeature.at(0) = featureIndex;    
        topLevelSplits[featureIndex] = splitter.splitNode(m_decisionTreeHead, singleFeature);
        if (featureNames.at(featureIndex) == "Quan_4")
        {
            shared_ptr<SplitDefinition> def = topLevelSplits[featureIndex].at(0)->getParentSplitDefinition();
            vlcMessage.Write("Splitting Quan_4");
            vlcMessage.Write("Imp: "         + lexical_cast<string>(def->getImprovement()));
            vlcMessage.Write("Split value "  + lexical_cast<string>(def->getSplitValue()));
            vlcMessage.Write("LhsSumZ: "     + lexical_cast<string>(def->getLhsSumZ()));
            vlcMessage.Write("LhsSumW: "     + lexical_cast<string>(def->getLhsSumW()));
            vlcMessage.Write("RhsSumZ: "     + lexical_cast<string>(def->getRhsSumZ()));
            vlcMessage.Write("RhsSumW: "     + lexical_cast<string>(def->getRhsSumW()));
            vlcMessage.Write("MissingSumZ: " + lexical_cast<string>(def->getMissingSumZ()));
            vlcMessage.Write("MissingSumW: " + lexical_cast<string>(def->getMissingSumW()));
        }
    }

    vlcMessage.Write("Updating Z");
    // reset Z to residuals
    updateZ(m_decisionTreeExperiments);

    vlcMessage.Write("Allocating mem");
    vector<FeatureInteraction> featureInteractions;
    featureInteractions.reserve(m_featureIndices.size() * m_featureIndices.size() * 3);

    
    typedef pair<int, vector<shared_ptr<DecisionTreeNode> > > ElementType;
    BOOST_FOREACH(ElementType e, topLevelSplits)
    {
        int primaryFeatureIndex = e.first;
        vector<shared_ptr<DecisionTreeNode> > children = e.second;

        if (children.size() == 0)
            continue;

        vlcMessage.Write("Secondary splits on " + lexical_cast<string>(primaryFeatureIndex));
        shared_ptr<SplitDefinition> primarySplitDefinition = children.at(0)->getParentSplitDefinition();
        BOOST_FOREACH(shared_ptr<DecisionTreeNode>& child, children)
        {
            if (child->getSumW() == 0)
                continue;

            // update sumZ / sumW
            child->updateSums();

            BOOST_FOREACH(int& secondaryFeatureIndex, m_featureIndices)
            {
                if (secondaryFeatureIndex == primaryFeatureIndex)
                    continue;

                shared_ptr<SplitDefinition> secondarySplitDefinition = splitter.createSplitDefinition(child,secondaryFeatureIndex);
                if (secondarySplitDefinition.get() == 0)
                    continue;

                // vlcMessage.Write("Secondary split on with imp " + ToString(secondarySplitDefinition->getImprovement()) + " lhsSumZ: " + ToString(secondarySplitDefinition->getLhsSumZ()) + " lhsSumW: " + ToString(secondarySplitDefinition->getLhsSumW()) + " rhsSumZ: " + ToString(secondarySplitDefinition->getRhsSumZ()) + " rhsSumW: " + ToString(secondarySplitDefinition->getRhsSumW()) + " missingSumZ: " + ToString(secondarySplitDefinition->getMissingSumZ()) + " missingSumZ: " + ToString(secondarySplitDefinition->getMissingSumW()));
                FeatureInteraction interaction(primarySplitDefinition,secondarySplitDefinition,child->getPartition());
                featureInteractions.push_back(interaction);
            }
        }
    }
    vlcMessage.Write("Sorting...");

    sort(featureInteractions.begin(), featureInteractions.end(), featureInteractionSorter);
    
    return vector<FeatureInteraction>(featureInteractions.begin(), featureInteractions.begin() + howMany);
}
