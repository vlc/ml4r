#include "utils/StochasticUtils.h"

vector<float> StochasticUtils::convertPdfToCumulativeSum(vector<float> pdf)
{
    float cumulativeSum = 0; 
    vector<float> returnVal(pdf.size());
    for (unsigned int i=0; i<pdf.size(); ++i)
    {
        returnVal[i] = cumulativeSum + pdf[i];
        cumulativeSum += pdf[i]; 
    }

    // normalise to unity sum
    if (cumulativeSum != 1.0)
    {
        for (unsigned int i=0; i<pdf.size(); ++i)
        {
            returnVal[i] /= cumulativeSum;
        } 
    }

    if (returnVal[returnVal.size()-1] != 1.0)
        returnVal[returnVal.size()-1] = 1.0; 

    return returnVal;
}

int StochasticUtils::chooseCategoryFromCdf( float * cumulativeProbabilities, int N )
{
    float selection = rand() / (RAND_MAX_FLOAT+1.0f);
    int i=0; 
    while ((i<N) && (selection > cumulativeProbabilities[i]))
        i++;
    return i;
}

int StochasticUtils::chooseCategoryFromCdf( vector<float>& cumulativeProbabilities )
{
    float selection = rand() / (RAND_MAX_FLOAT+1.0f);
    return chooseCategoryFromCdf(selection, cumulativeProbabilities);
}

int StochasticUtils::chooseCategoryFromCdf( float qot, vector<float>& cumulativeProbabilities )
{
    unsigned int i=0; 
    while ((i<cumulativeProbabilities.size()) && (qot > cumulativeProbabilities[i]))
        i++;
    return i;
}

int StochasticUtils::chooseCategoryFromPdf(vector<float>& probabilities, string categoryType)
{
    return chooseCategoryFromPdf(getQot(), probabilities, categoryType);
}

int StochasticUtils::chooseCategoryFromPdf(double qot, vector<float>& probabilities, string categoryType)
{
    if (!probabilities.size()) 
        throw runtime_error("There was a problem selecting a " + categoryType + " from an empty PDF!");
         
    double summative_usage = 0.0;
    unsigned int chosen_index = 0; 
    for (; chosen_index < probabilities.size(); ++chosen_index)
    {
        summative_usage += probabilities[chosen_index];
        if (summative_usage > qot) break;
    }
    if (chosen_index >= probabilities.size())
    {
        throw runtime_error("There was a problem selecting a " + categoryType + " from the PDF!");
    }
    return chosen_index; 
}