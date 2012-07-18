// #include "MachineLearning/MLData/ZenithMLData.h"
// #include "MachineLearning/MLData/MLData.h"
// #include "MachineLearning/MLExperiment.h"

// #include "RubyUtils.h"
// using namespace RubyUtils;

// void zenith_mldata_Free(void* v)
// {
//     delete (reinterpret_cast<MLData*>(v));
// }

// OtInterface::VALUE zenith_mldata_New(int argc, VALUE* argv, VALUE klass)
// {
//     VALUE obj = otRuby->DataWrapStruct(klass, 0, zenith_mldata_Free, 0);
//     otRuby->rb_obj_call_init(obj, argc, argv);
//     return obj;
// }

// OtInterface::VALUE zenith_mldata_Initialize(VALUE self)
// {
//     if (otRuby->GetDataPtr(self)) zenith_mldata_Free(otRuby->GetDataPtr(self));
//     otRuby->SetDataPtr(self, NULL);

//     MLData* mlData = new MLData();
//     if (mlData == NULL) otRuby->rb_sys_fail("MLData class could not be created");
//     otRuby->SetDataPtr(self, mlData);
//     return self;
// }

// OtInterface::VALUE zenith_mldata_createFolds(VALUE self, VALUE rb_numFolds, VALUE rb_randomSeed)
// {
//     MLData* mlData = (MLData*)otRuby->GetDataPtr(self);
//     int numFolds;
//     int randomSeed;
//     RubyUtils::fromValue(rb_numFolds, numFolds);
//     RubyUtils::fromValue(rb_randomSeed, randomSeed);
//     mlData->createFolds(numFolds, randomSeed);
//     return TOtRubyInterface::Qnil;
// }

// OtInterface::VALUE zenith_mldata_getResponse(VALUE self)
// {
//     vector<double> response;
//     MLData* mlData = (MLData*)otRuby->GetDataPtr(self);
    
//     vector<shared_ptr<MLExperiment> >& experiments = mlData->getExperiments();
//     response.reserve(experiments.size());

//     BOOST_FOREACH(auto& experiment, experiments)
//     {
//         response.push_back(experiment->getY());
//     }
//     return RubyUtils::toValue(response);
// }

// OtInterface::VALUE zenith_mldata_getExperimentIds( VALUE self )
// {
//     vector<double> response;
//     MLData* mlData = (MLData*)otRuby->GetDataPtr(self);

//     vector<shared_ptr<MLExperiment> >& experiments = mlData->getExperiments();
//     response.reserve(experiments.size());

//     BOOST_FOREACH(auto& experiment, experiments)
//     {
//         response.push_back(experiment->getExperimentId());
//     }
//     return RubyUtils::toValue(response);
// }

// OtInterface::VALUE zenith_mldata_setInitialPredictions( VALUE self, VALUE rb_predictions )
// {
//     MLData* mlData = (MLData*)otRuby->GetDataPtr(self);
//     vector<double> predictions;
//     vlcMessage.Write("Setting predictions");
//     RubyUtils::fromValue(rb_predictions, predictions);
//     vlcMessage.Write("Successfully converted to vector");
//     mlData->setInitialPredictions(predictions);
//     vlcMessage.Write("Successfully applied to MLData");
//     return TOtRubyInterface::Qnil;
// }

// OtInterface::VALUE zenith_mldata_setFolds( VALUE self, VALUE rb_folds )
// {
//     MLData* mlData = (MLData*)otRuby->GetDataPtr(self);
//     vector<int> folds = RubyUtils::fromValue<vector<int> >(rb_folds);
//     mlData->setFolds(folds);
//     return TOtRubyInterface::Qnil;
// }

// OtInterface::VALUE zenith_mldata_getFolds( VALUE self )
// {
//     MLData* mlData = (MLData*)otRuby->GetDataPtr(self);
//     return RubyUtils::toValue(mlData->getFolds());
// }
