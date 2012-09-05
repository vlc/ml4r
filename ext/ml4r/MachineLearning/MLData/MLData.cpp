#include "MachineLearning/MLData/MLData.h"
#include "MachineLearning/MLExperiment.h"
#include "utils/Utils.h"

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
using boost::lexical_cast;

MLData::MLData() : m_missingValueDefined(false)
{}

MLData::~MLData()
{}

vector<shared_ptr<MLExperiment> >& MLData::getExperiments()
{
    return m_experiments;
}

vector<shared_ptr<MLExperiment> >& MLData::getTrainingExperiments(int fold)
{
    return m_trainingExperiments[fold];
}

shared_ptr<MLExperiment> MLData::getExperimentWithId(int experimentId)
{
    if (m_experimentsById.find(experimentId) == m_experimentsById.end())
        throw std::runtime_error(string("Could not find experiment with id: " + lexical_cast<string>(experimentId)).c_str());

    return m_experimentsById[experimentId];
}

void MLData::setExperiments(vector<shared_ptr<MLExperiment> > experiments)
{
    m_experiments = experiments;
    BOOST_FOREACH(shared_ptr<MLExperiment>& experiment, m_experiments)
        m_experimentsById[experiment->getExperimentId()] = experiment;

    createFolds(1, 0);
}

vector<string> MLData::getFeatureNames()
{
    return m_featureNames;
}

void MLData::setFeatureNames(vector<string> features)
{
    m_featureNames = features;
    int index = -1;
    BOOST_FOREACH(string& e, m_featureNames)
    {
        ++index;
        m_featureIndices[e] = index;
    }
}

void MLData::constructCategories(vector<string> categoricalFeatures)
{
    BOOST_FOREACH(string& categoricalFeature, categoricalFeatures)
    {
        if (m_featureIndices.find(categoricalFeature) == m_featureIndices.end())
            throw std::runtime_error("Could not find categorical feature '" + categoricalFeature + "' in list of features to load or run");

        int featureIndex = m_featureIndices[categoricalFeature];
        m_categoricalFeatureIndices.insert(featureIndex);
        
    }
}

int MLData::getFeatureIndex(string& feature)
{
    const map<string,int>::iterator it = m_featureIndices.find(feature);
    if (it == m_featureIndices.end())
        throw std::runtime_error("Cannot get feature index for feature '" + feature + "'. Feature not loaded.");

    return it->second;
}

set<int>& MLData::getCategoricalFeatureIndices()
{
    return m_categoricalFeatureIndices;
}

void MLData::setInitialPredictionsDefined(bool defined)
{
    m_initialPredictionsDefined = defined;
}

bool MLData::initialPredictionsDefined()
{
    return m_initialPredictionsDefined;
}

void MLData::createFolds(int numFolds, int randomSeed)
{
    m_foldNumbers.clear();
    for (int i = 0; i < numFolds; ++i)
        m_foldNumbers.push_back(i);
    
    m_cvExperiments.clear();
    m_trainingExperiments.clear();

    if (m_foldNumbers.size() == 1)
    {    
        m_trainingExperiments[0] = m_experiments;
        return;
    }

    srand(randomSeed);
    vector<int> folds                   = Utils::vectorRange<int>(0,numFolds-1);
    vector<int> repeatedFolds           = Utils::vectorRepeat(folds, m_experiments.size());
    vector<int> randomlySortedFolds     = Utils::vectorShuffle(repeatedFolds);
        

    int index = -1;
    BOOST_FOREACH(shared_ptr<MLExperiment> experiment, m_experiments)
    {
        ++index;
        for (int fold = 0; fold < numFolds; ++fold)
        {
            if (randomlySortedFolds.at(index) == fold)
                m_cvExperiments[fold].push_back(experiment);
            else
                m_trainingExperiments[fold].push_back(experiment);
        }
    }
}


void MLData::setFolds( vector<int> experimentFolds )
{
    m_cvExperiments.clear();
    m_trainingExperiments.clear();
    m_foldNumbers.clear();

    if (experimentFolds.size() != m_experiments.size())
        throw std::runtime_error("[MLData::setFolds] to use this method, the list of folds must have the same length as the list of training experiments!");
    
    BOOST_FOREACH(int fold, experimentFolds)
        m_foldNumbers.push_back(fold);

    int index = -1;
    BOOST_FOREACH(shared_ptr<MLExperiment> experiment, m_experiments)
    {
        ++index;
        BOOST_FOREACH(int fold, m_foldNumbers)
        {
            int experimentFold = experimentFolds.at(index);
            if (fold == experimentFold)
                m_cvExperiments[fold].push_back(experiment);
            else
                m_trainingExperiments[fold].push_back(experiment);
        }
    }
}

int MLData::getNumFolds()
{
    return (int) m_foldNumbers.size();
}

