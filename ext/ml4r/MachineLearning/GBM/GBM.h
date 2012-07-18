#ifndef __GBM_h__
#define __GBM_h__

#include "MachineLearning/GBM/GBMParameters.h"

#include <boost/shared_ptr.hpp>
#include <vector>
using std::vector;
using boost::shared_ptr;

class MLData;
class DecisionTreeExperiment;
class MLExperiment;
class MLDataFields;
class DecisionTreeNode;
class SplitDefinition;
class GBMOutput;
class GBMCalculator;

class GBM
{
public:
    GBM();
    ~GBM();
    void estimate();
    void estimateMore(int numTrees);

    void setData(MLData* mlData);
    vector<double> getPredictions(MLData* newData);
    vector<double> getPredictions(vector<shared_ptr<DecisionTreeExperiment> >& experiments);

    vector<double> getMeanTrainingPredictions();
    vector<double> getCrossValidationPredictions();

    GBMParameters parameters;
protected:
    MLData* m_data;
    vector<shared_ptr<GBMOutput> > m_outputObjects; 
    shared_ptr<GBMCalculator> m_gbmCalculator;

    void config();
    void input();
    void goNuts();
    void output();
    vector<shared_ptr<DecisionTreeExperiment> > makeDecisionTreeExperiments(vector<shared_ptr<MLExperiment> >& experiments);
    

};

#endif // GBM_h__