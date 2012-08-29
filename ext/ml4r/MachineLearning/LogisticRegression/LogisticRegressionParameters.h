#ifndef LogisticRegressionParameters_h__
#define LogisticRegressionParameters_h__

#include <vector>
using std::vector;

class LogisticRegressionParameters
{
public:
    LogisticRegressionParameters() 
    : lambda(0.0), alpha(0.01)
    {};

    ~LogisticRegressionParameters() {};

    vector<string>  featuresToRun;          // X's for this run
    vector<double>  seedParameterEstimates;

    double lambda; // for regularisation
    double alpha;  // for gradient descent
    
    FunctionOptimiser* functionOptimiser;
protected:
	
private:

};
#endif // LogisticRegressionParameters_h__