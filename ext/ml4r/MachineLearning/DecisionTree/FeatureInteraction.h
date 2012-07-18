#ifndef FeatureInteraction_h__
#define FeatureInteraction_h__

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include "DecisionTreeNode.h"

class SplitDefinition;

class FeatureInteraction
{
public:
	FeatureInteraction(shared_ptr<SplitDefinition> primarySplitDefinition,shared_ptr<SplitDefinition> secondarySplitDefinition,Partition primaryPartition)
        : primarySplitDefinition(primarySplitDefinition), secondarySplitDefinition(secondarySplitDefinition),
          primaryPartition(primaryPartition)
    {

    }
    ~FeatureInteraction() {};

    shared_ptr<SplitDefinition> primarySplitDefinition;
    shared_ptr<SplitDefinition> secondarySplitDefinition;
    Partition primaryPartition;


protected:
	
private:
};
#endif // FeatureInteraction_h__