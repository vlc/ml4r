#ifndef LinearRegression_h__
#define LinearRegression_h__

#include <boost/numeric/ublas/matrix.hpp>
using boost::numeric::ublas::matrix;

#include <vector>
#include <utility>
using std::pair;

class LinearRegression
{
public:
	LinearRegression(std::vector<std::vector<double> > xs, std::vector<double> ys,
	                 std::vector<double> weights = std::vector<double>())
	: m_xs(xs), m_ys(ys), m_ws(weights), m_constantIsFixed(false), m_paramsAreValid(false) {}
	~LinearRegression() {}

    // void setXs();
    // void setYs(std::vector<double> ys);
    //void setWeights(std::vector<double> weights);
    void setFixedConstant(double val);
    
    
    pair<std::vector<double>,double>  getParameterEstimates();
    std::vector<double>&              getFittedYs();
    std::vector<double>&              getPredictedYs();
    // double                         GetConstant();
    double                            getRSquared();
    double                            getSSquared();
    double                            getFstatistic();
    std::vector<double>&              getTstatistics();
    std::vector<double>&              getStandardErrors();
    double                            getPressStatistic();
    double                            getPresarStatistic();
    double                            getAdjustedRSquared();
    double                            getRSquaredPrediction();
    
    // BOOM THIS IS THE PROBLEM HERE - CAN'T INSTANTIATE A PURE VIRTUAL CLASS
    // virtual void Execute() = 0;

protected:

    // helpers
    boost::numeric::ublas::matrix<double> multiplyMatrixByWeights(boost::numeric::ublas::matrix<double>& mat);
    boost::numeric::ublas::matrix<double> multiplyWeightsByMatrix(boost::numeric::ublas::matrix<double>& mat);


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

    boost::numeric::ublas::matrix<double>   m_X;
    boost::numeric::ublas::matrix<double>   m_Y;
    boost::numeric::ublas::matrix<double>   m_B; // m_B = prod(m_A, m_Y)
    boost::numeric::ublas::matrix<double>   m_A; // m_A = (X'WX)-1 X'W
    std::vector<double>                     m_h_diagonal; // hat ublas::matrix = XA. This is the diagonal of it.
    boost::numeric::ublas::matrix<double>   m_Xtranspose;
    boost::numeric::ublas::matrix<double>   m_Xtranspose_W;
    boost::numeric::ublas::matrix<double>   m_Xtranspose_W_X;
    boost::numeric::ublas::matrix<double>   m_Xtranspose_W_X_inverse;

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