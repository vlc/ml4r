#ifndef FunctionOptimiser_h__
#define FunctionOptimiser_h__

#include <boost/shared_ptr.hpp>
#include <vector>

using std::vector;
using boost::shared_ptr;

class OptimisationFunction;

class FunctionOptimiser
{
public:
    FunctionOptimiser()  {};
    ~FunctionOptimiser() {};
    
    void setFunction(shared_ptr<OptimisationFunction> function) { m_function = function; }
    void setSeedParameterEstimates(vector<double> seedParameterEstimates) { m_seedParameterEstimates = seedParameterEstimates; }

    virtual void optimise() = 0;
    vector<double> getFinalParameterEstimates() { return m_finalParameterEstimates; }
protected:

    shared_ptr<OptimisationFunction> m_function;
    vector<double>                   m_seedParameterEstimates;
    vector<double>                   m_finalParameterEstimates;

private:
};
#endif // FunctionOptimiser_h__