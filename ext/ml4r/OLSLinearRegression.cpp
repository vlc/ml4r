#include "OLSLinearRegression.h"
#include "MathUtils.h"
#include "Utils.h"

#include <iostream>
#include <boost/numeric/ublas/io.hpp>
using std::cout;
using std::endl;

namespace ublas = boost::numeric::ublas;
using Utils::operator+=;
using ublas::matrix;
using ublas::prod;

OLSLinearRegression::OLSLinearRegression()
{}

OLSLinearRegression::~OLSLinearRegression()
{}

void OLSLinearRegression::Execute()
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
    

//     for (int i = 0; i < bs.size1(); ++i)
//         vlcMessage.Write(ToString(bs(i, 0)));

}

void OLSLinearRegression::EstimateBs()
{
    matrix<double> Y = m_Y;
    cout << Y << endl;
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

    cout << m_B << endl;

    m_paramsAreValid = true;
}
