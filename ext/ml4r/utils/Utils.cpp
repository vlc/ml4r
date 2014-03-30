#include "utils/Utils.h"
#include <stdlib.h>

#include <vector>
using std::vector;

vector<int> Utils::vectorOfRandomInt(int length)
{
    vector<int> returnValue;
    returnValue.reserve(length);
    for (int i = 0; i < length; ++i)
        returnValue.push_back(rand());

    return returnValue;
}
