#ifndef MLRunner_h__
#define MLRunner_h__

#include <boost/shared_ptr.hpp>
#include <vector>
using std::vector;
using boost::shared_ptr;

class MLData;
class MLExperiment;
class MLParameters;
class MLOutput;
class MLEstimator;

class MLRunner
{
public:
    MLRunner();
    virtual ~MLRunner();

    void execute();

    void    setData(MLData* data);
    MLData* getData();

    std::vector<double> getPredictions(MLData* newData);    
    std::vector<double> getMeanTrainingPredictions();
    std::vector<double> getCrossValidationPredictions();

    virtual shared_ptr<MLEstimator> createEstimator(MLData* data, vector<shared_ptr<MLExperiment> > trainingExperiments) = 0;

protected:
    virtual void checks();
    virtual void config();
    virtual void input();
    virtual void estimate();
    virtual void output();

    std::vector<double> getPredictions(vector<shared_ptr<MLExperiment> > experiments);
    
    MLData*                         m_data;
    vector<shared_ptr<MLOutput> >    m_outputObjects;
    vector<shared_ptr<MLEstimator> > m_estimators;

private:
};
#endif // MLRunner_h__
