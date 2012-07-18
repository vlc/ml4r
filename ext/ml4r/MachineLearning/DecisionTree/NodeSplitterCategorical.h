#ifndef __NodeSplitterCategorical_h__
#define __NodeSplitterCategorical_h__

#include "MachineLearning/DecisionTree/NodeSplitter.h"

class NodeSplitterCategorical : public NodeSplitter
{
public:
    NodeSplitterCategorical(MLData* data, int minObservations, double scale);
    ~NodeSplitterCategorical();

    shared_ptr<SplitDefinition> createSplitDefinition(shared_ptr<DecisionTreeNode> node, int featureIndex);
protected:
    
};

#endif // NodeSplitterCategorical_h__