#ifndef __MLDataReader_h__
#define __MLDataReader_h__

#include <boost/shared_ptr.hpp>
#include <vector>
#include <string>
using std::string;
using std::vector;
using boost::shared_ptr;

class MLData;
class MLDataFields;

class MLDataReader
{
public:
    MLDataReader();
    ~MLDataReader();

    void execute(MLData* mlData);

    string databaseName;
    string tableName;

    shared_ptr<MLDataFields>    fieldsSpec;
    vector<string>              categoricalFeatures;
    double                      missingValue;
    bool                        missingValueDefined;
    void reportOnData(MLData* data, shared_ptr<MLDataFields> fieldsSpec);

protected:
    
    string getSelectSql();

};

#endif // MLDataReader_h__