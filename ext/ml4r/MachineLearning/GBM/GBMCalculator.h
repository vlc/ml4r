#ifndef GBMCalculator_h__
#define GBMCalculator_h__

#include <boost/shared_ptr.hpp>
#include <vector>
using std::vector;
using boost::shared_ptr;

class DecisionTreeExperiment;

class GBMCalculator
{
public:
    GBMCalculator() {};
    ~GBMCalculator() {};

    virtual double calculateDeviance(vector<shared_ptr<DecisionTreeExperiment> >& experiments) = 0;
    virtual void   populateInitialF(vector<shared_ptr<DecisionTreeExperiment> >& experiments, bool useInitialPredictions) = 0;
    virtual void   updateZ(vector<shared_ptr<DecisionTreeExperiment> >& experiments) = 0;
    virtual double computeFIncrement(vector<shared_ptr<DecisionTreeExperiment> >& experiments) = 0;
    virtual void   updatePredictions(vector<shared_ptr<DecisionTreeExperiment> >& experiments) = 0;
    virtual double calculatePrediction(double f) = 0;
    virtual double calculateF(double prediction) = 0;
protected:

private:
};

#endif // GBMCalculator_h__


