#ifndef LogisticRegressionOptimisationFunction_h__
#define LogisticRegressionOptimisationFunction_h__

#include "MachineLearning/Optimisation/OptimisationFunction.h"

#include <boost/shared_ptr.hpp>
#include <vector>
using std::vector;
using boost::shared_ptr;

class MLExperiment;

class LogisticRegressionOptimisationFunction : public OptimisationFunction
{
public:
	LogisticRegressionOptimisationFunction(vector<shared_ptr<MLExperiment> > trainingExperiments, vector<int> featureIndices);
	~LogisticRegressionOptimisationFunction();

    void            updateUsingParameters(vector<double> parameters);
    double          getCost();
    vector<double>  getFirstOrderDerivatives();

protected:
	vector<shared_ptr<MLExperiment>> m_trainingExperiments;
    vector<int>                      m_featureIndices;
private:

};
#endif // LogisticRegressionOptimisationFunction_h__