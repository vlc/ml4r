#ifndef LogisticRegressionOutput_h__
#define LogisticRegressionOutput_h__

#include "MachineLearning/MLOutput.h"

#include <boost/shared_ptr.hpp>
#include <vector>
using std::vector;
using boost::shared_ptr;

class MLData;
class LogisticRegressionParameters;
class MLExperiment;

class LogisticRegressionOutput : public MLOutput
{
public:
	LogisticRegressionOutput(MLData* data, vector<int> trainingExperimentIndicies, shared_ptr<LogisticRegressionParameters> parameters);
	~LogisticRegressionOutput();

    // compulsory
    double  calculateAveragePredictions(vector<double> predictions); 
    double  predictForExperiment(shared_ptr<MLExperiment> experiment);

    // custom to logistic regression
    void    setParameters(vector<int> featureIndices, vector<double> parameterEstimates);
protected:

	shared_ptr<LogisticRegressionParameters> m_parameters;
    vector<int>     featureIndices;
    vector<double>  parameterEstimates;

private:
};
#endif // LogisticRegressionOutput_h__