#ifndef DecisionTreeExperiment_h__
#define DecisionTreeExperiment_h__

#include "MachineLearning/MLExperiment.h"
#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

class DecisionTreeExperiment : public MLExperiment
{
public:
	DecisionTreeExperiment();
    DecisionTreeExperiment(shared_ptr<MLExperiment> mlExperiment);
	~DecisionTreeExperiment();

    void    setF(double f);
    void    setZ(double z);
    double  getF();
    double  getY();
    double  getZ();
    void    incrementF(double increment);

protected:
    double m_F; // modelled estimate (utility in the case of bernoulli)
    double m_Z; // the thing which decision trees attempt to partition
private:
};



#endif // DecisionTreeExperiment_h__
