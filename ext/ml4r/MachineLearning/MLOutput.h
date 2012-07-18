#ifndef MLOutput_h__
#define MLOutput_h__

#include <boost/shared_ptr.hpp>
#include <vector>
#include <stdexcept>
using std::runtime_error;
using std::vector;
using boost::shared_ptr;

#include "utils/Utils.h"

class MLData;
class MLExperiment;
class GBMParameters;

// pure virtual class

class MLOutput
{
public:
    MLOutput(MLData* data, vector<int> trainingExperimentIndicies)
        : m_trainingData(data), m_trainingExperimentIndicies(trainingExperimentIndicies)
    {};
    ~MLOutput() {};

    virtual double  predictForExperiment(shared_ptr<MLExperiment> experiment) = 0;
    virtual double  calculateAveragePredictions(vector<double> predictions) 
    {
        if (predictions.size() == 0)
            throw std::runtime_error("[MLOutput::calculateAveragePredictions] Cannot calculate an average on an empty vector!");

        return Utils::vectorSum<double>(predictions) / predictions.size();
    };

    vector<int> getTrainingExperimentIndicies() { return m_trainingExperimentIndicies; };

    MLData* getTrainingData() { return m_trainingData; }
protected:
    MLData*                          m_trainingData;
    vector<int>                      m_trainingExperimentIndicies;
        
private:
};
#endif // MLOutput_h__