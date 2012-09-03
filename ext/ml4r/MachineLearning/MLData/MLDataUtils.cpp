#include "MachineLearning/MLData/MLDataUtils.h"
#include "MachineLearning/MLExperiment.h"
#include <../Eigen/Dense>

#include <boost/foreach.hpp>

MatrixXd MLDataUtils::makeMlDataFeatureMatrix( vector<shared_ptr<MLExperiment> > experiments, vector<int> featureIndices /* = vector() */ )
{
    int rows = experiments.size();
    if (featureIndices.size() == 0)
    {
        int featureCount = experiments.front()->getFeatureValues().size();
        for (int index = 0; index < featureCount; ++index)
            featureIndices.push_back(index);
    }
    int cols = featureIndices.size();

    MatrixXd matrix(rows,cols);

    int row = -1;
    BOOST_FOREACH(shared_ptr<MLExperiment> experiment, experiments)
    {
        ++row;
        int col = -1;
        BOOST_FOREACH(int featureIndex, featureIndices)
        {
            ++col;
            matrix(row,col) = experiment->getFeatureValue(featureIndex);
        }
    }
    return matrix;
}

VectorXd MLDataUtils::makeMlDataResponseVector( vector<shared_ptr<MLExperiment> > experiments )
{
    VectorXd responseVector(experiments.size());
    
    int index = -1;
    BOOST_FOREACH(shared_ptr<MLExperiment> experiment, experiments)
    {
        ++index;
        responseVector(index) = experiment->getY();
    }
    return responseVector;
}
