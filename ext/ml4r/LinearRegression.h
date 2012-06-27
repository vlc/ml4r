#ifndef LinearRegression_h__
#define LinearRegression_h__

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/io.hpp>
namespace ublas = boost::numeric::ublas;
using ublas::matrix;


#include <vector>
#include <utility>
using std::pair;
using std::vector;

//#include "ZenithBase.h"
//#include "boost/MatrixInverse.h"

class LinearRegression
{
public:
	LinearRegression();
	~LinearRegression();

    void setXs(vector<vector<double> > xs);
    void setYs(vector<double> ys);
    void setWeights(vector<double> weights);
    void setFixedConstant(double val);
    
    
    pair<vector<double>,double>  getParameterEstimates();
    vector<double>&              getFittedYs();
    vector<double>&              getPredictedYs();
    // double                       GetConstant();
    double                       getRSquared();
    double                       getSSquared();
    double                       getFstatistic();
    vector<double>&              getTstatistics();
    vector<double>&              getStandardErrors();
    double                       getPressStatistic();
    double                       getPresarStatistic();
    double                       getAdjustedRSquared();
    double                       getRSquaredPrediction();
    
    virtual void Execute() = 0;

    // helpers
    matrix<double> multiplyMatrixByWeights(matrix<double>& mat);
    matrix<double> multiplyWeightsByMatrix(matrix<double>& mat);

protected:
    virtual void checkDimensions();
            void populateMembers();
            void checkParametersAreEstimated();
            void calculateStatistics();
            void calculateParameterStatistics();
            void calculateParameterStatistics2();
            void calculateModelStatistics();
            void estimateYs();

	vector<vector<double> >  m_xs;
    vector<double>           m_ys;
    vector<double>           m_ws;
    vector<double>           m_fittedYs;
    vector<double>           m_predictedYs;
    bool                     m_constantIsFixed;
    bool                     m_paramsAreValid;
 
    long                      m_k; // number of x variables
    long                      m_p; // number of parameters to be estimated (may include constant)
    long                      m_n; // number of observations
 
    vector<double>           m_bs;
    double                   m_constant;

    matrix<double>   m_X;
    matrix<double>   m_Y;
    matrix<double>   m_B; // m_B = prod(m_A, m_Y)
    matrix<double>   m_A; // m_A = (X'WX)-1 X'W
    vector<double>   m_h_diagonal; // hat matrix = XA. This is the diagonal of it.
    matrix<double>   m_Xtranspose;
    matrix<double>   m_Xtranspose_W;
    matrix<double>   m_Xtranspose_W_X;
    matrix<double>   m_Xtranspose_W_X_inverse;

    // Statistics of the regression
    vector<double>          m_tStatistics;
    vector<double>          m_standardErrors;
    double                  m_rSquared;
    double                  m_adjustedRSquared;
    double                  m_sSquared;
    double                  m_s;
    double                  m_fStatistic;
    double                  m_pressStatistic;
    double                  m_presarStatistic;
    double                  m_rSquaredPrediction;
private:

};

#endif // LinearRegression_h__