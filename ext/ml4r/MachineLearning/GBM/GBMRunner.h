#ifndef __GBM_h__
#define __GBM_h__

#include "MachineLearning/MLRunner.h"

#include <boost/shared_ptr.hpp>
#include <vector>
using std::vector;
using boost::shared_ptr;

class MLData;
class MLExperiment;
class FeatureInteraction;
class GBMParameters;

class GBMRunner : public MLRunner
{
public:
    GBMRunner();
    ~GBMRunner();

    void estimateMore(int numTrees);
    void capTrees(int numTrees);

    vector<FeatureInteraction> getFeatureInteractions(int howMany);
    
    shared_ptr<MLEstimator> createEstimator(MLData* data, vector<shared_ptr<MLExperiment> > trainingExperiments);

    shared_ptr<GBMParameters> parameters;

protected:
    void config();
};

#endif // GBM_h__