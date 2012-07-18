#ifndef GBMOutput_h__
#define GBMOutput_h__

#include "MachineLearning/GBM/GBMParameters.h"
#include "MachineLearning/MLOutput.h"

#include <boost/shared_ptr.hpp>
#include <vector>
#include <map>
using std::map;
using std::vector;
using boost::shared_ptr;

class DecisionTreeNode;
class DecisionTreeExperiment;
class MLData;
class GBMCalculator;

class GBMOutput : public MLOutput
{
public:
    GBMOutput(MLData* trainingData, vector<int> trainingExperimentIndicies,shared_ptr<GBMParameters> parameters);
    ~GBMOutput();

    // recording inputs of GBM estimation
    shared_ptr<GBMParameters>                   getParameters();

    // recording outputs of GBM estimation
    void setMeanY(double y);
    void addHeadDecisionTreeNode(shared_ptr<DecisionTreeNode> node);
    void addFIncrements(map<shared_ptr<DecisionTreeNode>, double> m_FIncrements);

    // applying model to new data
    double predictForExperiment(shared_ptr<MLExperiment> experiment);
    void setPredictionForDecisionTreeExperiment(shared_ptr<DecisionTreeExperiment> experiment);

    // stats
    int  getNumTrees();
    
    void capTrees(int numTrees);

protected:

    vector<shared_ptr<DecisionTreeNode> >                m_headNodes;
    vector<map<shared_ptr<DecisionTreeNode>, double> >   m_fIncrements;
    double                                              m_meanY;
    bool                                                m_useMeanY;
    shared_ptr<GBMParameters>                           m_parameters;
    shared_ptr<GBMCalculator>                           m_gbmCalculator;
private:
};

#endif // GBMOutput_h__
