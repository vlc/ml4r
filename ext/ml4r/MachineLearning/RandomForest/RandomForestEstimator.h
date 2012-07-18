#ifndef RandomForestEstimator_h__
#define RandomForestEstimator_h__

#include "MachineLearning/RandomForest/RandomForestParameters.h"
#include "MachineLearning/MLEstimator.h"

#include <boost/shared_ptr.hpp>
#include <vector>
using std::vector;
using boost::shared_ptr;

class RandomForestOutput;
class DecisionTreeExperiment;
class MLData;
class DecisionTreeNode;

class RandomForestEstimator : public MLEstimator
{
public:
	RandomForestEstimator(MLData* data, vector<shared_ptr<MLExperiment> > experiments, shared_ptr<RandomForestParameters> parameters);
	~RandomForestEstimator();

    shared_ptr<MLOutput> estimate();
    shared_ptr<MLOutput> estimateMore(int numTrees);

protected:
    void    updateZ();
    void    performIteration();
    void    initializeEstimator();
    void    constructFeatureIndices();
    void    constructDecisionTree(vector<shared_ptr<DecisionTreeExperiment> >& experiments);

	MLData*                                     m_data;
    vector<shared_ptr<DecisionTreeExperiment> >  m_decisionTreeExperiments;
    shared_ptr<RandomForestParameters>          m_parameters;

    shared_ptr<RandomForestOutput>              m_output;
    shared_ptr<DecisionTreeNode>                m_decisionTreeHead;

    bool                                        m_missingValueDefined;
    double                                      m_missingValue;

    vector<int>                                 m_featureIndices;
private:
};

#endif // RandomForestEstimator_h__
