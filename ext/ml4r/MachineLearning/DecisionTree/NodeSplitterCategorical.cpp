#include "MachineLearning/DecisionTree/NodeSplitterCategorical.h"
#include "MachineLearning/DecisionTree/DecisionTreeNode.h"
#include "MachineLearning/DecisionTree/DecisionTreeExperiment.h"
#include "MachineLearning/DecisionTree/SplitDefinition.h"
#include "MachineLearning/DecisionTree/CategoryInfo.h"

NodeSplitterCategorical::NodeSplitterCategorical(MLData* data, int minObservations, double scale)
    : NodeSplitter(data, minObservations, scale)
{

}

NodeSplitterCategorical::~NodeSplitterCategorical()
{

}

shared_ptr<SplitDefinition> NodeSplitterCategorical::createSplitDefinition(shared_ptr<DecisionTreeNode> node, 
                                                                           int featureIndex)
{
    
}