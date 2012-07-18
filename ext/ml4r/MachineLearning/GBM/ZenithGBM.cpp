// #include "MachineLearning/GBM/ZenithGBM.h"
// #include "MachineLearning/GBM/GBMRunner.h"
// #include "MachineLearning/DecisionTree/FeatureInteraction.h"
// #include "MachineLearning/DecisionTree/SplitDefinition.h"
// #include "MachineLearning/MLData/MLData.h"
// #include "MachineLearning/gbm/GBMParameters.h"

// #include "stringConversion.h"
// #include "RubyUtils.h"
// using namespace RubyUtils;



// void zenith_gbm_Free(void* v)
// {
//     delete (reinterpret_cast<GBMRunner*>(v));
// }

// OtInterface::VALUE zenith_gbm_New(int argc, VALUE* argv, VALUE klass)
// {
//     VALUE obj = otRuby->DataWrapStruct(klass, 0, zenith_gbm_Free, 0);
//     otRuby->rb_obj_call_init(obj, argc, argv);
//     return obj;
// }

// OtInterface::VALUE zenith_gbm_Initialize(VALUE self)
// {
//     if (otRuby->GetDataPtr(self)) zenith_gbm_Free(otRuby->GetDataPtr(self));
//     otRuby->SetDataPtr(self, NULL);

//     GBMRunner* gbm = new GBMRunner();
//     if (gbm == NULL) otRuby->rb_sys_fail("ZenithGBM class could not be created");
//     otRuby->SetDataPtr(self, gbm);
//     return self;
// }

// OtInterface::VALUE zenith_gbm_estimate(VALUE self)
// {
//     GBMRunner* gbm = (GBMRunner*)otRuby->GetDataPtr(self);
//     try
//     {
//         gbm->execute();
//     }
//     catch (std::exception e)
//     {
//         vlcMessage.Raise((string("Caught error: ") + e.what()).c_str());
//     }

//     return TOtRubyInterface::Qnil;
// }


// OtInterface::VALUE zenith_gbm_estimateMore(VALUE self, VALUE numTrees)
// {
//     GBMRunner* gbm = (GBMRunner*)otRuby->GetDataPtr(self);
//     try
//     {
//         gbm->estimateMore(RubyUtils::fromValue<int>(numTrees));
//     }
//     catch (std::exception e)
//     {
//         vlcMessage.Raise((string("Caught error: ") + e.what()).c_str());
//     }

//     return TOtRubyInterface::Qnil;
// }


// OtInterface::VALUE zenith_gbm_setFeaturesToRun(VALUE self, VALUE featuresValue)
// {
//     GBMRunner* gbm = (GBMRunner*)otRuby->GetDataPtr(self);
//     gbm->parameters->featuresToRun = RubyUtils::fromValue<vector<string> >(featuresValue);
//     return TOtRubyInterface::Qnil;
// }

// OtInterface::VALUE zenith_gbm_setData(VALUE self, VALUE data)
// {
//     GBMRunner* gbm = (GBMRunner*)otRuby->GetDataPtr(self);
//     MLData* mlData = (MLData*)otRuby->GetDataPtr(data);
//     gbm->setData(mlData);
//     return TOtRubyInterface::Qnil;
// }

// OtInterface::VALUE zenith_gbm_setTryMVariables(VALUE self, VALUE mVariablesValue)
// {
//     GBMRunner* gbm = (GBMRunner*)otRuby->GetDataPtr(self);
//     gbm->parameters->tryMVariables = RubyUtils::fromValue<int>(mVariablesValue);
//     return TOtRubyInterface::Qnil;
// }

// OtInterface::VALUE zenith_gbm_setKTerminalNodes(VALUE self, VALUE kNodesValue)
// {
//     GBMRunner* gbm = (GBMRunner*)otRuby->GetDataPtr(self);
//     gbm->parameters->growKDecisionTreeNodes = RubyUtils::fromValue<int>(kNodesValue);
//     return TOtRubyInterface::Qnil;
// }

// OtInterface::VALUE zenith_gbm_setNumIterations(VALUE self, VALUE numIterationsValue)
// {
//     GBMRunner* gbm = (GBMRunner*)otRuby->GetDataPtr(self);
//     gbm->parameters->numIterations = RubyUtils::fromValue<int>(numIterationsValue);
//     return TOtRubyInterface::Qnil;
// }

