#ifndef LogisticRegressionEstimator_h__
#define LogisticRegressionEstimator_h__

#include "MachineLearning/MLEstimator.h"

#include <boost/shared_ptr.hpp>
#include <vector>
using std::vector;
using boost::shared_ptr;

class MLData;
class MLExperiment;
class LogisticRegressionParameters;

class LogisticRegressionEstimator : public MLEstimator
{
public:
	LogisticRegressionEstimator(MLData* data, vector<shared_ptr<MLExperiment> > experiments, shared_ptr<LogisticRegressionParameters> parameters);
	~LogisticRegressionEstimator();

    shared_ptr<MLOutput> estimate();
protected:
	void determineValidFeatures();
    vector<double> getFeatureVector(int featureIndex);
    vector<double> getFeatureValuesGivenY(int featureIndex, double y);
    shared_ptr<LogisticRegressionParameters> m_parameters;

    vector<int>     m_validFeatureIndices;
    vector<double>  m_seedValuesForValidFeatures;
private:
};
#endif // LogisticRegressionEstimator_h__