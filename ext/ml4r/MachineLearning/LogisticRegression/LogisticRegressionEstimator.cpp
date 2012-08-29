#include "MachineLearning/LogisticRegression/LogisticRegressionEstimator.h"
#include "MachineLearning/LogisticRegression/LogisticRegressionParameters.h"
#include "MachineLearning/LogisticRegression/LogisticRegressionOutput.h"
#include "MachineLearning/LogisticRegression/LogisticRegressionOptimisationFunction.h"
#include "MachineLearning/Optimisation/FunctionOptimiser.h"
#include "MachineLearning/MLData/MLData.h"
#include "MachineLearning/MLData/MLDataUtils.h"
#include "MachineLearning/MLExperiment.h"

LogisticRegressionEstimator::LogisticRegressionEstimator( MLData* data, vector<shared_ptr<MLExperiment>> experiments, shared_ptr<LogisticRegressionParameters> parameters )
    : MLEstimator(data, experiments), m_parameters(parameters)
{

}

LogisticRegressionEstimator::~LogisticRegressionEstimator()
{

}

shared_ptr<MLOutput> LogisticRegressionEstimator::estimate()
{
    vector<int> featureIndicesToRun;
    BOOST_FOREACH(auto& feature, m_parameters->featuresToRun)
        featureIndicesToRun.push_back(m_data->getFeatureIndex(feature));

    vector<int> featuresIndicesToEstimate = MLDataUtils::findValidFeaturesForRegression(m_trainingExperiments, featureIndicesToRun);

    vector<double> userSeedParameterEstimates = m_parameters->seedParameterEstimates;
    if (userSeedParameterEstimates.size() < m_parameters->featuresToRun.size())
        userSeedParameterEstimates.resize(m_parameters->featuresToRun.size());

    vector<double> seedParameterEstimates;

    if (featuresIndicesToEstimate == featureIndicesToRun)
        seedParameterEstimates = userSeedParameterEstimates;
    else
    {
        BOOST_FOREACH(auto& featureIndex, featuresIndicesToEstimate)
            seedParameterEstimates.push_back(userSeedParameterEstimates.at(OTUtils::vectorIndex(featureIndicesToRun, featureIndex)));
    }

        
    // starting point parameters
    // remove variables as required
    FunctionOptimiser* functionOptimiser = m_parameters->functionOptimiser;
    
    functionOptimiser->setFunction(shared_ptr<OptimisationFunction>(new LogisticRegressionOptimisationFunction()));
    functionOptimiser->setSeedParameterEstimates(seedParameterEstimates);

    functionOptimiser->optimise();

    vector<double> finalParameters = functionOptimiser->getFinalParameterEstimates();

    vector<int> experimentIndicies;
    experimentIndicies.reserve(m_trainingExperiments.size());
    BOOST_FOREACH(auto& experiment, m_trainingExperiments)
        experimentIndicies.push_back(experiment->getExperimentIndex());

    shared_ptr<LogisticRegressionOutput> output = shared_ptr<LogisticRegressionOutput>(new LogisticRegressionOutput(m_data, experimentIndicies, m_parameters));
    output->setParameters(featuresIndicesToEstimate, finalParameters);
}

