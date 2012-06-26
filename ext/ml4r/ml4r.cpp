#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/io.hpp>
namespace ublas = boost::numeric::ublas;
using ublas::matrix;


// #include <ruby.h>
//#include "LinearRegression/LinearRegression.h"
//#include "LinearRegression/ZenithRegression.h"

//VALUE rb_cRegression;

//void Init_ZenithRegression()
//{
//    rb_cRegression = rb_define_class("ZenithRegression", rb_cObject);
//    rb_define_singleton_method(rb_cRegression, "new",         zenith_regression_New, -1);
//    rb_define_method(rb_cRegression, "initialize",            zenith_regression_Initialize, 0);
//    rb_define_method(rb_cRegression, "observations=",         zenith_regression_observations, 1);
//    rb_define_method(rb_cRegression, "weights=",              zenith_regression_weights, 1);
//    rb_define_method(rb_cRegression, "setFixedConstant=",     zenith_regression_setFixedConstant, 1);
//    rb_define_method(rb_cRegression, "fittedYs",              zenith_regression_getFittedYs, 0);
//    rb_define_method(rb_cRegression, "predictedYs",           zenith_regression_getPredictedYs, 0);
//    rb_define_method(rb_cRegression, "regressionStatistics",  zenith_regression_getRegressionStatistics, 0);
//    rb_define_method(rb_cRegression, "execute",               zenith_regression_execute, 0);
//}

/* ruby calls this to load the extension */
//extern "C" void Init_ml4r(void) {
//    Init_ZenithRegression();
//}
