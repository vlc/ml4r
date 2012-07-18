#ifndef MLEstimator_h__
#define MLEstimator_h__

class MLOutput;
class MLData;
class MLExperiment;

#include <vector>
#include <boost/shared_ptr.hpp>
using boost::shared_ptr;
using std::vector;

// pure virtual class
class MLEstimator
{
public:
    MLEstimator(MLData* data, vector<shared_ptr<MLExperiment> > experiments)
        : m_data(data), m_trainingExperiments(experiments)
    {};
    ~MLEstimator() {};

    virtual shared_ptr<MLOutput> estimate() = 0;
protected:
    MLData*                          m_data;
    vector<shared_ptr<MLExperiment> > m_trainingExperiments;

private:
};

#endif // MLAlgorithm_h__