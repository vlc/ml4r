#include "MachineLearning/RandomForest/RandomForestOutput.h"
#include "MachineLearning/RandomForest/RandomForestParameters.h"
#include "MachineLearning/DecisionTree/DecisionTreeNode.h"
#include "MachineLearning/DecisionTree/DecisionTreeExperiment.h"

#include "utils/VlcMessage.h"

#include <boost/make_shared.hpp>
using boost::make_shared;

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
    BOOST_FOREACH(shared_ptr<DecisionTreeNode>& head, m_headNodes)
    {
        shared_ptr<DecisionTreeNode> node = head->getTerminalNodeForExperiment(dtExperiment);
        if (node.get() == 0)
            node = head;

        if (node->getSumW() == 0)
        {
            
            vlcMessage.Write("Zero weight!! WTF!!");
            vlcMessage.Write("SumZ: "+ boost::lexical_cast<string>(node->getSumZ()));
            vlcMessage.Write("exp.size() " + boost::lexical_cast<string>(node->getExperiments().size()));
            vlcMessage.Write("Node is head: " + boost::lexical_cast<string>(node == head));
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
    return (int) m_headNodes.size();
}