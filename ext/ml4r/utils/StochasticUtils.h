#ifndef __STOCHASTIC_UTILS_H__
#define __STOCHASTIC_UTILS_H__

#include <cstdlib>
#include <vector>
#include <string>
#include <map>
#include <stdexcept>
using std::runtime_error;
using std::map;
using std::string;
using std::vector;

#define RAND_MAX_FLOAT ((float)RAND_MAX)

namespace StochasticUtils
{
	vector<double> convertPdfToCumulativeSum(std::vector<double> pdf);
    vector<float> convertHistogramToPdf(vector<float> histogram);
                        int chooseCategoryFromCdf(float * cumulativeProbabilities, int N);
                        int chooseCategoryFromCdf(vector<float>& cumulativeProbabilities);
                        int chooseCategoryFromCdf(float qot, vector<float>& cumulativeProbabilities);
						int chooseCategoryFromPdf(vector<float>& probabilities, string categoryType = "object");
                        int chooseCategoryFromPdf(double qot, vector<float>& probabilities, string categoryType);
    template<class T>   T   chooseCategoryFromPdf(map<T,float>& probabilities);
    template<class T>   T   chooseCategoryFromPdf(double qot, map<T,float>& probabilities);
    inline double getQot()
    {
        return (double)rand() / ((double)(RAND_MAX)+(double)(1));
    }
};

#endif // __STOCHASTIC_UTILS_H__