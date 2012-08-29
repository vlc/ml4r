#ifndef LogisticRegressionRunner_h__
#define LogisticRegressionRunner_h__

#include "MachineLearning/MLRunner.h"

#include <boost/shared_ptr.hpp>
#include <vector>
using std::vector;
using boost::shared_ptr;

class MLData;
class MLEstimator;
class MLExperiment;
class LogisticRegressionParameters;

class LogisticRegressionRunner : public MLRunner
{
public:
	LogisticRegressionRunner();
	~LogisticRegressionRunner();

    shared_ptr<MLEstimator> createEstimator(MLData* data, vector<shared_ptr<MLExperiment> > trainingExperiments);

    shared_ptr<LogisticRegressionParameters> parameters;
protected:
	
private:
};
#endif // LogisticRegressionRunner_h__