// OtInterface::VALUE zenith_gbm_setShrinkageFactor(VALUE self, VALUE shrinkageFactorValue)
// {
//     GBMRunner* gbm = (GBMRunner*)otRuby->GetDataPtr(self);
//     gbm->parameters->shrinkageFactor = RubyUtils::fromValue<double>(shrinkageFactorValue);
//     return TOtRubyInterface::Qnil;
// }

// OtInterface::VALUE zenith_gbm_setBagFraction(VALUE self, VALUE bagFractionValue)
// {
//     GBMRunner* gbm = (GBMRunner*)otRuby->GetDataPtr(self);
//     gbm->parameters->bagFraction = RubyUtils::fromValue<double>(bagFractionValue);
//     return TOtRubyInterface::Qnil;
// }

// OtInterface::VALUE zenith_gbm_setTrainingExperimentIds(VALUE self, VALUE experimentIdsValue)
// {
//     GBMRunner* gbm = (GBMRunner*)otRuby->GetDataPtr(self);
//     gbm->parameters->trainingExperimentIds = RubyUtils::fromValue<vector<int> >(experimentIdsValue);
//     return TOtRubyInterface::Qnil;
// }

// OtInterface::VALUE zenith_gbm_predictions(VALUE self, VALUE newMlData)
// {
//     GBMRunner* gbm = (GBMRunner*)otRuby->GetDataPtr(self);
//     MLData* data = (MLData*)otRuby->GetDataPtr(newMlData);

//     vector<double> predictions;

//     try
//     {
//         predictions = gbm->getPredictions(data);
//     }
//     catch (std::exception e)
//     {
//         vlcMessage.Raise((string("Could not get predictions. Error: ") + e.what()).c_str());
//     }
     
//     return RubyUtils::toValue(predictions);
// }

// OtInterface::VALUE zenith_gbm_training_predictions(VALUE self)
// {
//     GBMRunner* gbm = (GBMRunner*)otRuby->GetDataPtr(self);
//     vector<double> predictions;

//     try
//     {
//         predictions = gbm->getMeanTrainingPredictions();
//     }
//     catch (std::exception e)
//     {
//         vlcMessage.Raise((string("Could not get training predictions. Error: ") + e.what()).c_str());
//     }

//     return RubyUtils::toValue(predictions);
// }

// OtInterface::VALUE zenith_gbm_crossvalidation_predictions(VALUE self)
// {
//     GBMRunner* gbm = (GBMRunner*)otRuby->GetDataPtr(self);
//     vector<double> predictions;

//     try
//     {
//         predictions = gbm->getCrossValidationPredictions();
//     }
//     catch (std::exception e)
//     {
//         vlcMessage.Raise((string("Could not get cross validation predictions. Error: ") + e.what()).c_str());
//     }

//     return RubyUtils::toValue(predictions);
// }

// OtInterface::VALUE zenith_gbm_minObservations(VALUE self, VALUE minObservations)
// {
//     GBMRunner* gbm = (GBMRunner*)otRuby->GetDataPtr(self);
//     gbm->parameters->minObservations = RubyUtils::fromValue<int>(minObservations);
//     return TOtRubyInterface::Qnil;
// }

// OtInterface::VALUE zenith_gbm_setDistribution(VALUE self, VALUE rb_distribution)
// {
//     GBMRunner* gbm = (GBMRunner*)otRuby->GetDataPtr(self);
//     string distribution = stringToLower(RubyUtils::fromValue<string>(rb_distribution));

//     if (distribution == "bernoulli")
//         gbm->parameters->distribution = BERNOULLI;
//     else if (distribution == "gaussian")
//         gbm->parameters->distribution = GAUSSIAN;
//     else
//         throw std::invalid_argument("ZenithGBM::distribution = " + distribution);

//     return TOtRubyInterface::Qnil;
// }

// OtInterface::VALUE zenith_gbm_verbose(VALUE self, VALUE rb_verbose)
// {
//     GBMRunner* gbm = (GBMRunner*)otRuby->GetDataPtr(self);
//     bool verbose = RubyUtils::fromValue<bool>(rb_verbose);
//     gbm->parameters->verbose = verbose;
//     return TOtRubyInterface::Qnil;
// }

