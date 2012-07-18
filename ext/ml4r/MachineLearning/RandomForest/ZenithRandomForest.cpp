#include "MachineLearning/RandomForest/ZenithRandomForest.h"
#include "MachineLearning/RandomForest/RandomForestRunner.h"
#include "MachineLearning/RandomForest/RandomForestParameters.h"

#include "RubyUtils.h"
using namespace RubyUtils;

#include "stringConversion.h"

void zenith_randomForest_Free(void* v)
{
    delete (reinterpret_cast<RandomForestRunner*>(v));
}

OtInterface::VALUE zenith_randomForest_New(int argc, VALUE* argv, VALUE klass)
{
    VALUE obj = otRuby->DataWrapStruct(klass, 0, zenith_randomForest_Free, 0);
    otRuby->rb_obj_call_init(obj, argc, argv);
    return obj;
}

OtInterface::VALUE zenith_randomForest_Initialize(VALUE self)
{
    if (otRuby->GetDataPtr(self)) zenith_randomForest_Free(otRuby->GetDataPtr(self));
    otRuby->SetDataPtr(self, NULL);

    RandomForestRunner* randomForest = new RandomForestRunner();
    if (randomForest == NULL) otRuby->rb_sys_fail("ZenithRandomForest class could not be created");
    otRuby->SetDataPtr(self, randomForest);
    return self;
}

OtInterface::VALUE zenith_randomForest_estimate(VALUE self)
{
    RandomForestRunner* randomForest = (RandomForestRunner*)otRuby->GetDataPtr(self);
    try
    {
        randomForest->execute();
    }
    catch (std::exception e)
    {
        vlcMessage.Raise((string("Caught error: ") + e.what()).c_str());
    }

    return TOtRubyInterface::Qnil;
}


OtInterface::VALUE zenith_randomForest_estimateMore(VALUE self, VALUE numTrees)
{
    RandomForestRunner* randomForest = (RandomForestRunner*)otRuby->GetDataPtr(self);
    try
    {
        randomForest->estimateMore(RubyUtils::fromValue<int>(numTrees));
    }
    catch (std::exception e)
    {
        vlcMessage.Raise((string("Caught error: ") + e.what()).c_str());
    }

    return TOtRubyInterface::Qnil;
}


OtInterface::VALUE zenith_randomForest_setFeaturesToRun(VALUE self, VALUE featuresValue)
{
    RandomForestRunner* randomForest = (RandomForestRunner*)otRuby->GetDataPtr(self);
    randomForest->parameters->featuresToRun = RubyUtils::fromValue<vector<string> >(featuresValue);
    return TOtRubyInterface::Qnil;
}

OtInterface::VALUE zenith_randomForest_setData(VALUE self, VALUE data)
{
    RandomForestRunner* randomForest = (RandomForestRunner*)otRuby->GetDataPtr(self);
    MLData* mlData = (MLData*)otRuby->GetDataPtr(data);
    randomForest->setData(mlData);
    return TOtRubyInterface::Qnil;
}

OtInterface::VALUE zenith_randomForest_setTryMVariables(VALUE self, VALUE mVariablesValue)
{
    RandomForestRunner* randomForest = (RandomForestRunner*)otRuby->GetDataPtr(self);
    randomForest->parameters->tryMVariables = RubyUtils::fromValue<int>(mVariablesValue);
    return TOtRubyInterface::Qnil;
}

OtInterface::VALUE zenith_randomForest_setNumIterations(VALUE self, VALUE numIterationsValue)
{
    RandomForestRunner* randomForest = (RandomForestRunner*)otRuby->GetDataPtr(self);
    randomForest->parameters->numIterations = RubyUtils::fromValue<int>(numIterationsValue);
    return TOtRubyInterface::Qnil;
}

OtInterface::VALUE zenith_randomForest_setBagFraction(VALUE self, VALUE bagFractionValue)
{
    RandomForestRunner* randomForest = (RandomForestRunner*)otRuby->GetDataPtr(self);
    randomForest->parameters->bagFraction = RubyUtils::fromValue<double>(bagFractionValue);
    return TOtRubyInterface::Qnil;
}

OtInterface::VALUE zenith_randomForest_predictions(VALUE self, VALUE newMlData)
{
    RandomForestRunner* randomForest = (RandomForestRunner*)otRuby->GetDataPtr(self);
    MLData* data = (MLData*)otRuby->GetDataPtr(newMlData);

    vector<double> predictions;

    try
    {
        predictions = randomForest->getPredictions(data);
    }
    catch (std::exception e)
    {
        vlcMessage.Raise((string("Could not get predictions. Error: ") + e.what()).c_str());
    }

    return RubyUtils::toValue(predictions);
}

OtInterface::VALUE zenith_randomForest_training_predictions(VALUE self)
{
    RandomForestRunner* randomForest = (RandomForestRunner*)otRuby->GetDataPtr(self);
    vector<double> predictions;

    try
    {
        predictions = randomForest->getMeanTrainingPredictions();
    }
    catch (std::exception e)
    {
        vlcMessage.Raise((string("Could not get training predictions. Error: ") + e.what()).c_str());
    }

    return RubyUtils::toValue(predictions);
}

OtInterface::VALUE zenith_randomForest_crossvalidation_predictions(VALUE self)
{
    RandomForestRunner* randomForest = (RandomForestRunner*)otRuby->GetDataPtr(self);
    vector<double> predictions;

    try
    {
        predictions = randomForest->getCrossValidationPredictions();
    }
    catch (std::exception e)
    {
        vlcMessage.Raise((string("Could not get cross validation predictions. Error: ") + e.what()).c_str());
    }

    return RubyUtils::toValue(predictions);
}

OtInterface::VALUE zenith_randomForest_minObservations(VALUE self, VALUE minObservations)
{
    RandomForestRunner* randomForest = (RandomForestRunner*)otRuby->GetDataPtr(self);
    randomForest->parameters->minObservations = RubyUtils::fromValue<int>(minObservations);
    return TOtRubyInterface::Qnil;
}

OtInterface::VALUE zenith_randomForest_verbose(VALUE self, VALUE rb_verbose)
{
    RandomForestRunner* randomForest = (RandomForestRunner*)otRuby->GetDataPtr(self);
    bool verbose = RubyUtils::fromValue<bool>(rb_verbose);
    randomForest->parameters->verbose = verbose;
    return TOtRubyInterface::Qnil;
}

OtInterface::VALUE zenith_randomForest_withReplacement(VALUE self, VALUE rb_withReplacement)
{
    RandomForestRunner* randomForest = (RandomForestRunner*)otRuby->GetDataPtr(self);
    bool withReplacement = RubyUtils::fromValue<bool>(rb_withReplacement);
    randomForest->parameters->withReplacement = withReplacement;
    return TOtRubyInterface::Qnil;
}

OtInterface::VALUE zenith_randomForest_setScale( VALUE self, VALUE rb_scale )
{
    RandomForestRunner* randomForest = (RandomForestRunner*)otRuby->GetDataPtr(self);
    double scale = RubyUtils::fromValue<double>(rb_scale);
    randomForest->parameters->scale= scale;
    return TOtRubyInterface::Qnil;
}

