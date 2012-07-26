#include "MachineLearning/RandomForest/RandomForestRunner.h"
#include "MachineLearning/RandomForest/RandomForestOutput.h"
#include "MachineLearning/RandomForest/RandomForestEstimator.h"
#include "MachineLearning/DecisionTree/DecisionTreeNode.h"
#include "MachineLearning/MLData/MLData.h"

#include "utils/VlcMessage.h"

// #ifdef TBB_USE_THREADING_TOOLS
// #undef TBB_USE_THREADING_TOOLS
// #endif
// #define TBB_USE_THREADING_TOOLS 1
// #include "tbb/task_scheduler_init.h"
// #include "tbb/parallel_for.h"
// #include "tbb/blocked_range.h"
// #include "tbb/explicit_range.h"

#include <boost/pointer_cast.hpp>
using boost::dynamic_pointer_cast;
#include <math.h>

RandomForestRunner::RandomForestRunner()
{

}

RandomForestRunner::~RandomForestRunner()
{

}

void RandomForestRunner::estimateMore(int numTrees)
{
    int numFolds   = m_data->getNumFolds();
    int numThreads = m_data->getNumFolds();

    // tbb::task_scheduler_init init(numFolds);
    // static tbb::simple_partitioner sp;

    int grainSize       = numFolds / numThreads;

    // tbb::parallel_for(explicit_range<size_t>(0, numFolds, grainSize),
    //     [&](const explicit_range<size_t>& r) {
    //         int threadNumber = r.begin() / grainSize;
    //         for(size_t foldIndex=r.begin(); foldIndex!=r.end(); ++foldIndex)
            for(int foldIndex=numFolds; foldIndex<numFolds; ++foldIndex)
            {
                vlcMessage.Begin("Estimating more...");

                shared_ptr<RandomForestEstimator> estimator = dynamic_pointer_cast<RandomForestEstimator>(m_estimators.at(foldIndex));
                estimator->estimateMore(numTrees);

                vlcMessage.End();
            }
    // }, sp);
}

void RandomForestRunner::config()
{
    vector<string>& dataFeatures = m_data->getFeatures();

    if (parameters->featuresToRun.empty())
        parameters->featuresToRun = dataFeatures;
    else
    {    
        BOOST_FOREACH(string& feature, parameters->featuresToRun)
        {
            if (Utils::vectorIndex(dataFeatures, feature) == -1)
                throw std::runtime_error("Feature '" + feature + "' specified as part of parameter 'featuresToRun', but feature not found in data");
        }
    }
    if (parameters->featuresToRun.empty())
        throw std::runtime_error("There are no features to run!");

    if (m_data->missingValueDefined())
        DecisionTreeNode::setMissingValue(m_data->getMissingValue());
}



shared_ptr<MLEstimator> RandomForestRunner::createEstimator( MLData* data, vector<shared_ptr<MLExperiment> > trainingExperiments )
{
    return shared_ptr<MLEstimator>(shared_ptr<RandomForestEstimator>(new RandomForestEstimator(data, trainingExperiments, parameters)));
}
