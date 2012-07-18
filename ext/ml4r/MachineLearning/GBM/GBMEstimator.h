#ifndef GBMEstimator_h__
#define GBMEstimator_h__

#include "MachineLearning/DecisionTree/FeatureInteraction.h"
#include "MachineLearning/MLEstimator.h"

#include <boost/shared_ptr.hpp>
#include <vector>
#include <map>
#include <set>
#include <utility>
using std::pair;
using std::set;
using std::map;
using std::vector;
using boost::shared_ptr;


class GBMParameters;
class GBMOutput;
class DecisionTreeExperiment;
class DecisionTreeNode;
class SplitDefinition;
class MLData;
class GBMCalculator;
class MLExperiment;

class GBMEstimator : public MLEstimator
{
public:
	GBMEstimator(MLData* data, vector<shared_ptr<MLExperiment> > experiments, shared_ptr<GBMParameters> parameters);
	~GBMEstimator();
    
    shared_ptr<MLOutput> estimate();
    void estimateMore(int numTrees);
    vector<FeatureInteraction> findInteractions(int howMany);
protected:
    void initializeEstimator();
    void performIteration();
    void constructFeatureIndices();
    void initialiseGBMExperimentData();
    void populateInitialF();
    void updateZ(vector<shared_ptr<DecisionTreeExperiment> >& experiments);
    void updatePredictions(vector<shared_ptr<DecisionTreeExperiment> >& experiments);

    void sortTrainingExperiments();

    void constructDecisionTree(vector<shared_ptr<DecisionTreeExperiment> >& experiments);
    void constructGenerousDecisionTree(vector<shared_ptr<DecisionTreeExperiment> >& experiments, int rfToLevel);
    
    

    // map<int, vector<shared_ptr<DecisionTreeExperiment> > > partitionSortedExperiments(shared_ptr<SplitDefinition> splitDefinition, Partition partition);
    map<int, vector<shared_ptr<DecisionTreeExperiment> > > bagSortedExperiments(vector<shared_ptr<DecisionTreeExperiment> >& baggedExperiments);

    vector<int> getRandomFeatureList();

    void calculateFIncrementPerDecisionTreeNode();
    void applyFIncrementToInBagExperiments();
    pair<vector<shared_ptr<DecisionTreeExperiment> >, vector<shared_ptr<DecisionTreeExperiment> > > bagExperiments();
    void applyFIncrementToExperiments(vector<shared_ptr<DecisionTreeExperiment> >& outOfBagExperiments);
    void reportDeviance(vector<shared_ptr<DecisionTreeExperiment> >& experiments);
    void deleteRedundantData();


    shared_ptr<GBMOutput>                                 m_output;
	vector<shared_ptr<DecisionTreeExperiment> >           m_decisionTreeExperiments;
    shared_ptr<GBMParameters>                             m_parameters;
    vector<int>                                           m_featureIndices;
    shared_ptr<DecisionTreeNode>                          m_decisionTreeHead;
    set<shared_ptr<DecisionTreeNode> >                    m_terminalNodes;
    map<shared_ptr<DecisionTreeNode>, double>             m_FIncrements;  
    shared_ptr<GBMCalculator>                             m_gbmCalculator;
    bool                                                  m_missingValueDefined;
    double                                                m_missingValue;
private:
};

#endif // GBMEstimator_h__
