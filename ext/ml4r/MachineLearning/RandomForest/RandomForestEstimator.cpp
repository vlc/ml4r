#include "MachineLearning/RandomForest/RandomForestEstimator.h"
#include "MachineLearning/MLData/MLData.h"
#include "MachineLearning/DecisionTree/DecisionTreeExperiment.h"
#include "MachineLearning/DecisionTree/DecisionTreeNode.h"
#include "MachineLearning/DecisionTree/NodeSplitterCategorical.h"
#include "MachineLearning/DecisionTree/NodeSplitterContinuous.h"
#include "MachineLearning/DecisionTree/SplitDefinition.h"
#include "MachineLearning/RandomForest/RandomForestOutput.h"
#include "MachineLearning/MLUtils.h"

RandomForestEstimator::RandomForestEstimator(MLData* data, 
                                             vector<shared_ptr<MLExperiment> > experiments, 
                                             shared_ptr<RandomForestParameters> parameters)
                                             : MLEstimator(data, experiments), m_parameters(parameters)
{
    m_decisionTreeExperiments.reserve(experiments.size());
    BOOST_FOREACH(auto& experiment, experiments)
        m_decisionTreeExperiments.push_back(make_shared<DecisionTreeExperiment>(experiment));

    vector<int> experimentIndicies;
    experimentIndicies.reserve(experiments.size());
    BOOST_FOREACH(auto& experiment, experiments)
        experimentIndicies.push_back(experiment->getExperimentIndex());

    m_output = shared_ptr<RandomForestOutput>(new RandomForestOutput(m_data, experimentIndicies, m_parameters));
}

RandomForestEstimator::~RandomForestEstimator()
{

}

shared_ptr<MLOutput> RandomForestEstimator::estimate()
{
    initializeEstimator();
    updateZ();

    for (int iteration = 0; iteration < m_parameters->numIterations; ++iteration)
    {
        if (m_parameters->verbose)
            vlcMessage.Begin((string("Iteration ") + ToString(iteration + 1)).c_str());

        performIteration();

        if (m_parameters->verbose)
            vlcMessage.End();
    }
    return shared_ptr<MLOutput>(m_output);
}

shared_ptr<MLOutput> RandomForestEstimator::estimateMore(int numTrees)
{
    initializeEstimator();
    updateZ();
    int numberOfExistingTrees = m_output->getNumTrees();

    for (int iteration = 0; iteration < numTrees; ++iteration)
    {
        if (m_parameters->verbose)
            vlcMessage.Begin((string("Iteration ") + ToString(numberOfExistingTrees + iteration + 1)).c_str());

        performIteration();

        if (m_parameters->verbose)
            vlcMessage.End();
    }
    return shared_ptr<MLOutput>(m_output);
}

void RandomForestEstimator::updateZ()
{
    BOOST_FOREACH(auto& e, m_decisionTreeExperiments)
        e->setZ(e->getY());
}

void RandomForestEstimator::performIteration()
{
    vector<shared_ptr<DecisionTreeExperiment> > experiments;
    int bagSize = m_decisionTreeExperiments.size() * m_parameters->bagFraction;

    if (m_parameters->withReplacement)
        experiments = MLUtils::bagObjectsWithReplacement<shared_ptr<DecisionTreeExperiment> >(m_decisionTreeExperiments, bagSize);
    else
    {
        pair<vector<shared_ptr<DecisionTreeExperiment> >,vector<shared_ptr<DecisionTreeExperiment> > > inAndOutOfBag =
            MLUtils::bagObjectsWithoutReplacement<shared_ptr<DecisionTreeExperiment> >(m_decisionTreeExperiments, min(m_decisionTreeExperiments.size(), bagSize));
        experiments = inAndOutOfBag.first;
    }

    if (m_parameters->verbose)
        vlcMessage.Begin("Constructing decision tree");

    constructDecisionTree(experiments);

    m_output->addHeadDecisionTreeNode(m_decisionTreeHead);
    m_decisionTreeHead->clearExperimentsWithinTree();

    if (m_parameters->verbose)
        vlcMessage.End();
}

void RandomForestEstimator::constructDecisionTree(vector<shared_ptr<DecisionTreeExperiment> >& experiments)
{
    vector<shared_ptr<DecisionTreeNode> > currentGeneration;
    vector<shared_ptr<DecisionTreeNode> > nextGeneration;

    // create a head DecisionTreeNode
    double sumZ = 0.0, sumW = 0.0;
    BOOST_FOREACH(auto& e, experiments)
    {
        double w = e->getWeight();
        sumW += w;
        sumZ += w * e->getZ();
    }

    m_decisionTreeHead = shared_ptr<DecisionTreeNode>(new DecisionTreeNode(experiments, sumZ, sumW, ROOT, shared_ptr<SplitDefinition>()));
    currentGeneration.push_back(m_decisionTreeHead);
    
    NodeSplitter splitter(m_data, m_parameters->minObservations, m_parameters->scale);

    while (!currentGeneration.empty())
    {
        BOOST_FOREACH(auto& nodeToSplit, currentGeneration)
        {
            if (nodeToSplit->getSumW() == 0)
                continue;

            // choose M variables to test splitting on
            // find terminal node with best improvement for any of those variables
            pair<vector<int>,vector<int> > inAndOut = MLUtils::bagObjectsWithoutReplacement<int>(m_featureIndices, min(m_featureIndices.size(), m_parameters->tryMVariables));
            vector<int> featuresToConsider = inAndOut.first;

            double bestImprovement = 0.0;
            shared_ptr<SplitDefinition> bestSplit;

            vector<shared_ptr<DecisionTreeNode> > children = splitter.splitNode(nodeToSplit, featuresToConsider);
            BOOST_FOREACH(auto& child, children)
            {
                nextGeneration.push_back(child);
            }
        }
        currentGeneration = nextGeneration;
        nextGeneration.clear();
    }
    

}

void RandomForestEstimator::initializeEstimator()
{
    m_missingValueDefined = m_data->missingValueDefined();
    if (m_missingValueDefined)
        m_missingValue        = m_data->getMissingValue();
        

    constructFeatureIndices();
    // sortTrainingExperiments();
}

void RandomForestEstimator::constructFeatureIndices()
{
    BOOST_FOREACH(auto& feature, m_parameters->featuresToRun)
    {
        // note that in a given run, we may not "run" with all loaded variables.
        m_featureIndices.push_back(m_data->getFeatureIndex(feature));
    }
}
