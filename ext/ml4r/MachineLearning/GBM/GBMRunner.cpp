
#include "MachineLearning/GBM/GBMRunner.h"
#include "MachineLearning/GBM/GBMEstimator.h"
#include "MachineLearning/GBM/GBMOutput.h"
#include "MachineLearning/GBM/BernoulliCalculator.h"
#include "MachineLearning/GBM/GaussianCalculator.h"
#include "MachineLearning/MLData/MLData.h"
#include "MachineLearning/DecisionTree/DecisionTreeExperiment.h"
#include "MachineLearning/DecisionTree/DecisionTreeNode.h"
#include "MachineLearning/DecisionTree/FeatureInteraction.h"

#include "utils/VlcMessage.h"

// #ifdef TBB_USE_THREADING_TOOLS
// #undef TBB_USE_THREADING_TOOLS
// #endif
// #define TBB_USE_THREADING_TOOLS 1
// #include "tbb/task_scheduler_init.h"
// #include "tbb/parallel_for.h"
// #include "tbb/blocked_range.h"
// #include "tbb/explicit_range.h"

#include <cmath>
#include <boost/pointer_cast.hpp>
#include <boost/make_shared.hpp>
#include <boost/foreach.hpp>
using boost::make_shared;
using boost::dynamic_pointer_cast;


GBMRunner::GBMRunner()
{
    parameters = make_shared<GBMParameters>();
}

GBMRunner::~GBMRunner()
{

}

void GBMRunner::config()
{
    
    vector<string> dataFeatures = m_data->getFeatureNames();

    // parameters->loadedFeatures = dataFeatures;
    if (parameters->featuresToRun.empty())
        parameters->featuresToRun = dataFeatures;
    else
    {    
        BOOST_FOREACH(string feature, parameters->featuresToRun)
        {
            if (!Utils::hasElement(dataFeatures, feature))
                throw std::runtime_error("Feature '" + feature + "' specified as part of parameter 'featuresToRun', but feature not found in data");
        }
    }
    if (parameters->featuresToRun.empty())
        throw std::runtime_error("There are no features to run!");

    if (m_data->missingValueDefined())
        DecisionTreeNode::setMissingValue(m_data->getMissingValue());

}

void GBMRunner::estimateMore(int numTrees)
{
    int numFolds             = m_data->getNumFolds();
    int numThreads           = numFolds; // TODO: change this!

    // tbb::task_scheduler_init init(numFolds);
    // static tbb::simple_partitioner sp;

    int grainSize       = numFolds / numThreads;

    // tbb::parallel_for(explicit_range<size_t>(0, numFolds, grainSize),
    //     [&](const explicit_range<size_t>& r) {
    //         int threadNumber = r.begin() / grainSize;
    //         for(size_t foldIndex=r.begin(); foldIndex!=r.end(); ++foldIndex)
            for (int foldIndex = 0; foldIndex < numFolds; ++foldIndex)
            {
                vlcMessage.Begin("Estimating more...");
                
                shared_ptr<GBMEstimator> estimator = dynamic_pointer_cast<GBMEstimator>(m_estimators.at(foldIndex));
                estimator->estimateMore(numTrees);

                vlcMessage.End();
            }
    // }, sp);
}

void GBMRunner::capTrees( int numTrees )
{
    BOOST_FOREACH(shared_ptr<MLOutput>& output, m_outputObjects)
    {
        shared_ptr<GBMOutput> gbmOutput = dynamic_pointer_cast<GBMOutput>(output);
        gbmOutput->capTrees(numTrees);
    }
}

vector<FeatureInteraction> GBMRunner::getFeatureInteractions( int howMany )
{
    config();
    GBMEstimator gbmEstimator(m_data, m_data->getExperiments(), parameters);
    return gbmEstimator.findInteractions(howMany);
}

shared_ptr<MLEstimator> GBMRunner::createEstimator(MLData* data, vector<shared_ptr<MLExperiment> > trainingExperiments)
{
    return shared_ptr<MLEstimator>(shared_ptr<GBMEstimator>(new GBMEstimator(data, trainingExperiments, parameters)));
}







