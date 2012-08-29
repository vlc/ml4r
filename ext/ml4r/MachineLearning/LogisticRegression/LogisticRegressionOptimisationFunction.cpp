#include "MachineLearning/LogisticRegression/LogisticRegressionOptimisationFunction.h"


LogisticRegressionOptimisationFunction::LogisticRegressionOptimisationFunction( vector<shared_ptr<MLExperiment>> trainingExperiments, vector<int> featureIndices )
    : m_trainingExperiments(trainingExperiments), m_featureIndices(featureIndices)
{

}

LogisticRegressionOptimisationFunction::~LogisticRegressionOptimisationFunction()
{

}

void LogisticRegressionOptimisationFunction::updateUsingParameters( vector<double> parameters )
{
    
    vector<double> predictions;
    predictions.reserve(m_trainingExperiments.size());
    boost::math::log
    BOOST_FOREACH(auto& experiment, m_trainingExperiments)
    {

    }
}

double LogisticRegressionOptimisationFunction::getCost()
{

}

vector<double> LogisticRegressionOptimisationFunction::getFirstOrderDerivatives()
{

}
