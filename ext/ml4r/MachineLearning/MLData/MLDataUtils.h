#ifndef MLDataUtils_h__
#define MLDataUtils_h__

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <vector>
using std::vector;

class MLData;
class MLExperiment;

#include <../Eigen/Dense>
using Eigen::MatrixXd;
using Eigen::VectorXd;

namespace MLDataUtils
{
    vector<int> findValidFeaturesForRegression(vector<shared_ptr<MLExperiment> > experiments, vector<int> featuresIndicesToRun );
    MatrixXd    makeMlDataFeatureMatrix(vector<shared_ptr<MLExperiment> > experiments, vector<int> featureIndices = vector<int>());
    VectorXd    makeMlDataResponseVector(vector<shared_ptr<MLExperiment> > experiments);
    

};

#endif // MLDataUtils_h__