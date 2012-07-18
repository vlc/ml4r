#ifndef __NodeSplitter_h__
#define __NodeSplitter_h__

#include "MachineLearning/DecisionTree/DecisionTreeNode.h"

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

class SplitDefinition;
class DecisionTreeNode;
class DecisionTreeExperiment;
class MLData;

// pure virtual base class for NodeSplitterContinuous and NodeSplitterCategorical

class NodeSplitter
{
public:
    NodeSplitter(MLData* data, int minObservations, double scale);
    ~NodeSplitter();

    shared_ptr<SplitDefinition> createSplitDefinition(shared_ptr<DecisionTreeNode> node, int featureIndex);
    shared_ptr<SplitDefinition> createContinuousSplitDefinition(shared_ptr<DecisionTreeNode> node, int featureIndex);
    shared_ptr<SplitDefinition> createCategoricalSplitDefinition(shared_ptr<DecisionTreeNode> node, int featureIndex);

    double calculateImprovement(double lhsSumW, double lhsSumZ, double rhsSumW, double rhsSumZ, double missingSumW, double missingSumZ);
    
    shared_ptr<DecisionTreeNode> createLhsChild(shared_ptr<SplitDefinition> splitDefinition);
    shared_ptr<DecisionTreeNode> createRhsChild(shared_ptr<SplitDefinition> splitDefinition);
    shared_ptr<DecisionTreeNode> createMissingChild(shared_ptr<SplitDefinition> splitDefinition);
    shared_ptr<DecisionTreeNode> createChild(shared_ptr<SplitDefinition> splitDefinition, Partition partition);

    vector<shared_ptr<DecisionTreeExperiment> > partitionExperiments(vector<shared_ptr<DecisionTreeExperiment> >& experiments, 
        shared_ptr<SplitDefinition> splitDefinition, Partition partition);

    vector<shared_ptr<DecisionTreeNode> > splitNode(shared_ptr<DecisionTreeNode> nodeToSplit, vector<int> featuresToConsider);
protected:
    MLData* m_data;
    double  m_missingValue;
    int     m_minObservations;
    bool    m_missingValueDefined;
    double  m_scale;
};

#endif // NodeSplitter_h__