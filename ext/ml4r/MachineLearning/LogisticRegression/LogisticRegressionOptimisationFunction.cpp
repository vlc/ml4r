#include "MachineLearning/LogisticRegression/LogisticRegressionOptimisationFunction.h"
#include <boost/foreach.hpp>

LogisticRegressionOptimisationFunction::LogisticRegressionOptimisationFunction( vector<shared_ptr<MLExperiment> > trainingExperiments, vector<int> featureIndices )
    : m_trainingExperiments(trainingExperiments), m_featureIndices(featureIndices)
{

}

LogisticRegressionOptimisationFunction::~LogisticRegressionOptimisationFunction()
{

}

void LogisticRegressionOptimisationFunction::updateUsingParameters( vector<double> parameters )
{
    
    vector<double> predictions;
    
    
    
}

double LogisticRegressionOptimisationFunction::getCost()
{

}

vector<double> LogisticRegressionOptimisationFunction::getFirstOrderDerivatives()
{

}
