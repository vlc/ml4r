#ifndef GradientDescentOptimiser_h__
#define GradientDescentOptimiser_h__

#include "MachineLearning/Optimisation/FunctionOptimiser.h"

#include <boost/shared_ptr.hpp>
#include <vector>
using std::vector;
using boost::shared_ptr;

class GradientDescentOptimiser : public FunctionOptimiser
{
public:
    GradientDescentOptimiser();
    ~GradientDescentOptimiser();

    void optimise();
protected:

    double alpha;
    double maxParameterDelta;
private:
};
#endif // GradientDescentOptimiser_h__