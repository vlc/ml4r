#ifndef MLDataUtils_h__
#define MLDataUtils_h__

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <vector>
using std::vector;

class MLData;
class MLExperiment;

namespace MLDataUtils
{
    vector<int> findValidFeaturesForRegression(vector<shared_ptr<MLExperiment> > experiments, vector<int> featuresIndicesToRun );

};

#endif // MLDataUtils_h__