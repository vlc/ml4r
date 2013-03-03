#include "MachineLearning/LogisticRegression/LogisticRegressionOptimisationFunction.h"
#include "MachineLearning/MLData/MLDataUtils.h"
#include <boost/foreach.hpp>
#include "utils/VlcMessage.h"
#include "utils/Utils.h"

#include <string>
using std::string;

#include <boost/lexical_cast.hpp>
using boost::lexical_cast;
using namespace std;

LogisticRegressionOptimisationFunction::LogisticRegressionOptimisationFunction( vector<shared_ptr<MLExperiment> > trainingExperiments, vector<int> featureIndices, double regularisationParameter )
    : m_trainingExperiments(trainingExperiments), m_featureIndices(featureIndices),
      m_regularisationParameter(regularisationParameter)
{
    m_trainingDataMatrix     = MLDataUtils::makeMlDataFeatureMatrix(m_trainingExperiments, m_featureIndices);
    m_trainingResponseVector = MLDataUtils::makeMlDataResponseVector(m_trainingExperiments);   

    // vlcMessage.Write(lexical_cast<string>(m_trainingDataMatrix));
    // vlcMessage.Write(lexical_cast<string>(m_trainingResponseVector));
}

LogisticRegressionOptimisationFunction::~LogisticRegressionOptimisationFunction()
{

}

void LogisticRegressionOptimisationFunction::updateUsingParameters( vector<double> parameters )
{
    VectorXd parametersVector(parameters.size());
    for (unsigned int i = 0; i < parameters.size(); ++i)
        parametersVector(i) = parameters.at(i);

    // vlcMessage.Write("Parameters: " + lexical_cast<string>(parametersVector));
    
    // probability = 1/(1+exp(-u))  u is calculated using data * parameters
    ArrayXd probabilities = (((m_trainingDataMatrix * parametersVector).array() * -1).exp() + 1).inverse();
    ArrayXd y             = m_trainingResponseVector.array();

    // cost function is (-logsum + regularisationTerm) / m
    // where logsum is calculated as sum(log( y * p + (1-y) * (1-p) ) )
    m_costFunction = -((y * probabilities) + ((1.0 - y) * (1.0 - probabilities))).log().sum();

    // vlcMessage.Write("Cost function: " + lexical_cast<string>(m_costFunction));

    // add regularisation term
    if (m_regularisationParameter)
        m_costFunction += m_regularisationParameter / 2.0 * parametersVector.dot(parametersVector);

    // calculate mean cost
    m_costFunction /= m_trainingExperiments.size();

    // derivative is (residuals * X) / m + regularisationParameter * parameters
    ArrayXd     residuals  = probabilities - y;
    ArrayXd     derivative = (residuals.matrix().transpose() * m_trainingDataMatrix).transpose().array();
    if (m_regularisationParameter)
        derivative += m_regularisationParameter * parametersVector.array();

    derivative /= m_trainingExperiments.size();

    // vlcMessage.Write("Derivatives: " + lexical_cast<string>(derivative));

    m_derivatives.resize(derivative.rows());
    for (unsigned int row = 0; row < m_derivatives.size(); ++row)
        m_derivatives.at(row) = derivative(row);

    // cout << "m_derivatives: " << m_derivatives << endl;
    // vlcMessage.Write("m_Derivatives: " + lexical_cast<string>(m_derivatives));
}

double LogisticRegressionOptimisationFunction::getCost()
{
    return m_costFunction;
}

vector<double> LogisticRegressionOptimisationFunction::getFirstOrderDerivatives()
{
    return m_derivatives;
}
