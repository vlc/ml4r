// #include "MachineLearning/MLData/ZenithMLDataReader.h"
// #include "MachineLearning/MLData/MLDataReader.h"
// #include "MachineLearning/MLData/MLDataFields.h"
// #include "MachineLearning/MLData/ZenithMLData.h"

// #include "RubyUtils.h"
// using namespace RubyUtils;

// void zenith_mldatareader_Free(void* v)
// {
//     delete (reinterpret_cast<MLDataReader*>(v));
// }

// OtInterface::VALUE zenith_mldatareader_New(int argc, VALUE* argv, VALUE klass)
// {
//     VALUE obj = otRuby->DataWrapStruct(klass, 0, zenith_mldatareader_Free, 0);
//     otRuby->rb_obj_call_init(obj, argc, argv);
//     return obj;
// }

// OtInterface::VALUE zenith_mldatareader_Initialize(VALUE self)
// {
//     if (otRuby->GetDataPtr(self)) zenith_mldatareader_Free(otRuby->GetDataPtr(self));
//     otRuby->SetDataPtr(self, NULL);

//     MLDataReader* mlDataReader = new MLDataReader();
//     if (mlDataReader == NULL) otRuby->rb_sys_fail("ZenithMLDataReader class could not be created");
//     otRuby->SetDataPtr(self, mlDataReader);
//     return self;
// }

// OtInterface::VALUE zenith_mldatareader_execute(VALUE self)
// {
//     MLDataReader* mlDataReader = (MLDataReader*)otRuby->GetDataPtr(self);
//     VALUE obj = zenith_mldata_New(0, 0, rb_cMLData);
//     MLData* mlData = (MLData*)otRuby->GetDataPtr(obj);

//     try
//     {
//         mlDataReader->execute(mlData);
//     }
//     catch (std::exception e)
//     {
//         vlcMessage.Raise((string("Caught error: ") + e.what()).c_str());
//     }

//     return obj;
// }

// OtInterface::VALUE zenith_mldatareader_setDatabaseName(VALUE self, VALUE databaseNameValue)
// {
//     MLDataReader* mlDataReader = (MLDataReader*)otRuby->GetDataPtr(self);
//     mlDataReader->databaseName = RubyUtils::fromValue<string>(databaseNameValue);
//     return TOtRubyInterface::Qnil;
// }

// OtInterface::VALUE zenith_mldatareader_setTableName(VALUE self, VALUE tableNameValue)
// {
//     MLDataReader* mlDataReader = (MLDataReader*)otRuby->GetDataPtr(self);
//     mlDataReader->tableName = RubyUtils::fromValue<string>(tableNameValue);
//     return TOtRubyInterface::Qnil;
// }

// OtInterface::VALUE zenith_mldatareader_setFeaturesToLoad(VALUE self, VALUE featuresValue)
// {
//     MLDataReader* mlDataReader = (MLDataReader*)otRuby->GetDataPtr(self);
//     mlDataReader->fieldsSpec->featuresFields = RubyUtils::fromValue<vector<string> >(featuresValue);
//     return TOtRubyInterface::Qnil;
// }

// OtInterface::VALUE zenith_mldatareader_setCategoricalFeatures(VALUE self, VALUE categoricalFeaturesValue)
// {
//     MLDataReader* mlDataReader = (MLDataReader*)otRuby->GetDataPtr(self);
//     mlDataReader->categoricalFeatures = RubyUtils::fromValue<vector<string> >(categoricalFeaturesValue);
//     return TOtRubyInterface::Qnil;
// }

// OtInterface::VALUE zenith_mldatareader_setActualYField(VALUE self, VALUE yFieldValue)
// {
//     MLDataReader* mlDataReader = (MLDataReader*)otRuby->GetDataPtr(self);
//     mlDataReader->fieldsSpec->actualYField = RubyUtils::fromValue<string>(yFieldValue);
//     return TOtRubyInterface::Qnil;
// }

// OtInterface::VALUE zenith_mldatareader_setExperimentIdField(VALUE self, VALUE experimentIdFieldValue)
// {
//     MLDataReader* mlDataReader = (MLDataReader*)otRuby->GetDataPtr(self);
//     mlDataReader->fieldsSpec->experimentIdField = RubyUtils::fromValue<string>(experimentIdFieldValue);
//     return TOtRubyInterface::Qnil;
// }

// OtInterface::VALUE zenith_mldatareader_setWeightsField(VALUE self, VALUE weightsFieldValue)
// {
//     MLDataReader* mlDataReader = (MLDataReader*)otRuby->GetDataPtr(self);
//     mlDataReader->fieldsSpec->weightsField = RubyUtils::fromValue<string>(weightsFieldValue);
//     return TOtRubyInterface::Qnil;

// }

// OtInterface::VALUE zenith_mldatareader_setInitialPredictionsField(VALUE self, VALUE initialEstimatesFieldValue)
// {
//     MLDataReader* mlDataReader = (MLDataReader*)otRuby->GetDataPtr(self);
//     mlDataReader->fieldsSpec->initialPredictionsField = RubyUtils::fromValue<string>(initialEstimatesFieldValue);
//     return TOtRubyInterface::Qnil;
// }

// OtInterface::VALUE zenith_mldatareader_setMissingValue(VALUE self, VALUE missingValue)
// {
//     MLDataReader* mlDataReader = (MLDataReader*)otRuby->GetDataPtr(self);
//     mlDataReader->missingValue = RubyUtils::fromValue<double>(missingValue);
//     mlDataReader->missingValueDefined = true;
//     return TOtRubyInterface::Qnil;
// }
