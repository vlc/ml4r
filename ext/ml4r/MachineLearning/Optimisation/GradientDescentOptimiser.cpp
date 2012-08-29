#include "MachineLearning/Optimisation/GradientDescentOptimiser.h"
#include "MachineLearning/Optimisation/OptimisationFunction.h"
#include "OtUtils.h"

GradientDescentOptimiser::GradientDescentOptimiser()
    : alpha(0.01), maxParameterDelta(0.0001)
{

}

GradientDescentOptimiser::~GradientDescentOptimiser()
{

}

vector<double> GradientDescentOptimiser::optimise()
{
    double maxParameterChange = std::numeric_limits<double>::infinity();    
    vector<double> parameters = m_seedParameterEstimates;
    double costFunction       = std::numeric_limits<double>::infinity();

    while (maxParameterChange > maxParameterDelta)
    {
        m_function->updateUsingParameters(parameters);
        costFunction = m_function->getCost();
        vector<double> firstOrderDerivatives = m_function->getFirstOrderDerivatives();

        vector<double> delta = firstOrderDerivatives;
        OTUtils::operator*=(delta, -alpha);

        maxParameterChange = OTUtils::vectorMax(delta);

        OTUtils::operator+=(parameters, delta);
    }
    return parameters;
}
