#ifndef __NodeSplitterContinuous_h__
#define __NodeSplitterContinuous_h__

#include "MachineLearning/DecisionTree/NodeSplitter.h"

class NodeSplitterContinuous : public NodeSplitter
{
public:
    NodeSplitterContinuous(MLData* data, int minObservations, double scale);
    ~NodeSplitterContinuous();

    shared_ptr<SplitDefinition> createSplitDefinition(shared_ptr<DecisionTreeNode> node, int featureIndex);
protected:
};

#endif // NodeSplitterContinuous_h__