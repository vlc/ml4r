#ifndef LinearRegression_h__
#define LinearRegression_h__

#include <boost/numeric/ublas/matrix.hpp>
namespace ublas = boost::numeric::ublas;
using boost::numeric::ublas::matrix;


#include <vector>
#include <utility>
using std::pair;

//#include "ZenithBase.h"
//#include "boost/MatrixInverse.h"

class LinearRegression
{
public:
	LinearRegression();
	~LinearRegression();

    void setXs(std::vector<std::vector<double> > xs);
    void setYs(std::vector<double> ys);
    void setWeights(std::vector<double> weights);
    void setFixedConstant(double val);
    
    
    pair<std::vector<double>,double>  getParameterEstimates();
    std::vector<double>&              getFittedYs();
    std::vector<double>&              getPredictedYs();
    // double                       GetConstant();
    double                       getRSquared();
    double                       getSSquared();
    double                       getFstatistic();
    std::vector<double>&              getTstatistics();
    std::vector<double>&              getStandardErrors();
    double                       getPressStatistic();
    double                       getPresarStatistic();
    double                       getAdjustedRSquared();
    double                       getRSquaredPrediction();
    
    virtual void Execute() = 0;

protected:

    // helpers
    ublas::matrix<double> multiplyMatrixByWeights(ublas::matrix<double>& mat);
    ublas::matrix<double> multiplyWeightsByMatrix(ublas::matrix<double>& mat);


    virtual void checkDimensions();
            void populateMembers();
            void checkParametersAreEstimated();
            void calculateStatistics();
            void calculateParameterStatistics();
            void calculateParameterStatistics2();
            void calculateModelStatistics();
            void estimateYs();

	std::vector<std::vector<double> >  m_xs;
    std::vector<double>           m_ys;
    std::vector<double>           m_ws;
    std::vector<double>           m_fittedYs;
    std::vector<double>           m_predictedYs;
    bool                     m_constantIsFixed;
    bool                     m_paramsAreValid;
 
    long                      m_k; // number of x variables
    long                      m_p; // number of parameters to be estimated (may include constant)
    long                      m_n; // number of observations
 
    std::vector<double>           m_bs;
    double                   m_constant;

    ublas::matrix<double>   m_X;
    ublas::matrix<double>   m_Y;
    ublas::matrix<double>   m_B; // m_B = prod(m_A, m_Y)
    ublas::matrix<double>   m_A; // m_A = (X'WX)-1 X'W
    std::vector<double>     m_h_diagonal; // hat ublas::matrix = XA. This is the diagonal of it.
    ublas::matrix<double>   m_Xtranspose;
    ublas::matrix<double>   m_Xtranspose_W;
    ublas::matrix<double>   m_Xtranspose_W_X;
    ublas::matrix<double>   m_Xtranspose_W_X_inverse;

    // Statistics of the regression
    std::vector<double>          m_tStatistics;
    std::vector<double>          m_standardErrors;
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