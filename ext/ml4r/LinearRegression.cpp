#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/foreach.hpp>

#include "LinearRegression.h" 
#include "Utils.h"
namespace ublas = boost::numeric::ublas;

using std::vector;
using ublas::prod;
using ublas::matrix;

LinearRegression::LinearRegression() : m_constantIsFixed(false), m_paramsAreValid(false)
{}

LinearRegression::~LinearRegression()
{}

void LinearRegression::setXs(vector<vector<double> > xs)
{
    m_xs = xs;
}

void LinearRegression::setYs(vector<double> ys)
{
    m_ys = ys;
}

void LinearRegression::setWeights(vector<double> weights)
{
    m_ws = weights;
}

void LinearRegression::setFixedConstant(double val)
{
    m_constant = val;
    m_constantIsFixed = true;
}

pair<vector<double>,double> LinearRegression::getParameterEstimates()
{
    return make_pair(m_bs,m_constant);
}

void LinearRegression::checkDimensions()
{
    if (!m_ys.size())
        throw std::runtime_error("[LinearRegression] Number of observations equals zero");

    if (m_xs.size() != m_ys.size())
        throw std::runtime_error("[LinearRegression] Number of observations in x doesn't match number of observations in y");

    if (m_ws.size() && m_ws.size() != m_ys.size())
        throw std::runtime_error("[LinearRegression] Number of specified weights doesn't match number of observations");

    unsigned long dimensionOfX = m_xs.front().size();
    BOOST_FOREACH(vector<double>& x, m_xs)
        if (x.size() != dimensionOfX)
            throw std::runtime_error("[LinearRegression] Dimensions of x variables are inconsistent between observations");
}

void LinearRegression::calculateStatistics()
{
    if (!m_paramsAreValid) 
        throw std::runtime_error("[LinearRegression] Parameters have not been estimated");

    calculateModelStatistics();
    calculateParameterStatistics();
}

void LinearRegression::calculateParameterStatistics2()
{
    // form the matrix X'X
    ublas::matrix<double> X(m_xs.size(), m_xs.front().size()+1);
    ublas::matrix<double>::iterator2 matrixIterator = X.begin2();
    BOOST_FOREACH(vector<double>& row, m_xs)
    {
        matrixIterator = std::copy(row.begin(), row.end(), matrixIterator);
        *(matrixIterator++) = 1.0;
    }    
    ublas::matrix<double> X_transpose_X = ublas::prod(ublas::trans(X), X);

    // Invert the matrix 
    // cout << X_transpose_X << endl;
    ublas::matrix<double> X_transpose_X_inverse(X_transpose_X);
    // cout << X_transpose_X_inverse << endl;
    
    // TODO: Uncomment me
    // InvertMatrix(X_transpose_X, X_transpose_X_inverse);

    // cout << X_transpose_X_inverse << endl;

    // Also construct a t-stat for the constant
    if (!m_constantIsFixed) m_bs.push_back(m_constant);

    m_tStatistics.resize(m_bs.size());
    for (unsigned int i=0; i<m_bs.size(); ++i)
    {
        m_tStatistics.at(i) = m_bs.at(i) / (m_s * sqrt(X_transpose_X_inverse(i,i)));
    }

    if (!m_constantIsFixed) m_bs.pop_back();
}

void LinearRegression::calculateModelStatistics()
{
    checkDimensions();
    checkParametersAreEstimated();
    estimateYs();
    
    double meanY                = Utils::vectorSum(m_ys) / m_n;
    double sumSquaresTotal      = 0.0;
    double sumSquaresRegression = 0.0; 
    double sumSquaresError      = 0.0;
    double meanWeight           = Utils::vectorSum(m_ws) / m_n;
    for (int i=0; i<m_n; ++i)
    {
        sumSquaresTotal         += m_ws.at(i) / meanWeight * pow(m_ys.at(i)        - meanY,      2.0);
        sumSquaresRegression    += m_ws.at(i) / meanWeight * pow(m_fittedYs.at(i)  - meanY,      2.0);
        sumSquaresError         += m_ws.at(i) / meanWeight * pow(m_ys.at(i) - m_fittedYs.at(i), 2.0);
    }

    double meanSquaredRegression = sumSquaresRegression / (m_k);
    
    m_rSquared      = 1.0 - (sumSquaresError / sumSquaresTotal);
    m_adjustedRSquared = 1.0 - (sumSquaresError / (m_n - m_p)) / (sumSquaresTotal / (m_n - 1));
    m_fStatistic    = (m_n-m_p) * sumSquaresRegression / (sumSquaresError * m_k);
    m_sSquared      = 1.0 / (m_n-m_p) * sumSquaresError;
    m_s             = sqrt(m_sSquared);
    
    m_h_diagonal.resize(m_n, 0.0);
//     auto XIterator = m_X.begin2(); // row-wise
//     auto AIterator = m_A.begin1(); // column-wise
    for (int i = 0; i < m_n; ++i)
    {
        double sumProduct = 0.0;
        for (int j = 0; j < m_p; ++j)
            sumProduct += m_X(i, j) * m_A(j, i);
        m_h_diagonal.at(i) = sumProduct;
    }

    m_pressStatistic    = 0.0;
    m_presarStatistic   = 0.0;

    m_predictedYs.resize(m_n);
    for (int i = 0; i < m_n; ++i)
    {
        double ei = m_fittedYs.at(i) - m_ys.at(i);
        double hii = m_h_diagonal.at(i);
        double ei_prediction = ei / (1.0 - hii); // best thing eva!!!
        m_predictedYs.at(i)  = m_ys.at(i) + ei_prediction;
        m_presarStatistic   += m_ws.at(i) / meanWeight * abs((float)ei_prediction);
        m_pressStatistic    += m_ws.at(i) / meanWeight * pow(ei_prediction, 2.0);
    }
    m_rSquaredPrediction = 1.0 - m_pressStatistic / sumSquaresTotal;
}

