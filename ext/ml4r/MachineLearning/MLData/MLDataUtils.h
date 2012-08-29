#ifndef MLDataUtils_h__
#define MLDataUtils_h__

#include "stdafx.h"

class MLData;
class MLExperiment;

namespace MLDataUtils
{
    vector<int> findValidFeaturesForRegression(vector<shared_ptr<MLExperiment>> experiments, vector<int> featuresIndicesToRun );

};

#endif // MLDataUtils_h__