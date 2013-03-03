#include "MachineLearning/MLRunner.h"
#include "MachineLearning/MLData/MLData.h"
#include "MachineLearning/MLOutput.h"
#include "MachineLearning/MLExperiment.h"
#include "MachineLearning/MLParameters.h"
#include "MachineLearning/MLEstimator.h"
#include "MachineLearning/MLEstimatorFactory.h"

#include "utils/VlcMessage.h"

#include <boost/lexical_cast.hpp>
using boost::lexical_cast;

#include <algorithm>
using std::max;

// #ifdef TBB_USE_THREADING_TOOLS
// #undef TBB_USE_THREADING_TOOLS
// #endif
// #define TBB_USE_THREADING_TOOLS 1
// #include "tbb/task_scheduler_init.h"
// #include "tbb/parallel_for.h"
// #include "tbb/blocked_range.h"
// #include "tbb/explicit_range.h"

#include <boost/foreach.hpp>

MLRunner::MLRunner()
    : m_data(0)
{

}

MLRunner::~MLRunner()
{

}

void MLRunner::execute()
{
    checks();
    config();
    input();
    estimate();
    output();
}

void MLRunner::checks()
{
    if (m_data == 0)
        throw std::runtime_error("[MLRunner::checks()] - MLData is not defined - have you provided input data?");
}

void MLRunner::config()
{
    
}

void MLRunner::input()
{

}

void MLRunner::estimate()
{
    vector<int>& foldNumbers = m_data->getFoldNumbers();
    long numFolds             = foldNumbers.size();
    long numThreads           = numFolds; // TODO: change this!

    // tbb::task_scheduler_init init(numFolds);
    // static tbb::simple_partitioner sp;

    //int grainSize       = numFolds / numThreads;

    m_outputObjects.resize(numFolds);
    m_estimators.resize(numFolds);

    //tbb::parallel_for(explicit_range<size_t>(0, numFolds, grainSize),
    //    [&](const explicit_range<size_t>& r) {
    //        int threadNumber = r.begin() / grainSize;
    //        for(size_t foldIndex=r.begin(); foldIndex!=r.end(); ++foldIndex)
            for (long foldIndex = 0; foldIndex < numFolds; ++foldIndex)
            {
                vlcMessage.Begin("Estimating");
                int foldNumber = foldNumbers.at(foldIndex);

                shared_ptr<MLEstimator> estimator = createEstimator(m_data, m_data->getTrainingExperiments(foldNumber));
                m_estimators.at(foldIndex)    = estimator;
                m_outputObjects.at(foldIndex) = estimator->estimate();
                
                vlcMessage.End();
            }
    //}, sp);
}

void MLRunner::output()
{

}

void MLRunner::setData( MLData* data )
{
    m_data = data;
}

MLData* MLRunner::getData()
{
    return m_data;
}

vector<double> MLRunner::getPredictions( MLData* newData )
{
    if (m_data->initialPredictionsDefined() && !newData->initialPredictionsDefined())
        throw std::runtime_error("Cannot apply model to new data as initial predictions are not defined (but were in initial data).");

    if (m_data->getFeatureNames() != newData->getFeatureNames())
        throw std::runtime_error("Features in prediction dataset do not match those in the estimation dataset (order is important)");

    return getPredictions(newData->getExperiments());
}

vector<double> MLRunner::getPredictions( vector<shared_ptr<MLExperiment> > experiments )
{
    // we can get a prediction from each of our outputs, when then need to be averaged.
    vector<double> predictions;
    predictions.reserve(experiments.size());

    BOOST_FOREACH(shared_ptr<MLExperiment> experiment, experiments)
    {
        vector<double> experimentPredictions;
        experimentPredictions.reserve(m_outputObjects.size());

        BOOST_FOREACH(shared_ptr<MLOutput> outputObject, m_outputObjects)
        {
            experimentPredictions.push_back(outputObject->predictForExperiment(experiment));
        }
        predictions.push_back(m_outputObjects.front()->calculateAveragePredictions(experimentPredictions));
    }
    return predictions;
}

vector<double> MLRunner::getMeanTrainingPredictions()
{
    long experimentCount = m_data->getExperiments().size();
    vector<double> meanPredictions;
    meanPredictions.reserve(experimentCount);

    vector<vector<double> > predictionsForEachFold(experimentCount);

    int numberOfTrainingPredictions = (m_outputObjects.size() == 1 ? 1 : (m_outputObjects.size() - 1));

    BOOST_FOREACH(vector<double>& experimentPredictions, predictionsForEachFold)
        experimentPredictions.reserve(numberOfTrainingPredictions);
    
    BOOST_FOREACH(shared_ptr<MLOutput> outputObject, m_outputObjects)
    {
        BOOST_FOREACH(int experimentIndex, outputObject->getTrainingExperimentIndicies())
        {
            double prediction = outputObject->predictForExperiment(m_data->getExperiment(experimentIndex));
            predictionsForEachFold.at(experimentIndex).push_back(prediction);
        }
    }

    // take the mean of our predictions
    BOOST_FOREACH(vector<double>& experimentPredictions, predictionsForEachFold)
        meanPredictions.push_back(m_outputObjects.front()->calculateAveragePredictions(experimentPredictions));

    return meanPredictions;
}

vector<double> MLRunner::getCrossValidationPredictions()
{
    int experimentCount = (int) m_data->getExperiments().size();
    vector<double> predictions(experimentCount);
    
    int foldIndex = -1;
    vector<int> foldNumbers = m_data->getFoldNumbers();

    BOOST_FOREACH(shared_ptr<MLOutput> outputObject, m_outputObjects)
    {
        ++foldIndex;
        int foldNumber = foldNumbers.at(foldIndex);

        BOOST_FOREACH(shared_ptr<MLExperiment> experiment, m_data->getCrossValidationExperiments(foldNumber))
        {
            double prediction = outputObject->predictForExperiment(experiment);
            predictions.at(experiment->getExperimentIndex()) = prediction;
        }
    }
    return predictions;
}