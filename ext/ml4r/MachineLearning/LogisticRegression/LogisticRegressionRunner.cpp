#include "MachineLearning/LogisticRegression/LogisticRegressionRunner.h"


LogisticRegressionRunner::LogisticRegressionRunner()
{
    parameters = make_shared<LogisticRegressionParameters>();
}

LogisticRegressionRunner::~LogisticRegressionRunner()
{

}

shared_ptr<MLEstimator> LogisticRegressionRunner::createEstimator( MLData* data, vector<shared_ptr<MLExperiment>> trainingExperiments )
{
    return shared_ptr<MLEstimator>(shared_ptr<LogisticRegressionEstimator>(new LogisticRegressionEstimator(data, trainingExperiments, parameters)));
}
