#include "MachineLearning/LogisticRegression/ZenithLogisticRegression.h"
#include "MachineLearning/LogisticRegression/LogisticRegressionRunner.h"
#include "MachineLearning/LogisticRegression/LogisticRegressionParameters.h"

#include "stringConversion.h"
#include "RubyUtils.h"
using namespace RubyUtils;

void zenith_logisticRegression_Free( void* v )
{
    delete (reinterpret_cast<LogisticRegressionRunner*>(v));
}

OtInterface::VALUE zenith_logisticRegression_New( int argc, VALUE* argv, VALUE klass )
{
    VALUE obj = otRuby->DataWrapStruct(klass, 0, zenith_logisticRegression_Free, 0);
    otRuby->rb_obj_call_init(obj, argc, argv);
    return obj;
}

OtInterface::VALUE zenith_logisticRegression_Initialize( VALUE self )
{
    if (otRuby->GetDataPtr(self)) zenith_logisticRegression_Free(otRuby->GetDataPtr(self));
    otRuby->SetDataPtr(self, NULL);

    LogisticRegressionRunner* lrr = new LogisticRegressionRunner();
    if (lrr == NULL) otRuby->rb_sys_fail("ZenithLogisticRegression class could not be created");
    otRuby->SetDataPtr(self, lrr);
    return self;
}

OtInterface::VALUE zenith_logisticRegression_estimate( VALUE self )
{
    LogisticRegressionRunner* lrr = (LogisticRegressionRunner*)otRuby->GetDataPtr(self);
    try
    {
        lrr->execute();
    }
    catch (std::exception e)
    {
        vlcMessage.Raise((string("Caught error: ") + e.what()).c_str());
    }

    return TOtRubyInterface::Qnil;
}

OtInterface::VALUE zenith_logisticRegression_setFeaturesToRun( VALUE self, VALUE featuresValue )
{
    LogisticRegressionRunner* lrr = (LogisticRegressionRunner*)otRuby->GetDataPtr(self);
    lrr->parameters->featuresToRun = RubyUtils::fromValue<vector<string>>(featuresValue);
    return TOtRubyInterface::Qnil;
}

OtInterface::VALUE zenith_logisticRegression_setData( VALUE self, VALUE data )
{
    LogisticRegressionRunner* lrr = (LogisticRegressionRunner*)otRuby->GetDataPtr(self);
    MLData* mlData = (MLData*)otRuby->GetDataPtr(data);
    lrr->setData(mlData);
    return TOtRubyInterface::Qnil;
}

OtInterface::VALUE zenith_logisticRegression_predictions( VALUE self, VALUE newMlData )
{
    LogisticRegressionRunner* lrr = (LogisticRegressionRunner*)otRuby->GetDataPtr(self);
    MLData* data = (MLData*)otRuby->GetDataPtr(newMlData);

    vector<double> predictions;

    try
    {
        predictions = lrr->getPredictions(data);
    }
    catch (std::exception e)
    {
        vlcMessage.Raise((string("Could not get predictions. Error: ") + e.what()).c_str());
    }

    return RubyUtils::toValue(predictions);
}

OtInterface::VALUE zenith_logisticRegression_training_predictions( VALUE self )
{
    LogisticRegressionRunner* lrr = (LogisticRegressionRunner*)otRuby->GetDataPtr(self);
    vector<double> predictions;

    try
    {
        predictions = lrr->getMeanTrainingPredictions();
    }
    catch (std::exception e)
    {
        vlcMessage.Raise((string("Could not get training predictions. Error: ") + e.what()).c_str());
    }

    return RubyUtils::toValue(predictions);
}

OtInterface::VALUE zenith_logisticRegression_crossvalidation_predictions( VALUE self )
{
    LogisticRegressionRunner* lrr = (LogisticRegressionRunner*)otRuby->GetDataPtr(self);
    vector<double> predictions;

    try
    {
        predictions = lrr->getCrossValidationPredictions();
    }
    catch (std::exception e)
    {
        vlcMessage.Raise((string("Could not get cross validation predictions. Error: ") + e.what()).c_str());
    }

    return RubyUtils::toValue(predictions);
}
