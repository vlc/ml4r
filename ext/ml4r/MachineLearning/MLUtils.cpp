#include "MachineLearning/MLUtils.h"

#include <boost/foreach.hpp>

double MLUtils::getMeanY(vector<shared_ptr<MLExperiment> > experiments)
{   
    double sumY = 0.0, sumWeight = 0.0;
    BOOST_FOREACH(shared_ptr<MLExperiment>& e, experiments)
    {
        sumY        += e->getY() * e->getWeight();
        sumWeight   += e->getWeight();
    }

    return sumY / sumWeight;
}
