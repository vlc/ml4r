#ifndef __SplitDefinition_h__
#define __SplitDefinition_h__

#include <boost/shared_ptr.hpp>
#include <set>
using std::set;
using boost::shared_ptr;

class DecisionTreeExperiment;
class DecisionTreeNode;

class SplitDefinition
{
public:
    SplitDefinition(shared_ptr<DecisionTreeNode> nodeToSplit, 
                    int       featureIndex, 
                    set<double>& lhsCategories, 
                    set<double>& rhsCategories,
                    double    lhsSumZ,
                    double    lhsSumW,
                    int       lhsCount,
                    double    rhsSumZ,
                    double    rhsSumW,
                    int       rhsCount,
                    double    missingSumZ,
                    double    missingSumW,
                    int       missingCount,
                    double    improvement);

    SplitDefinition(shared_ptr<DecisionTreeNode> nodeToSplit, 
                    int    featureIndex,
                    double splitValue, 
                    double    lhsSumZ,
                    double    lhsSumW,
                    int       lhsCount,
                    double    rhsSumZ,
                    double    rhsSumW,
                    int       rhsCount,
                    double    missingSumZ,
                    double    missingSumW,
                    int       missingCount,
                    double improvement);
    
    ~SplitDefinition();

    int                                 getFeatureIndex();
    double                              getImprovement();
    shared_ptr<DecisionTreeNode>        getNodeToSplit();
    double                              getLhsSumZ();
    double                              getLhsSumW();
    int                                 getLhsExperimentCount();
    double                              getRhsSumZ();
    double                              getRhsSumW();
    int                                 getRhsExperimentCount();
    double                              getMissingSumZ();
    double                              getMissingSumW();
    int                                 getMissingExperimentCount();
    set<double>&                        getLhsCategories();
    set<double>&                        getRhsCategories();
    double                              getSplitValue();
    bool                                isCategorical();
protected:
    shared_ptr<DecisionTreeNode> m_nodeToSplit;
    int         m_splitFeatureIndex;
    set<double> m_lhsCategories;
    set<double> m_rhsCategories;
    double      m_splitValue;
    double      m_lhsSumZ;
    double      m_rhsSumZ;
    double      m_missingSumZ;
    double      m_lhsSumW;
    double      m_rhsSumW;
    double      m_missingSumW;
    int         m_lhsCount;
    int         m_rhsCount;
    int         m_missingCount;
    double      m_improvement;
    bool        m_featureIsCategorical;
};

#endif // SplitDefinition_h__