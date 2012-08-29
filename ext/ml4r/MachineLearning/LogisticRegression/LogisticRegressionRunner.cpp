#include "MachineLearning/LogisticRegression/LogisticRegressionRunner.h"
#include "MachineLearning/LogisticRegression/LogisticRegressionParameters.h"
#include "MachineLearning/LogisticRegression/LogisticRegressionEstimator.h"
#include <boost/make_shared.hpp>
using boost::make_shared;
#include <boost/shared_ptr.hpp>
using boost::shared_ptr;


LogisticRegressionRunner::LogisticRegressionRunner()
{
    parameters = make_shared<LogisticRegressionParameters>();
}

LogisticRegressionRunner::~LogisticRegressionRunner()
{

}

shared_ptr<MLEstimator> LogisticRegressionRunner::createEstimator( MLData* data, vector<shared_ptr<MLExperiment> > trainingExperiments )
{
    return shared_ptr<MLEstimator>(shared_ptr<LogisticRegressionEstimator>(new LogisticRegressionEstimator(data, trainingExperiments, parameters)));
}
