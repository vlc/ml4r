#include "MachineLearning/DecisionTree/NodeSplitterContinuous.h"
#include "MachineLearning/DecisionTree/DecisionTreeExperiment.h"
#include "MachineLearning/DecisionTree/SplitDefinition.h"
#include "MachineLearning/DecisionTree/DecisionTreeNode.h"



NodeSplitterContinuous::NodeSplitterContinuous(MLData* data, int minObservations, double scale)
    : NodeSplitter(data, minObservations, scale)
{
}

NodeSplitterContinuous::~NodeSplitterContinuous()
{

}

shared_ptr<SplitDefinition> NodeSplitterContinuous::createSplitDefinition(shared_ptr<DecisionTreeNode> node, int featureIndex)
{
    
}
