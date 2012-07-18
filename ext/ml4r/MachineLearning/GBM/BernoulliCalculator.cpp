#include "MachineLearning/GBM/BernoulliCalculator.h"
#include "MachineLearning/DecisionTree/DecisionTreeExperiment.h"

#include <cmath>
#include <boost/foreach.hpp>

BernoulliCalculator::BernoulliCalculator()
{}

BernoulliCalculator::~BernoulliCalculator()
{}

double BernoulliCalculator::calculateDeviance(vector<shared_ptr<DecisionTreeExperiment> >& experiments)
{
    double sumL = 0.0;
    double sumW = 0.0;

    BOOST_FOREACH(shared_ptr<DecisionTreeExperiment>& e, experiments)
    {
        double f = e->getF();
        sumL    += e->getWeight() * (e->getY() * f - log(1.0 + exp(f)));
        sumW    += e->getWeight();
    }
    return -2.0 * sumL / sumW;
}

void BernoulliCalculator::populateInitialF(vector<shared_ptr<DecisionTreeExperiment> >& experiments, bool useInitialPredictions)
{
    if (!useInitialPredictions)
    {
        double sumY = 0.0, sumWeight = 0.0;

        BOOST_FOREACH(shared_ptr<DecisionTreeExperiment>& e, experiments)
        {
            sumY        += e->getY() * e->getWeight();
            sumWeight   += e->getWeight();
        }

        double meanY = sumY / sumWeight;

        // the output object needs to know this value for applying to new experiments
        // m_output->setMeanY(meanY);

        BOOST_FOREACH(shared_ptr<DecisionTreeExperiment>& e, experiments)
            e->setPrediction(meanY);
    }    

    // now, update F for all our experiments
    BOOST_FOREACH(shared_ptr<DecisionTreeExperiment>& e, experiments)
    {
        double utility = calculateF(e->getPrediction());
        e->setF(utility);
    }
}

void BernoulliCalculator::updateZ(vector<shared_ptr<DecisionTreeExperiment> >& experiments)
{
    BOOST_FOREACH(shared_ptr<DecisionTreeExperiment>& experiment, experiments)
    {
        experiment->setZ(experiment->getY() - experiment->getPrediction());
    }
}

double BernoulliCalculator::computeFIncrement(vector<shared_ptr<DecisionTreeExperiment> >& experiments)
{
    // TV - I don't know much about this maths except that it's the standard for bernoulli (logit)
    double numerator = 0.0, denominator = 0.0;
    BOOST_FOREACH(shared_ptr<DecisionTreeExperiment>& experiment, experiments)
    {
        numerator += experiment->getWeight() * experiment->getZ();
        double p   = experiment->getPrediction();

        denominator += experiment->getWeight() * p * (1.0 - p);
    }

    return numerator / denominator;
}

void BernoulliCalculator::updatePredictions(vector<shared_ptr<DecisionTreeExperiment> >& experiments)
{
    BOOST_FOREACH(shared_ptr<DecisionTreeExperiment>& experiment, experiments)
    {
        experiment->setPrediction(calculatePrediction(experiment->getF()));
    }
}

double BernoulliCalculator::calculatePrediction(double f)
{
    return 1.0 / (1.0 + exp(-f));
}

double BernoulliCalculator::calculateF(double prediction)
{
    return log(prediction / (1.0 - prediction));
}
