#ifndef BernoulliCalculator_h__
#define BernoulliCalculator_h__

#include "MachineLearning/GBM/GBMCalculator.h"

#include <boost/shared_ptr.hpp>
#include <vector>
using std::vector;
using boost::shared_ptr;

class BernoulliCalculator : public GBMCalculator
{
public:
	BernoulliCalculator();
	~BernoulliCalculator();

    double calculateDeviance(vector<shared_ptr<DecisionTreeExperiment> >& experiments);
    void   populateInitialF(vector<shared_ptr<DecisionTreeExperiment> >& experiments, bool useInitialPredictions);
    void   updateZ(vector<shared_ptr<DecisionTreeExperiment> >& experiments);
    double computeFIncrement(vector<shared_ptr<DecisionTreeExperiment> >& experiments);
    void   updatePredictions(vector<shared_ptr<DecisionTreeExperiment> >& experiments);
    double calculatePrediction(double f);
    double calculateF(double prediction);
protected:
	
private:
};

#endif // BernoulliCalculator_h__
