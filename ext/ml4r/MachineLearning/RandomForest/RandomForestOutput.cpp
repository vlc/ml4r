#include "MachineLearning/RandomForest/RandomForestOutput.h"
#include "MachineLearning/RandomForest/RandomForestParameters.h"
#include "MachineLearning/DecisionTree/DecisionTreeNode.h"
#include "MachineLearning/DecisionTree/DecisionTreeExperiment.h"

RandomForestOutput::RandomForestOutput( MLData* trainingData, vector<int> trainingExperimentIndicies, shared_ptr<RandomForestParameters> parameters )
    : MLOutput(trainingData, trainingExperimentIndicies), m_parameters(parameters)
{

}

RandomForestOutput::~RandomForestOutput()
{

}

shared_ptr<RandomForestParameters> RandomForestOutput::getParameters()
{
    return m_parameters;
}

double RandomForestOutput::predictForExperiment( shared_ptr<MLExperiment> experiment )
{
    shared_ptr<DecisionTreeExperiment> dtExperiment = make_shared<DecisionTreeExperiment>(experiment);
    
    double sumPrediction = 0.0;
    int count = 0;
    BOOST_FOREACH(auto& head, m_headNodes)
    {
        shared_ptr<DecisionTreeNode> node = head->getTerminalNodeForExperiment(dtExperiment);
        if (node.get() == 0)
            node = head;

        if (node->getSumW() == 0)
        {
            
            vlcMessage.Write("Zero weight!! WTF!!");
            vlcMessage.Write("SumZ: "+ ToString(node->getSumZ()));
            vlcMessage.Write("exp.size() " + ToString(node->getExperiments().size()));
            vlcMessage.Write("Node is head: " + ToString(node == head));
        }

        if (node->isTerminalNode())
        {
            sumPrediction += node->getSumZ() / node->getSumW();
            count++;
        }
        
    }
    return sumPrediction / count;
}

void RandomForestOutput::addHeadDecisionTreeNode( shared_ptr<DecisionTreeNode> node )
{
    m_headNodes.push_back(node);
}

int RandomForestOutput::getNumTrees()
{
    return m_headNodes.size();
}