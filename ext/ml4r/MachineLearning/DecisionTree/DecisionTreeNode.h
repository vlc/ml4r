#ifndef __DecisionTreeNode_h__
#define __DecisionTreeNode_h__

#include <vector>
#include <boost/shared_ptr.hpp>
#include <set>
#include <map>
using std::map;
using std::set;
using boost::shared_ptr;
using std::vector;

class DecisionTreeExperiment;
class SplitDefinition;

enum Partition
{
    ROOT,
    LHS,
    RHS,
    MISSING,
};

class DecisionTreeNode
{
public:
    DecisionTreeNode(   vector<shared_ptr<DecisionTreeExperiment> > experiments,
                        double sumZ,
                        double sumW,
                        Partition partition,
                        shared_ptr<SplitDefinition> parentSplitDefinition);
    ~DecisionTreeNode();

    

    shared_ptr<DecisionTreeNode> getTerminalNodeForExperiment(shared_ptr<DecisionTreeExperiment> experiment);
    
    void defineSplit(shared_ptr<SplitDefinition> splitDefinition,
                     shared_ptr<DecisionTreeNode> lhsChild, 
                     shared_ptr<DecisionTreeNode> rhsChild, 
                     shared_ptr<DecisionTreeNode> missingChild);
    
    vector<shared_ptr<DecisionTreeExperiment> > getExperiments();
  
    bool isTerminalNode();
    void clearExperimentsWithinTree();

    double getSumZ();
    double getSumW();
    void   setSumZ(double sumZ);
    void   setSumW(double sumW);
    void   updateSums();

    shared_ptr<SplitDefinition> getSplitDefinition();
    shared_ptr<SplitDefinition> getParentSplitDefinition();
    Partition                   getPartition();

    static void setMissingValue(double missingValue);
protected:
    shared_ptr<DecisionTreeNode> chooseChild(shared_ptr<DecisionTreeExperiment> experiment);
    void setChildren(shared_ptr<DecisionTreeNode> lhsChild, 
                     shared_ptr<DecisionTreeNode> rhsChild,
                     shared_ptr<DecisionTreeNode> missingChild);
    
    // if this decision tree node has been further split, the following variables will be populated, otherwise they will be null!
    bool                                    m_nodeHasChildren;
    
    shared_ptr<DecisionTreeNode>            m_lhsChild;
    shared_ptr<DecisionTreeNode>            m_rhsChild;
    shared_ptr<DecisionTreeNode>            m_missingChild;
    
    vector<shared_ptr<DecisionTreeExperiment> > m_experiments;

    static bool                             m_missingValueDefined;
    static double                           m_missingValue;

    double                                  m_sumZ;
    double                                  m_sumW;
    Partition                               m_whichPartitionAmI;
    shared_ptr<SplitDefinition>             m_parentSplitDefinition;
    shared_ptr<SplitDefinition>             m_splitDefinition;
};



#endif // DecisionTreeNode_h__