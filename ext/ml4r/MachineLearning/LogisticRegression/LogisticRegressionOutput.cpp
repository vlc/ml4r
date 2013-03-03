#include "MachineLearning/LogisticRegression/LogisticRegressionOutput.h"
#include "MachineLearning/MLExperiment.h"
// #include <math.h>
#include <cmath>

LogisticRegressionOutput::LogisticRegressionOutput( MLData* data, vector<int> trainingExperimentIndicies, shared_ptr<LogisticRegressionParameters> parameters )
    : MLOutput(data, trainingExperimentIndicies), m_parameters(parameters)
{

}

LogisticRegressionOutput::~LogisticRegressionOutput()
{

}

double LogisticRegressionOutput::calculateAveragePredictions( vector<double> predictions )
{
    double sumUtility = 0.0;
    BOOST_FOREACH(double probability, predictions)
        sumUtility += log(probability / (1.0 - probability));

    double meanUtility = sumUtility / predictions.size();
    return 1.0 / (1.0 + exp(-meanUtility));
}

double LogisticRegressionOutput::predictForExperiment( shared_ptr<MLExperiment> experiment )
{
    double utility = 0.0;
    double parameterIndex = -1;
    BOOST_FOREACH(int featureIndex, m_featureIndices)
    {
        ++parameterIndex;
        utility += m_parameterEstimates.at(parameterIndex) * experiment->getFeatureValue(featureIndex);
    }
    return 1.0 / (1.0 + exp(-utility));
}

void LogisticRegressionOutput::setParameters( vector<int>& featureIndices, vector<double>& parameterEstimates )
{
    m_featureIndices     = featureIndices;
    m_parameterEstimates = parameterEstimates;
}
