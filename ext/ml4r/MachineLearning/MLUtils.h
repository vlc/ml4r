#ifndef __MLUtils_h__
#define __MLUtils_h__

#include "MachineLearning/MLExperiment.h"

#include <vector>
#include <boost/shared_ptr.hpp>
using boost::shared_ptr;
using std::vector;

namespace MLUtils
{
    double getMeanY(vector<shared_ptr<MLExperiment> > experiments);

    template <class T>
    vector<T> bagObjectsWithReplacement(vector<T> experiments, int bagSize);

    template <class T>
    pair<vector<T>,vector<T> > bagObjectsWithoutReplacement(vector<T> experiments, int bagSize);
};


template <class T>
vector<T> MLUtils::bagObjectsWithReplacement(vector<T> objects, int bagSize)
{
    vector<T> inBagObjects;

    int numTrainingExperiments  = objects.size();
    int numBagged               = 0;

    inBagObjects.reserve(bagSize);

    for (int i = 0; i < bagSize; ++i)
    {
        double unit_rand = rand() * 1.0 / (RAND_MAX + 1.0);
        int index = unit_rand * objects.size();
        inBagObjects.push_back(objects.at(index));
    }
    return inBagObjects;
}

template <class T>
pair<vector<T>,vector<T> > 
    MLUtils::bagObjectsWithoutReplacement(vector<T> objects, int bagSize)
{
    vector<T> inBagObjects;
    vector<T> outOfBagObjects;

    // int numTotalExperiments     = allExperiments.size();
    long numObjects  = objects.size();
    int numBagged               = 0;

    inBagObjects.reserve(bagSize);
    outOfBagObjects.reserve(numObjects - bagSize);

    for (int i = 0; i < numObjects; ++i)
    {
        double unit_rand = rand() * 1.0 / (RAND_MAX + 1.0);
        T object = objects.at(i);

        if (unit_rand * (numObjects - i) < bagSize - numBagged)
        {
            inBagObjects.push_back(object);
            ++numBagged;
        }
        else
            outOfBagObjects.push_back(object);
    }

    return make_pair(inBagObjects, outOfBagObjects);
}

#endif // MLUtils_h__