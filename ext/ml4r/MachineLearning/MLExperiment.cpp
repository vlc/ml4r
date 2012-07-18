#include "MachineLearning/MLExperiment.h"

MLExperiment::MLExperiment(int experimentId, int experimentIndex, double y, double initialPrediction,
                            double weight, vector<double> features)
 : m_experimentId(experimentId), m_experimentIndex(experimentIndex), m_yValue(y), m_prediction(initialPrediction),
   m_weight(weight), m_features(features)
{
    
}

MLExperiment::MLExperiment(shared_ptr<MLExperiment> mlExperiment)
  : m_yValue(mlExperiment->getY()), 
    m_experimentId(mlExperiment->getExperimentId()),
    m_experimentIndex(mlExperiment->getExperimentIndex()),
    m_prediction(mlExperiment->getPrediction()),
    m_weight(mlExperiment->getWeight()),
    m_features(mlExperiment->getFeatureValues())
{
}

MLExperiment::MLExperiment()
{

}

MLExperiment::~MLExperiment()
{

}

double MLExperiment::getY()
{
    return m_yValue;
}

int MLExperiment::getExperimentId()
{
    return m_experimentId;
}

int MLExperiment::getExperimentIndex()
{
    return m_experimentIndex;
}

double MLExperiment::getPrediction()
{
    return m_prediction;
}

double MLExperiment::getWeight()
{
    return m_weight;
}

vector<double>& MLExperiment::getFeatureValues()
{
    return m_features;
}

double MLExperiment::getFeatureValue(int featureIndex)
{
    return m_features.at(featureIndex);
}

void MLExperiment::setPrediction(double prediction)
{
    m_prediction = prediction;
}