vector<shared_ptr<MLExperiment> >& MLData::getCrossValidationExperiments(int fold)
{
    return m_cvExperiments[fold];
}

void MLData::setMissingValue(double missingValue)
{
    m_missingValueDefined = true;
    m_missingValue = missingValue;
}

bool MLData::missingValueDefined()
{
    return m_missingValueDefined;
}

double MLData::getMissingValue()
{
    if (!m_missingValueDefined)
        throw std::runtime_error("Cannot ask for missing value when it is not defined!");

    return m_missingValue;
}

shared_ptr<MLExperiment> MLData::getExperiment( int experimentIndex )
{
    return m_experiments.at(experimentIndex);
}

vector<int>& MLData::getFoldNumbers()
{
    return m_foldNumbers;
}

vector<int> MLData::getFolds()
{
    vector<int> folds(m_experiments.size());
    
    typedef pair<int, vector<shared_ptr<MLExperiment> > > ElementType;
    BOOST_FOREACH(ElementType p, m_cvExperiments)
    {
        int fold = p.first;
        vector<shared_ptr<MLExperiment> > experiments = p.second;

        BOOST_FOREACH(shared_ptr<MLExperiment>& experiment, experiments)
        {
            folds.at(experiment->getExperimentIndex()) = fold;
        }
    }
    return folds;
}

void MLData::setXs( vector<vector<double> > xs )
{
    if (m_experiments.size() != 0 && m_experiments.size() != xs.size())
        throw std::runtime_error("[MLData::setXs] - length of xs does not match number of existing records");

    if (m_experiments.size() == 0)
        createEmptyExperiments(xs.size());

    for (int i = 0; i < xs.size(); ++i)
        m_experiments.at(i)->setFeatureValues(xs.at(i));
}

void MLData::setYs( vector<double> ys )
{
    if (m_experiments.size() != 0 && m_experiments.size() != ys.size())
        throw std::runtime_error("[MLData::setXs] - length of ys does not match number of existing records");

    if (m_experiments.size() == 0)
        createEmptyExperiments(ys.size());

    for (int i = 0; i < ys.size(); ++i)
        m_experiments.at(i)->setY(ys.at(i));

}

void MLData::setWeights( vector<double> ws )
{
    if (m_experiments.size() != 0 && m_experiments.size() != ws.size())
        throw std::runtime_error("[MLData::setWeights] - length of weights does not match number of existing records");

    if (m_experiments.size() == 0)
        createEmptyExperiments(ws.size());

    for (int i = 0; i < ws.size(); ++i)
        m_experiments.at(i)->setWeight(ws.at(i));
}

void MLData::setInitialPredictions( vector<double> initialPredictions )
{
    if (m_experiments.size() != 0 && m_experiments.size() != initialPredictions.size())
        throw std::runtime_error("[MLData::setInitialPredictions] - Initial predictions are not of the same length as existing records. " +
        lexical_cast<string>(initialPredictions.size()) + " versus " + lexical_cast<string>(m_experiments.size()));

    if (m_experiments.size() == 0)
        createEmptyExperiments(initialPredictions.size());

    for (int i = 0; i < initialPredictions.size(); ++i)
        m_experiments.at(i)->setPrediction(initialPredictions.at(i));

    m_initialPredictionsDefined = true;
}

void MLData::createEmptyExperiments( int size )
{
    m_experiments.clear();
    m_experiments.reserve(size);

    for (int i = 0; i < size; ++i)
        m_experiments.push_back(shared_ptr<MLExperiment>(new MLExperiment()));
}

std::vector<std::vector<double> > MLData::getXs()
{
    vector<vector<double> > returnValue(m_experiments.size());

    int index = -1;
    BOOST_FOREACH(shared_ptr<MLExperiment> experiment, m_experiments)
    {
        ++index;
        returnValue.at(index) = experiment->getFeatureValues();
    }
    return returnValue;
}

std::vector<double> MLData::getYs()
{
    vector<double> returnValue(m_experiments.size());

    int index = -1;
    BOOST_FOREACH(shared_ptr<MLExperiment> experiment, m_experiments)
    {
        ++index;
        returnValue.at(index) = experiment->getY();
    }
    return returnValue;
}

std::vector<double> MLData::getWeights()
{
    vector<double> returnValue(m_experiments.size());

    int index = -1;
    BOOST_FOREACH(shared_ptr<MLExperiment> experiment, m_experiments)
    {
        ++index;
        returnValue.at(index) = experiment->getWeight();
    }
    return returnValue;
}

std::vector<double> MLData::getPredictions()
{
    vector<double> returnValue(m_experiments.size());

    int index = -1;
    BOOST_FOREACH(shared_ptr<MLExperiment> experiment, m_experiments)
    {
        ++index;
        returnValue.at(index) = experiment->getPrediction();
    }
    return returnValue;
}




