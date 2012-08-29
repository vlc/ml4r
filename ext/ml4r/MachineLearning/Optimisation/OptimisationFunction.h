#ifndef OptimisationFunction_h__
#define OptimisationFunction_h__

#include <boost/shared_ptr.hpp>
#include <vector>
using std::vector;
using boost::shared_ptr;

class OptimisationFunction
{
public:
	OptimisationFunction();
	~OptimisationFunction();

    virtual void            updateUsingParameters(vector<double> parameters) = 0;
    virtual double          getCost() = 0;
    virtual vector<double>  getFirstOrderDerivatives() = 0;
protected:
	
private:
};
#endif // OptimisationFunction_h__