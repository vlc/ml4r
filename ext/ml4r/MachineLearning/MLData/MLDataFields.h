#ifndef __MLDataFields_h__
#define __MLDataFields_h__

#include <string>
#include <vector>
using std::vector;
using std::string;

class MLDataFields
{
public:
    MLDataFields() {};
    ~MLDataFields() {};

    string experimentIdField;
    string weightsField;
    string actualYField;
    string initialPredictionsField;

    vector<string> featuresFields;

protected:
};

#endif // MLDataFields_h__