#include "MachineLearning/DecisionTree/DecisionTreeExperiment.h"

DecisionTreeExperiment::DecisionTreeExperiment()
    : MLExperiment()
{

}

DecisionTreeExperiment::DecisionTreeExperiment(shared_ptr<MLExperiment> mlExperiment)
    : MLExperiment(mlExperiment)
{

}



DecisionTreeExperiment::~DecisionTreeExperiment()
{

}

void DecisionTreeExperiment::setF(double f)
{
    m_F = f;
}

void DecisionTreeExperiment::setZ(double z)
{
    m_Z = z;
}

double DecisionTreeExperiment::getF()
{
    return m_F;
}

double DecisionTreeExperiment::getZ()
{
    return m_Z;
}

void DecisionTreeExperiment::incrementF(double increment)
{
    m_F += increment;
}

double DecisionTreeExperiment::getY()
{
    return m_yValue;
}
