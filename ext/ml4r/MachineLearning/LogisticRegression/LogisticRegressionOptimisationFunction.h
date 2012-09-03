#ifndef LogisticRegressionOptimisationFunction_h__
#define LogisticRegressionOptimisationFunction_h__

#include "MachineLearning/Optimisation/OptimisationFunction.h"

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <vector>
using std::vector;

#include <../Eigen/Dense>
using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::RowVectorXd;
using Eigen::ArrayXd;

class MLExperiment;

class LogisticRegressionOptimisationFunction : public OptimisationFunction
{
public:
	LogisticRegressionOptimisationFunction(vector<shared_ptr<MLExperiment> > trainingExperiments, vector<int> featureIndices, double regularisationParameter);
	~LogisticRegressionOptimisationFunction();

    void            updateUsingParameters(vector<double> parameters);
    double          getCost();
    vector<double>  getFirstOrderDerivatives();

protected:
	vector<shared_ptr<MLExperiment> > m_trainingExperiments;
    MatrixXd                          m_trainingDataMatrix;
    VectorXd                          m_trainingResponseVector;
    vector<int>                       m_featureIndices;
    double                            m_regularisationParameter;
    double                            m_costFunction;
    vector<double>                    m_derivatives;

private:

};
#endif // LogisticRegressionOptimisationFunction_h__