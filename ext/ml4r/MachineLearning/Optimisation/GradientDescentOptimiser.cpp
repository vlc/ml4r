#include "MachineLearning/Optimisation/GradientDescentOptimiser.h"
#include "MachineLearning/Optimisation/OptimisationFunction.h"
#include "utils/Utils.h"
#include <limits>
#include "utils/VlcMessage.h"
#include <boost/lexical_cast.hpp>
using boost::lexical_cast;
#include <string>

using std::string;
using namespace Utils;

GradientDescentOptimiser::GradientDescentOptimiser()
    : alpha(0.01), maxParameterDelta(0.0001)
{

}

GradientDescentOptimiser::~GradientDescentOptimiser()
{

}

void GradientDescentOptimiser::optimise()
{
    double maxParameterChange = std::numeric_limits<double>::infinity();    
    vector<double> parameters = m_seedParameterEstimates;
    double costFunction       = std::numeric_limits<double>::infinity();

    int count = 0;
    while (maxParameterChange > maxParameterDelta && count < 100)
    {
        m_function->updateUsingParameters(parameters);
        costFunction = m_function->getCost();
        vector<double> firstOrderDerivatives = m_function->getFirstOrderDerivatives();

        vector<double> delta = firstOrderDerivatives;
        // Utils::operator*=(delta, -alpha);
        delta *= -alpha;

        // vector<double> deltaAbs = Utils::vectorAbs(delta);
        double maxParameterChange = 0.0;
        // maxParameterChange = Utils::vectorMax(Utils::vectorAbs(delta));
        for (int i = 0; i < delta.size(); ++i)
        {
            double absDelta = std::abs(delta.at(i));
            if (absDelta > maxParameterChange)
                maxParameterChange = absDelta;
        }

        // Utils::operator+=(parameters, delta);
        parameters += delta;

        count++;
    }
    m_finalParameterEstimates = parameters;
}
