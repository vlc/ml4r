#include "LinearRegression/OLSLinearRegression.h"
#include "utils/MathUtils.h"
#include "utils/Utils.h"

#include <iostream>
#include <boost/numeric/ublas/io.hpp>
using std::cout;
using std::endl;

namespace ublas = boost::numeric::ublas;
using Utils::operator+=;
using ublas::matrix;
using ublas::prod;

OLSLinearRegression::OLSLinearRegression(std::vector<std::vector<double> > xs, std::vector<double> ys,
                                         std::vector<double> weights)
: LinearRegression(xs, ys, weights)
{
    calculate();
}

OLSLinearRegression::~OLSLinearRegression()
{}

void OLSLinearRegression::calculate()
{
    checkDimensions();
    // matrix based implementation

    // b = inverse(X'WX)X'Wy
    // where X is the data matrix (rows are observations, columns are our X variables.  If a constant is to be estimated,
    // then the first column is set to 1, and the first estimated parameter will be the constant).
    // X' is the transpose of X
    // W is a matrix with diag(w1, w2, w3) etc, where wi is the weight of observation i
    // y is the column matrix containing the observed y's.
    populateMembers();
    EstimateBs();
    if (m_paramsAreValid) calculateStatistics();
}

void OLSLinearRegression::EstimateBs()
{
    matrix<double> Y = m_Y;
    if (m_constantIsFixed)
    {
        for (int i = 0; i < m_n; ++i) Y(i, 0) -= m_constant;
    }
    
    m_A = prod(m_Xtranspose_W_X_inverse, m_Xtranspose_W);
    m_B = prod(m_A, Y);
    
    // set m_bs and constant
    m_bs.resize(m_k);
    for (int i = 0; i < m_k; ++i)
        m_bs.at(i) = m_B(i, 0);

    if (!m_constantIsFixed)
        m_constant = m_B(m_p-1, 0);

    m_paramsAreValid = true;
}
