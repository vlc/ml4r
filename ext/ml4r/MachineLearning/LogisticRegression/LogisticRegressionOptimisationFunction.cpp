#include "MachineLearning/LogisticRegression/LogisticRegressionOptimisationFunction.h"
#include "MachineLearning/MLData/MLDataUtils.h"
#include <boost/foreach.hpp>


LogisticRegressionOptimisationFunction::LogisticRegressionOptimisationFunction( vector<shared_ptr<MLExperiment> > trainingExperiments, vector<int> featureIndices, double regularisationParameter )
    : m_trainingExperiments(trainingExperiments), m_featureIndices(featureIndices),
      m_regularisationParameter(regularisationParameter)
{
    m_trainingDataMatrix     = MLDataUtils::makeMlDataFeatureMatrix(m_trainingExperiments, m_featureIndices);
    m_trainingResponseVector = MLDataUtils::makeMlDataResponseVector(m_trainingExperiments);   
}

LogisticRegressionOptimisationFunction::~LogisticRegressionOptimisationFunction()
{

}

void LogisticRegressionOptimisationFunction::updateUsingParameters( vector<double> parameters )
{
    VectorXd parametersVector(parameters.size());
    for (int i = 0; i < parameters.size(); ++i)
        parametersVector(i) = parameters.at(i);
    
    // probability = 1/(1+exp(-u))  u is calculated using data * parameters
    ArrayXd probabilities = (((m_trainingDataMatrix * parametersVector).array() * -1).exp() + 1).inverse();
    ArrayXd y             = m_trainingResponseVector.array();

    // cost function is (-logsum + regularisationTerm) / m
    // where logsum is calculated as sum(log( y * p + (1-y) * (1-p) ) )
    m_costFunction = -((y * probabilities) + ((1.0 - y) * (1.0 - probabilities))).log().sum();

    // add regularisation term
    if (m_regularisationParameter)
        m_costFunction += m_regularisationParameter / 2.0 * parametersVector.dot(parametersVector);

    // calculate mean cost
    m_costFunction /= m_trainingExperiments.size();

    // derivative is (residuals * X) / m + regularisationParameter * parameters
    ArrayXd     residuals  = probabilities - y;
    ArrayXd     derivative = (residuals.matrix() * m_trainingDataMatrix).array();
    if (m_regularisationParameter)
        derivative += m_regularisationParameter * parametersVector.array();

    derivative /= m_trainingExperiments.size();

    m_derivatives.resize(derivative.cols());
    for (int col = 0; col < m_derivatives.size(); ++col)
        m_derivatives.at(col) = derivative(col);
}

double LogisticRegressionOptimisationFunction::getCost()
{
    return m_costFunction;
}

vector<double> LogisticRegressionOptimisationFunction::getFirstOrderDerivatives()
{
    return m_derivatives;
}
