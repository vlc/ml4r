#ifndef RandomForestParameters_h__
#define RandomForestParameters_h__

#include <vector>
#include <string>
#include <limits>
using std::string;
using std::vector;

class RandomForestParameters
{
public:
    RandomForestParameters() 
    : minObservations(1), tryMVariables(100), numIterations(200), bagFraction(0.3), verbose(false),
      withReplacement(false),scale(std::numeric_limits<double>::infinity())
    {};
    ~RandomForestParameters() {};

    int             minObservations;
    int             tryMVariables;
    vector<string>  featuresToRun;
    int             numIterations;
    double          bagFraction;
    bool            verbose;
    bool            withReplacement;
    double          scale;
protected:
	
private:
};

#endif // RandomForestParameters_h__