void LinearRegression::estimateYs()
{
    m_fittedYs.clear();
    m_fittedYs.resize(m_ys.size(), m_constant);
    for (unsigned int i=0; i<m_ys.size(); ++i)
    {
        for (unsigned int j=0; j<m_bs.size(); ++j)
            m_fittedYs.at(i) += m_bs.at(j) * m_xs.at(i).at(j);
    }
}

void LinearRegression::checkParametersAreEstimated()
{
    if (!m_paramsAreValid) 
        throw std::runtime_error("[LinearRegression] Parameters have not been estimated");
}

double LinearRegression::getRSquared()
{
    return m_rSquared;
}

double LinearRegression::getFstatistic()
{
    return m_fStatistic;
}

vector<double>& LinearRegression::getFittedYs()
{
    return m_fittedYs;
}

vector<double>& LinearRegression::getTstatistics()
{
    return m_tStatistics;
}

void LinearRegression::populateMembers()
{
//     ublas::matrix<double>   m_X;
//     ublas::matrix<double>   m_Y;
//     ublas::matrix<double>   m_Xtranspose;
//     ublas::matrix<double>   m_Xtranspose_W_X;
//     ublas::matrix<double>   m_Xtranspose_W_X_inverse;

    m_k = m_xs.front().size();
    m_p = m_k + (m_constantIsFixed ? 0 : 1);
    m_n = m_xs.size();

    // populate m_X
    m_X.resize(m_n, m_p);
    ublas::matrix<double>::iterator2 matrixIterator = m_X.begin2();
    BOOST_FOREACH(vector<double>& row, m_xs)
    {
        matrixIterator = std::copy(row.begin(), row.end(), matrixIterator);
        if (!m_constantIsFixed) *(matrixIterator++) = 1.0;
    }    

//     for (int i = 0; i < m_X.size1(); ++i)
//         for (int j = 0; j < m_X.size2(); ++j)
//             vlcMessage.Write(ToString(m_X(i, j)));

    // populate m_Y
    m_Y.resize(m_n, 1);
    ublas::matrix<double>::iterator1 matrixIterator2 = m_Y.begin1();
    BOOST_FOREACH(double& y, m_ys)
    {
        (*matrixIterator2) = y;
        ++matrixIterator2;
    } 

    // populate m_ws with 1's if it's not already defined
    if (!m_ws.size())
    {
        m_ws.resize(m_n, 1.0);
    }

    // form the matrix X'  [P x N]
    m_Xtranspose = ublas::trans(m_X);

    // form the matrix X'WX [P x N] . [N x N] . [N x P] => [P x P]
    m_Xtranspose_W_X.resize(m_p, m_p);
    m_Xtranspose_W = multiplyMatrixByWeights(m_Xtranspose);
    m_Xtranspose_W_X = ublas::prod(m_Xtranspose_W, m_X);
    
    // Invert the matrix 

    m_Xtranspose_W_X_inverse.resize(m_p, m_p);
    // TODO: Uncomment me
    // InvertMatrix(m_Xtranspose_W_X, m_Xtranspose_W_X_inverse);
}

void LinearRegression::calculateParameterStatistics()
{
    m_tStatistics.resize(m_p);
    m_standardErrors.resize(m_p);

    ublas::matrix<double> AAt = prod(m_A, ublas::trans(m_A));
    for (int i=0; i<m_p; ++i)
    {
        // made more complicated by weights!!!
        m_standardErrors.at(i) = m_s * sqrt(AAt(i,i));
        m_tStatistics.at(i) = m_B(i,0) / m_standardErrors.at(i);
    }
}

double LinearRegression::getPressStatistic()
{
    return m_pressStatistic;
}

double LinearRegression::getPresarStatistic()
{
    return m_presarStatistic;
}

double LinearRegression::getRSquaredPrediction()
{
    return m_rSquaredPrediction;
}

vector<double>& LinearRegression::getPredictedYs()
{
    return m_predictedYs;
}

double LinearRegression::getAdjustedRSquared()
{
    return m_adjustedRSquared;
}

matrix<double> LinearRegression::multiplyMatrixByWeights(matrix<double>& mat)
{
    if (mat.size2() != m_ws.size())
        throw std::runtime_error("[LinearRegression::multiplyMatrixByWeights] invalid matrix dimensions!");

    matrix<double> new_matrix = mat; // copy
    for (unsigned int j = 0; j < new_matrix.size2(); ++j) // each column
    {
        double weight = m_ws.at(j);
        for (unsigned int i = 0; i < new_matrix.size1(); ++i) // each row
            new_matrix(i,j) *= weight;
    }
    return new_matrix;
}

matrix<double> LinearRegression::multiplyWeightsByMatrix(matrix<double>& mat)
{
    if (mat.size1() != m_ws.size())
        throw std::runtime_error("[LinearRegression::multiplyMatrixByWeights] invalid matrix dimensions!");

    matrix<double> new_matrix = mat; // copy
    for (unsigned int i = 0; i < new_matrix.size2(); ++i) // each row
    {
        double weight = m_ws.at(i);
        for (unsigned int j = 0; j < new_matrix.size1(); ++j) // each column
            new_matrix(i,j) *= weight;
    }
    return new_matrix;
}

vector<double>& LinearRegression::getStandardErrors()
{
    return m_standardErrors;
}

double LinearRegression::getSSquared()
{
    return m_sSquared;
}



