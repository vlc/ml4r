#include "MachineLearning/GBM/GaussianCalculator.h"
#include "MachineLearning/DecisionTree/DecisionTreeExperiment.h"

#include <boost/foreach.hpp>

GaussianCalculator::GaussianCalculator()
{

}

GaussianCalculator::~GaussianCalculator()
{

}

double GaussianCalculator::calculateDeviance(vector<shared_ptr<DecisionTreeExperiment> >& experiments)
{
    double sumSquaredErrors = 0.0;
    double sumWeight        = 0.0;
    BOOST_FOREACH(shared_ptr<DecisionTreeExperiment>& experiment, experiments)
    {
        double error = experiment->getY() - experiment->getPrediction();
        sumSquaredErrors += experiment->getWeight() * error * error;
        sumWeight        += experiment->getWeight();
    }
    return sumSquaredErrors / sumWeight;
}

void GaussianCalculator::populateInitialF(vector<shared_ptr<DecisionTreeExperiment> >& experiments, bool useInitialPredictions)
{
    if (!useInitialPredictions)
    {
        // compute mean
        double sumY = 0.0;
        double sumW = 0.0;
        BOOST_FOREACH(shared_ptr<DecisionTreeExperiment>& experiment, experiments)
        {
            sumY += experiment->getWeight() * experiment->getY();
            sumW += experiment->getWeight();
        }
        double meanY = sumY / sumW;

        BOOST_FOREACH(shared_ptr<DecisionTreeExperiment>& experiment, experiments)
        {
            experiment->setPrediction(meanY);
        }
    }
    BOOST_FOREACH(shared_ptr<DecisionTreeExperiment>& experiment, experiments)
    {
        experiment->setF(calculateF(experiment->getPrediction()));
    }
}

void GaussianCalculator::updateZ(vector<shared_ptr<DecisionTreeExperiment> >& experiments)
{
    BOOST_FOREACH(shared_ptr<DecisionTreeExperiment>& experiment, experiments)
    {
        double z = experiment->getY() - experiment->getPrediction();
        experiment->setZ(z);
    }
}

double GaussianCalculator::computeFIncrement(vector<shared_ptr<DecisionTreeExperiment> >& experiments)
{
    double sumZ = 0.0;
    double sumW = 0.0;
    BOOST_FOREACH(shared_ptr<DecisionTreeExperiment>& experiment, experiments)
    {
        sumZ += experiment->getWeight() * experiment->getZ();
        sumW += experiment->getWeight();
    }
    if (sumW == 0)
        return 0.0;

    return sumZ / sumW;
}

void GaussianCalculator::updatePredictions(vector<shared_ptr<DecisionTreeExperiment> >& experiments)
{
    BOOST_FOREACH(shared_ptr<DecisionTreeExperiment>& experiment, experiments)
    {
        experiment->setPrediction(calculatePrediction(experiment->getF()));
    }
}

double GaussianCalculator::calculatePrediction(double f)
{
    return f;
}

double GaussianCalculator::calculateF(double prediction)
{
    return prediction;
}
