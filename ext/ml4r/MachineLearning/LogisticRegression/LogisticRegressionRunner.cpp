#include "MachineLearning/LogisticRegression/LogisticRegressionRunner.h"
#include "MachineLearning/LogisticRegression/LogisticRegressionParameters.h"
#include "MachineLearning/LogisticRegression/LogisticRegressionEstimator.h"
#include "MachineLearning/MLData/MLData.h"
#include "MachineLearning/Optimisation/FunctionOptimiser.h"
#include "MachineLearning/Optimisation/GradientDescentOptimiser.h"
#include "utils/Utils.h"
#include "utils/VlcMessage.h"

#include <boost/make_shared.hpp>
using boost::make_shared;
#include <boost/shared_ptr.hpp>
using boost::shared_ptr;
#include <boost/foreach.hpp>


LogisticRegressionRunner::LogisticRegressionRunner()
{
    parameters                      = make_shared<LogisticRegressionParameters>();
    m_defaultFunctionOptimiser      = new GradientDescentOptimiser();
    parameters->functionOptimiser   = m_defaultFunctionOptimiser;
}

LogisticRegressionRunner::~LogisticRegressionRunner()
{
    delete m_defaultFunctionOptimiser;
}

shared_ptr<MLEstimator> LogisticRegressionRunner::createEstimator( MLData* data, vector<shared_ptr<MLExperiment> > trainingExperiments )
{
    shared_ptr<LogisticRegressionEstimator> estimator = make_shared<LogisticRegressionEstimator>(data, trainingExperiments, parameters);
    return shared_ptr<MLEstimator>(estimator);
}

void LogisticRegressionRunner::config()
{
    vector<string> dataFeatures = m_data->getFeatureNames();

    // parameters->loadedFeatures = dataFeatures;
    if (parameters->featuresToRun.empty())
        parameters->featuresToRun = dataFeatures;
    else
    {    
        BOOST_FOREACH(string feature, parameters->featuresToRun)
        {
            if (!Utils::hasElement(dataFeatures, feature))
                throw std::runtime_error("Feature '" + feature + "' specified as part of parameter 'featuresToRun', but feature not found in data");
        }
    }
    if (parameters->featuresToRun.empty())
        throw std::runtime_error("There are no features to run!");

    vector<double>& seedParameterEstimates = parameters->seedParameterEstimates;
    if (seedParameterEstimates.size() == 0)
        seedParameterEstimates.resize(parameters->featuresToRun.size());
    else if (seedParameterEstimates.size() != parameters->featuresToRun.size())
        throw std::runtime_error("[LogisticRegressionRunner::config] - length of seedParameterEstimates does not match the featuresToRun");

}

void LogisticRegressionRunner::setFunctionOptimiser( FunctionOptimiser* optimiser )
{
    delete m_defaultFunctionOptimiser;
    m_defaultFunctionOptimiser = 0;

    parameters->functionOptimiser = optimiser;
}

