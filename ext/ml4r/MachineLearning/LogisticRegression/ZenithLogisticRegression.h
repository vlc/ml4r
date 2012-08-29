#ifndef ZenithLogisticRegression_h__
#define ZenithLogisticRegression_h__

#include "stdafx.h"

// ruby interface methods
void               zenith_logisticRegression_Free(void* v);
OtInterface::VALUE zenith_logisticRegression_New(int argc, VALUE* argv, VALUE klass);
OtInterface::VALUE zenith_logisticRegression_Initialize(VALUE self);
OtInterface::VALUE zenith_logisticRegression_estimate(VALUE self);
OtInterface::VALUE zenith_logisticRegression_setFeaturesToRun(VALUE self, VALUE featuresValue);
OtInterface::VALUE zenith_logisticRegression_setData(VALUE self, VALUE data);

OtInterface::VALUE zenith_logisticRegression_predictions(VALUE self, VALUE newMlData);
OtInterface::VALUE zenith_logisticRegression_training_predictions(VALUE self);
OtInterface::VALUE zenith_logisticRegression_crossvalidation_predictions(VALUE self);

#endif // ZenithLogisticRegression_h__