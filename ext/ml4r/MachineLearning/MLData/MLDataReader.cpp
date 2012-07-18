#include "MachineLearning/MLData/MLDataReader.h"
#include "MachineLearning/MLData/MLDataFields.h"
#include "MachineLearning/MLData/MLData.h"
#include "MachineLearning/MLExperiment.h"

#include <sstream>
#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>
#include <stdexcept>
#include <boost/lexical_cast.hpp>
using std::runtime_error;
using boost::make_shared;
using std::ostringstream;
using boost::lexical_cast;
#include "sqlite3.h"
#include "utils/VlcMessage.h"

MLDataReader::MLDataReader()
{
    fieldsSpec = make_shared<MLDataFields>();
    missingValueDefined = false;
}

MLDataReader::~MLDataReader()
{

}

void MLDataReader::execute(MLData* mlData)
{
    vlcMessage.Begin("Input (sqlite3)");
    if (tableName.empty())
        throw runtime_error("tableName not specified for sqlite3 database!");

    sqlite3 *database;
    sqlite3_open(databaseName.c_str(), &database);

    int experimentIndex    = -1;

    sqlite3_stmt *statement;

    vector<string> featuresToLoad = fieldsSpec->featuresFields;

    vector<shared_ptr<MLExperiment> > experiments;
    
    int attributesStartIndex    =  1;
    int weightIndex             = -1;
    int initialPredictionsIndex = -1;
    int responseIndex           = -1;

    if (!fieldsSpec->actualYField.empty())               responseIndex             = attributesStartIndex++;
    if (!fieldsSpec->weightsField.empty())               weightIndex               = attributesStartIndex++; 
    if (!fieldsSpec->initialPredictionsField.empty())    initialPredictionsIndex   = attributesStartIndex++;
    
    string sql = getSelectSql();

    vector<double> features(featuresToLoad.size());

    if(sqlite3_prepare_v2(database, sql.c_str(), -1, &statement, 0) != SQLITE_OK)
        throw runtime_error("Couldn't prepare sql: " + sql);

    while(true)
    {
        if (sqlite3_step(statement) != SQLITE_ROW) break;

        int     experimentId        = sqlite3_column_int(statement, 0);
        
        double  yValue              = responseIndex != -1 ?           sqlite3_column_double(statement, 1) : 0.0;
        double  weight              = weightIndex != -1 ?             sqlite3_column_double(statement, weightIndex) : 1.0;
        double  initialPrediction   = initialPredictionsIndex != -1 ? sqlite3_column_double(statement, initialPredictionsIndex) : -1;

        if (responseIndex != -1 && missingValueDefined && yValue == missingValue)
            continue;

        int index = attributesStartIndex;
        BOOST_FOREACH(double& f, features)
        {
            f = sqlite3_column_double(statement, index++);
            // attributes.at(i) = q.GetDouble(f_attributes.at(i));
        }

        // create an MLExperiment
        ++experimentIndex;
        shared_ptr<MLExperiment> experiment = 
            shared_ptr<MLExperiment>(new MLExperiment(experimentId, experimentIndex, yValue, initialPrediction, weight, features));

        experiments.push_back(experiment);
    }

    sqlite3_finalize(statement);
    string error = sqlite3_errmsg(database);
    if (error != "not an error") 
        vlcMessage.Write("Error: " + sql + "\n" + error, 1);

    // finally, set up our MLData object
    mlData->setExperiments(experiments);
    mlData->setFeatures(featuresToLoad);
    mlData->constructCategories(categoricalFeatures);
    mlData->setInitialPredictionsDefined(initialPredictionsIndex != -1);

    if (missingValueDefined)
        mlData->setMissingValue(missingValue);

    reportOnData(mlData, fieldsSpec);

    vlcMessage.End();
}

string MLDataReader::getSelectSql()
{
    ostringstream str;
    str << "SELECT "    << "tbl.'" << fieldsSpec->experimentIdField << "'";

    if (fieldsSpec->actualYField != "")
        str << ", tbl.'" << fieldsSpec->actualYField      << "'";
    
    if (fieldsSpec->weightsField != "")
        str << ", tbl.'" << fieldsSpec->weightsField << "'";
    
    if (fieldsSpec->initialPredictionsField != "")
        str << ", tbl.'" << fieldsSpec->initialPredictionsField << "'";
    
    vector<string> featuresToLoad = fieldsSpec->featuresFields;
    
    for (unsigned int i = 0; i < featuresToLoad.size(); ++i)
        str << ", tbl.'" << featuresToLoad.at(i) << "'";
    
    str << " FROM '" << tableName << "' tbl";
    str << " ORDER BY tbl.'" << fieldsSpec->experimentIdField << "'";
    return str.str();
}

void MLDataReader::reportOnData(MLData* data, shared_ptr<MLDataFields> fieldsSpec)
{
    vector<shared_ptr<MLExperiment> > experiments = data->getExperiments();
    vlcMessage.Write("Successfully read " + lexical_cast<string>(experiments.size()) + " experiments");
    vlcMessage.Write("Loaded " + lexical_cast<string>(fieldsSpec->featuresFields.size()) + " fields");
}

