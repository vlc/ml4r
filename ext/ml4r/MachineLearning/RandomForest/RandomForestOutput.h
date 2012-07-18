#ifndef RandomForestOutput_h__
#define RandomForestOutput_h__

#include "MachineLearning/MLOutput.h"


class MLData;
class RandomForestParameters;
class MLExperiment;
class DecisionTreeNode;

class RandomForestOutput : public MLOutput
{
public:
    RandomForestOutput( MLData* trainingData, 
                        vector<int> trainingExperimentIndicies,
                        shared_ptr<RandomForestParameters> parameters);
	~RandomForestOutput();

    MLData*                            getTrainingData();
    shared_ptr<RandomForestParameters> getParameters();

    double predictForExperiment(shared_ptr<MLExperiment> experiment);
    void addHeadDecisionTreeNode(shared_ptr<DecisionTreeNode> node);
    // stats
    int  getNumTrees();

protected:
    shared_ptr<RandomForestParameters>    m_parameters;
    vector<shared_ptr<DecisionTreeNode> >  m_headNodes;
private:
};

#endif // RandomForestOutput_h__
