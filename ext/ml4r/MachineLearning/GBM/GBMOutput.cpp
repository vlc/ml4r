#include "MachineLearning/GBM/GBMOutput.h"
#include "MachineLearning/GBM/BernoulliCalculator.h"
#include "MachineLearning/GBM/GaussianCalculator.h"
#include "MachineLearning/DecisionTree/DecisionTreeExperiment.h"
#include "MachineLearning/DecisionTree/DecisionTreeNode.h"

#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>
using boost::make_shared;

GBMOutput::GBMOutput(MLData* trainingData, vector<int> trainingExperimentIndicies, shared_ptr<GBMParameters> parameters)
  : MLOutput(trainingData, trainingExperimentIndicies), m_parameters(parameters), m_useMeanY(false)
{
    if (m_parameters->distribution == GAUSSIAN)
        m_gbmCalculator = make_shared<GaussianCalculator>();
    else if (m_parameters->distribution == BERNOULLI)
        m_gbmCalculator = make_shared<BernoulliCalculator>();
}

GBMOutput::~GBMOutput()
{

}

void GBMOutput::addHeadDecisionTreeNode(shared_ptr<DecisionTreeNode> node)
{
    m_headNodes.push_back(node);
}

double GBMOutput::predictForExperiment(shared_ptr<MLExperiment> experiment)
{
    shared_ptr<DecisionTreeExperiment> dtExperiment = make_shared<DecisionTreeExperiment>(experiment);
    setPredictionForDecisionTreeExperiment(dtExperiment);
    return dtExperiment->getPrediction();
}

void GBMOutput::addFIncrements(map<shared_ptr<DecisionTreeNode>, double> fIncrements)
{
    m_fIncrements.push_back(fIncrements);
}

void GBMOutput::setMeanY(double y)
{
    m_meanY = y;
    m_useMeanY = true;
}

int GBMOutput::getNumTrees()
{
    return (int) m_headNodes.size();
}

shared_ptr<GBMParameters> GBMOutput::getParameters()
{
    return m_parameters;
}

void GBMOutput::capTrees( int numTrees )
{
    m_headNodes.resize(numTrees);
    m_fIncrements.resize(numTrees);
}

void GBMOutput::setPredictionForDecisionTreeExperiment( shared_ptr<DecisionTreeExperiment> experiment )
{
    // determine initial F
    double initialPrediction = (m_useMeanY ? m_meanY : experiment->getPrediction());

    experiment->setF(m_gbmCalculator->calculateF(initialPrediction));

    int index = -1;
    BOOST_FOREACH(shared_ptr<DecisionTreeNode> node, m_headNodes)
    {
        ++index;
        shared_ptr<DecisionTreeNode> terminalNode = (node->isTerminalNode() ? node : node->getTerminalNodeForExperiment(experiment));
        if (terminalNode.get() == 0)
            terminalNode = node;

        if (m_fIncrements.at(index).find(terminalNode) == m_fIncrements.at(index).end())
            throw std::runtime_error("We have no increment for this terminal node!!");

        double incrementF = m_fIncrements.at(index)[terminalNode];
        experiment->incrementF(incrementF);
    }
    experiment->setPrediction(m_gbmCalculator->calculatePrediction(experiment->getF()));
}
