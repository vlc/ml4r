#ifndef RandomForest_h__
#define RandomForest_h__

#include "MachineLearning/MLRunner.h"

#include <boost/shared_ptr.hpp>
#include <vector>
using std::vector;
using boost::shared_ptr;

class MLData;
class MLExperiment;
class MLEstimator;
class RandomForestParameters;

class RandomForestRunner : public MLRunner
{
public:
	RandomForestRunner();
	~RandomForestRunner();

    void estimateMore(int numTrees);
    
    shared_ptr<RandomForestParameters> parameters;

    shared_ptr<MLEstimator> createEstimator(MLData* data, vector<shared_ptr<MLExperiment> > trainingExperiments);

protected:
    void config();
    
private:
};

#endif // RandomForest_h__
