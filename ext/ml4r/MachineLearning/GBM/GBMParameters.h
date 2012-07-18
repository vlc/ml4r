#ifndef __GBMParameters_h__
#define __GBMParameters_h__

#include <string>
#include <vector>
using std::vector;
using std::string;

enum GBMDistribution {
    BERNOULLI,
    GAUSSIAN
};

class GBMParameters
{
public:
    GBMParameters()
        : tryMVariables(-1),growKDecisionTreeNodes(5),bagFraction(1.0),
          shrinkageFactor(0.01),numIterations(100),minObservations(10),
          distribution(BERNOULLI), greedy(true), rfToLevel(0), verbose(false),
          scale(std::numeric_limits<double>::infinity())
    {};
    ~GBMParameters() {};

    // parameters will be public
    vector<string>  featuresToRun;          // X's for this run
    
    int             tryMVariables;
    int             growKDecisionTreeNodes;

    double          bagFraction;
    double          shrinkageFactor;
    int             numIterations;

    int             minObservations;
    vector<int>     trainingExperimentIds;
    
    GBMDistribution distribution;

    int             rfToLevel;
    bool            greedy;

    double          scale;

    bool            verbose;
protected:
};

#endif // GBMParameters_h__