// OtInterface::VALUE zenith_gbm_setGreedy( VALUE self, VALUE rb_greedy )
// {
//     GBMRunner* gbm = (GBMRunner*)otRuby->GetDataPtr(self);
//     bool greedy = RubyUtils::fromValue<bool>(rb_greedy);
//     gbm->parameters->greedy = greedy;
//     return TOtRubyInterface::Qnil;
// }

// OtInterface::VALUE zenith_gbm_setRfToLevel( VALUE self, VALUE rb_rfToLevel )
// {
//     GBMRunner* gbm = (GBMRunner*)otRuby->GetDataPtr(self);
//     int rfToLevel = RubyUtils::fromValue<int>(rb_rfToLevel);
//     gbm->parameters->rfToLevel = rfToLevel;
//     return TOtRubyInterface::Qnil;
// }

// OtInterface::VALUE zenith_gbm_capTrees( VALUE self, VALUE rb_cap )
// {
//     GBMRunner* gbm = (GBMRunner*)otRuby->GetDataPtr(self);
//     int cap = RubyUtils::fromValue<int>(rb_cap);
//     gbm->capTrees(cap);
//     return TOtRubyInterface::Qnil;
// }

// OtInterface::VALUE zenith_gbm_setScale( VALUE self, VALUE rb_scale )
// {
//     GBMRunner* gbm = (GBMRunner*)otRuby->GetDataPtr(self);
//     double scale = RubyUtils::fromValue<double>(rb_scale);
//     gbm->parameters->scale = scale;;
//     return TOtRubyInterface::Qnil;
// }

// OtInterface::VALUE splitValueOrCategories(MLData* data, shared_ptr<SplitDefinition> splitDefinition, Partition partition)
// {
//     VALUE returnValue;
//     if (splitDefinition->isCategorical())
//     {
//         // categories as an array
//         if (partition == LHS)
//             returnValue = RubyUtils::toValue(splitDefinition->getLhsCategories());
//         else if (partition == RHS)
//             returnValue = RubyUtils::toValue(splitDefinition->getRhsCategories());
//         else if (partition == MISSING)
//         {
//             set<double> setMissingValue;
//             setMissingValue.insert(data->getMissingValue());
//             returnValue = RubyUtils::toValue(setMissingValue);
//         }
//         else
//             throw std::runtime_error("Primary partition should be either LHS, RHS or MISSING!");
//     }
//     else
//     {
//         // split value as a double 
//         returnValue = RubyUtils::toValue(splitDefinition->getSplitValue());
//     }
//     return returnValue;
// }

// OtInterface::VALUE zenith_gbm_getFeatureInteractions( VALUE self, VALUE howMany )
// {
//     GBMRunner* gbm = (GBMRunner*)otRuby->GetDataPtr(self);
//     vector<FeatureInteraction> featureInteractions;

//     try
//     {
//         featureInteractions = gbm->getFeatureInteractions(RubyUtils::fromValue<int>(howMany));
//     }
//     catch (std::exception e)
//     {
//         vlcMessage.Raise((string("Could not get calculate interactions. Error: ") + e.what()).c_str());
//     }
//     MLData* data = gbm->getData();
//     vector<string> featureNames = data->getFeatures();

//     vector<vector<VALUE> > returnVector;
//     returnVector.reserve(featureInteractions.size());

//     BOOST_FOREACH(auto& interaction, featureInteractions)
//     {
//         vector<VALUE> v;
//         v.reserve(6);

//         // improvement
//         v.push_back(RubyUtils::toValue(interaction.secondarySplitDefinition->getImprovement()));

//         // primary feature name
//         v.push_back(RubyUtils::toValue(featureNames.at(interaction.primarySplitDefinition->getFeatureIndex())));
        
//         // either the split value (as double), or the categories
//         v.push_back(splitValueOrCategories(data, interaction.primarySplitDefinition, interaction.primaryPartition));        
        
//         // the partition chosen
//         Partition p = interaction.primaryPartition;
//         int partition = (p == LHS ? 1 : (p == RHS ? 2 : 3));
//         v.push_back(RubyUtils::toValue(partition));

//         // second feature name
//         v.push_back(RubyUtils::toValue(featureNames.at(interaction.secondarySplitDefinition->getFeatureIndex())));
        
//         // secondary split value / left hand side categories
//         v.push_back(splitValueOrCategories(data, interaction.secondarySplitDefinition, LHS));        

//         // no need for a secondary partition, as all children of the second partition are important
//         returnVector.push_back(v);
//     }
//     return RubyUtils::toValue(returnVector);
// }

