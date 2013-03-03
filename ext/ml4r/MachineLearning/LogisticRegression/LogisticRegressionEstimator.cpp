#include "MachineLearning/LogisticRegression/LogisticRegressionEstimator.h"
#include "MachineLearning/LogisticRegression/LogisticRegressionParameters.h"
#include "MachineLearning/LogisticRegression/LogisticRegressionOutput.h"
#include "MachineLearning/LogisticRegression/LogisticRegressionOptimisationFunction.h"
#include "MachineLearning/Optimisation/FunctionOptimiser.h"
#include "MachineLearning/MLData/MLData.h"
#include "MachineLearning/MLData/MLDataUtils.h"
#include "MachineLearning/MLExperiment.h"
#include "utils/Utils.h"
#include "utils/MathUtils.h"
#include "utils/VlcMessage.h"
#include <boost/foreach.hpp>


LogisticRegressionEstimator::LogisticRegressionEstimator( MLData* data, vector<shared_ptr<MLExperiment> > experiments, shared_ptr<LogisticRegressionParameters> parameters )
    : MLEstimator(data, experiments), m_parameters(parameters)
{

}

LogisticRegressionEstimator::~LogisticRegressionEstimator()
{

}

void LogisticRegressionEstimator::determineValidFeatures()
{
    vector<int> featureIndices = m_data->getFeatureIndices(m_parameters->featuresToRun);
    
    if (m_parameters->lambda > 0)
    {
        // regularised!  No need to remove dodgy parameters! WOO!
        m_validFeatureIndices        = featureIndices;
        m_seedValuesForValidFeatures = m_parameters->seedParameterEstimates;
        return;
    }

    map<int, bool> removeFeature;
    vector<vector<double> > featureVectors;

    BOOST_FOREACH(int featureIndex, featureIndices)
        featureVectors.push_back(getFeatureVector(featureIndex));

    // NOTE! This method deliberately modifies the data in place, as it helps with the final check.
    vector<int> parametersToRemove = MathUtils::identifyLinearlyDependentMatrixRows(featureVectors);

    BOOST_FOREACH(int parameter, parametersToRemove)
        removeFeature[featureIndices.at(parameter)] = true;

    BOOST_FOREACH(int featureIndex, featureIndices)
    {
        if (removeFeature[featureIndex])
            continue;

        vector<double> activeFeatureValues   = getFeatureValuesGivenY(featureIndex, 1.0);
        vector<double> inactiveFeatureValues = getFeatureValuesGivenY(featureIndex, 0.0);

        if (Utils::vectorMax(activeFeatureValues) < Utils::vectorMin(inactiveFeatureValues) ||
            Utils::vectorMin(activeFeatureValues) > Utils::vectorMax(inactiveFeatureValues) )
            removeFeature[featureIndex] = true;
    }
    m_validFeatureIndices.clear();
    m_seedValuesForValidFeatures.clear();

    for (unsigned int i = 0; i < featureIndices.size(); ++i)
    {
        if (removeFeature[featureIndices.at(i)]) continue;

        m_validFeatureIndices.push_back(featureIndices.at(i));
        m_seedValuesForValidFeatures.push_back(m_parameters->seedParameterEstimates.at(i));
    }
}

vector<double> LogisticRegressionEstimator::getFeatureVector( int featureIndex )
{
    vector<double> featureVector(m_trainingExperiments.size());
    for (unsigned int i = 0; i < m_trainingExperiments.size(); ++i)
        featureVector.at(i) = m_trainingExperiments.at(i)->getFeatureValue(featureIndex);

    return featureVector;
}

vector<double> LogisticRegressionEstimator::getFeatureValuesGivenY( int featureIndex, double y )
{
    vector<double> featureValues;
    BOOST_FOREACH(shared_ptr<MLExperiment> experiment, m_trainingExperiments)
    {
        if (experiment->getY() == y)
            featureValues.push_back(experiment->getFeatureValue(featureIndex));
    }
    return featureValues;
}

shared_ptr<MLOutput> LogisticRegressionEstimator::estimate()
{
    determineValidFeatures();

    FunctionOptimiser* functionOptimiser = m_parameters->functionOptimiser;
    
    functionOptimiser->setFunction(shared_ptr<OptimisationFunction>(new LogisticRegressionOptimisationFunction(m_trainingExperiments, m_validFeatureIndices, m_parameters->lambda)));
    functionOptimiser->setSeedParameterEstimates(m_seedValuesForValidFeatures);
    functionOptimiser->optimise();
    vector<double> finalParameters = functionOptimiser->getFinalParameterEstimates();
    vector<int> experimentIndicies;
    experimentIndicies.reserve(m_trainingExperiments.size());
    BOOST_FOREACH(shared_ptr<MLExperiment> experiment, m_trainingExperiments)
        experimentIndicies.push_back(experiment->getExperimentIndex());

    shared_ptr<LogisticRegressionOutput> output = shared_ptr<LogisticRegressionOutput>(new LogisticRegressionOutput(m_data, experimentIndicies, m_parameters));
    
    output->setParameters(m_validFeatureIndices, finalParameters);
    return output;
}

