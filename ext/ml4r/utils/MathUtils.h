#ifndef __MathUtils_H__
#define __MathUtils_H__

#include <vector>
using std::vector;


namespace MathUtils
{
    vector<double>  solveSystemOfLinearEquations(vector<vector<double> > a, vector<double> b);
    void            checkSystemDimensions(vector<vector<double> >& a, vector<double>& b);
    void            eliminate(vector<vector<double> >& a, vector<double>& b, unsigned long index);
    vector<int>     identifyLinearlyDependentMatrixRows(vector<vector<double> >& matrix);
    double          convertRandomNumberToExtremeValue(long double randomNumber);
};


#endif // __MathUtils_H__