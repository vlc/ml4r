#ifndef MLEstimatorFactory_h__
#define MLEstimatorFactory_h__

class MLExperiment;
class MLData;
class MLParameters;
class MLEstimator;

#include <boost/shared_ptr.hpp>
#include <vector>
using std::vector;
using boost::shared_ptr;

class MLEstimatorFactory
{
public:
    MLEstimatorFactory() {};
    ~MLEstimatorFactory() {};

    virtual shared_ptr<MLEstimator> create(MLData* data, vector<shared_ptr<MLExperiment> > trainingExperiments) = 0;
protected:
	
private:
};
#endif // MLEstimatorFactory_h__