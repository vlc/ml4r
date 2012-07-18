#ifndef __MLExperiment_h__
#define __MLExperiment_h__

#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>
using boost::shared_ptr;
using std::map;
using std::vector;

class MLExperiment
{
public:
    MLExperiment();
    MLExperiment(int experimentId, int experimentIndex, double y, double initialPrediction,
        double weight, vector<double> features);
    
    MLExperiment(shared_ptr<MLExperiment> experiment);

    ~MLExperiment();

    double          getY();
    int             getExperimentId();
    int             getExperimentIndex();
    double          getPrediction();
    double          getWeight();
    vector<double>& getFeatureValues();
    double          getFeatureValue(int featureIndex);
    void            setPrediction(double prediction);
    
protected:
    double m_yValue;
    int m_experimentId;
    int m_experimentIndex;
    double m_prediction;
    double m_weight;
    vector<double> m_features;
    
};

#endif // MLExperiment_h__