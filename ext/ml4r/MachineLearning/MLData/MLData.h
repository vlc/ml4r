#ifndef __MLData_h__
#define __MLData_h__

#include <boost/shared_ptr.hpp>
#include <vector>
#include <set>
#include <map>
#include <utility>
#include <string>
using std::string;
using std::pair;
using std::set;
using std::map;
using std::vector;
using boost::shared_ptr;

class MLExperiment;

class MLData
{
public:
    MLData();
    ~MLData();

    void setXs(std::vector<std::vector<double> > xs);
    void setYs(std::vector<double> ys);
    void setWeights(std::vector<double> ws);

    std::vector<std::vector<double> >   getXs();
    std::vector<double>                 getYs();
    std::vector<double>                 getWeights();
    std::vector<double>                 getPredictions();

    void setExperiments(std::vector<shared_ptr<MLExperiment> > experiments);
    std::vector<shared_ptr<MLExperiment> >& getExperiments();
    shared_ptr<MLExperiment> getExperiment(int experimentIndex);
    
    void createFolds(int numFolds, int randomSeed);
    void setFolds(std::vector<int> folds);
        
    void setTrainingExperimentIds(std::vector<int>& experimentIds);
    void setTrainingExperiments(std::vector<shared_ptr<MLExperiment> > experiments);

    std::vector<int>& getFoldNumbers();
    std::vector<int>  getFolds();

    std::vector<shared_ptr<MLExperiment> >& getTrainingExperiments(int fold);
    std::vector<shared_ptr<MLExperiment> >& getCrossValidationExperiments(int fold);

    shared_ptr<MLExperiment> getExperimentWithId(int experimentId);
    // vector<shared_ptr<MLExperiment> >& getExperimentsSortedOnFeature(int featureIndex);
    
    std::vector<std::string> getFeatureNames();
    void  setFeatureNames(std::vector<std::string> features);
    int   getFeatureIndex(std::string& feature);

    void        constructCategories(std::vector<std::string> categoricalFeatures);
    set<int>&   getCategoricalFeatureIndices();
    
    void setInitialPredictions(std::vector<double> initialPredictions);
    void setInitialPredictionsDefined(bool defined);
    bool initialPredictionsDefined();

    int getNumFolds();

    void setMissingValue(double missingValue);
    bool missingValueDefined();
    double getMissingValue();
protected:
    void createEmptyExperiments(int size);

    vector<shared_ptr<MLExperiment> > m_experiments;
    map<int, vector<shared_ptr<MLExperiment> > > m_trainingExperiments;
    map<int, vector<shared_ptr<MLExperiment> > > m_cvExperiments;

    map<int, shared_ptr<MLExperiment> > m_experimentsById;
    vector<string>                     m_featureNames;
    map<string, int>                   m_featureIndices;
    set<int>                           m_categoricalFeatureIndices;
    bool                               m_initialPredictionsDefined;
    vector<int>                        m_foldNumbers;
    double                             m_missingValue;
    bool                               m_missingValueDefined;
    // map<int, vector<shared_ptr<MLExperiment> > > m_experimentsSortedByFeature;
};

#endif // MLData_h__