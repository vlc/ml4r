#ifndef GaussianCalculator_h__
#define GaussianCalculator_h__

#include "MachineLearning/GBM/GBMCalculator.h"

#include <boost/shared_ptr.hpp>
#include <vector>
using std::vector;
using boost::shared_ptr;

class GaussianCalculator : public GBMCalculator
{
public:
	GaussianCalculator();
	~GaussianCalculator();

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

#endif // GaussianCalculator_h__
