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

    void setExperiments(vector<shared_ptr<MLExperiment> > experiments);
    vector<shared_ptr<MLExperiment> >& getExperiments();
    shared_ptr<MLExperiment> getExperiment(int experimentIndex);
    
    void createFolds(int numFolds, int randomSeed);
    void setFolds(vector<int> folds);
        
    void setTrainingExperimentIds(vector<int>& experimentIds);
    void setTrainingExperiments(vector<shared_ptr<MLExperiment> > experiments);

    vector<int>& getFoldNumbers();
    vector<int>  getFolds();

    vector<shared_ptr<MLExperiment> >& getTrainingExperiments(int fold);
    vector<shared_ptr<MLExperiment> >& getCrossValidationExperiments(int fold);

    shared_ptr<MLExperiment> getExperimentWithId(int experimentId);
    // vector<shared_ptr<MLExperiment> >& getExperimentsSortedOnFeature(int featureIndex);
    
    vector<string>& getFeatures();
    void  setFeatures(vector<string> features);
    int   getFeatureIndex(string& feature);

    void        constructCategories(vector<string> categoricalFeatures);
    set<int>&   getCategoricalFeatureIndices();
    
    void setInitialPredictions(vector<double> initialPredictions);
    void setInitialPredictionsDefined(bool defined);
    bool initialPredictionsDefined();

    int getNumFolds();

    void setMissingValue(double missingValue);
    bool missingValueDefined();
    double getMissingValue();
protected:

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