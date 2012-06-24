#include <ruby.h>
#include "LinearRegression/LinearRegression.h"

VALUE rb_cRegression;

void Init_ZenithRegression()
{
    rb_cRegression = otRuby->rb_define_class("ZenithRegression", rb_cObject());
    otRuby->rb_define_singleton_method(rb_cRegression, "new", zenith_regression_New), -1);
    otRuby->rb_define_method(rb_cRegression, "initialize",            zenith_regression_Initialize, 0);
    otRuby->rb_define_method(rb_cRegression, "observations=",         zenith_regression_observations, 1);
    otRuby->rb_define_method(rb_cRegression, "weights=",              zenith_regression_weights, 1);
    otRuby->rb_define_method(rb_cRegression, "setFixedConstant=",     zenith_regression_setFixedConstant, 1);
    otRuby->rb_define_method(rb_cRegression, "fittedYs",              zenith_regression_getFittedYs, 0);
    otRuby->rb_define_method(rb_cRegression, "predictedYs",           zenith_regression_getPredictedYs, 0);
    otRuby->rb_define_method(rb_cRegression, "regressionStatistics",  zenith_regression_getRegressionStatistics, 0);
    otRuby->rb_define_method(rb_cRegression, "execute",               zenith_regression_execute, 0);
}

/* ruby calls this to load the extension */
void Init_ml4r(void) {
    Init_ZenithRegression
}
