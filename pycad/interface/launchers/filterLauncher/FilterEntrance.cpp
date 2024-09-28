//
// Created by liuxingbo03 on 2021/12/21.
//

#ifdef _WIN32
#define XLIB __declspec(dllexport)
#else
#define XLIB
#endif

#include "pycad/filter/Filter.h"

#include "pycad/interface/converters/configConverter/ConfigConverter.h"
#include "pycad/interface/converters/databaseConverter/DatabaseConverter.h"

#ifdef __cplusplus
extern "C"
{
#endif


static char* ptr_outputDatabase = nullptr;


XLIB const char* filterEntrance(const char* configJsonstrForPaths, const char* configJsonstr, const char* databaseJsonstr) {
    // 配置
    ConfigConverter cfgCvter = ConfigConverter();
    cfgCvter.jsonstr2Config(configJsonstrForPaths, configJsonstr); //jsonstr 转换为 config
    Configuration& config = cfgCvter.config;

    // jsonstr2database
    DatabaseConverter dbCvter = DatabaseConverter();
    string jsonstr = databaseJsonstr;
    dbCvter.jsonstr2database(jsonstr);
    Database& database = dbCvter.inputDatabase;

    // 筛选器对象
    Filter filter = Filter(config);
    filter.filterDatabase(database);
    Database& newDatabase = filter.newDatabase;

    // database 转换为 jsonstr
    dbCvter.database2jsonstr(newDatabase);
    char* outputDatabase = (char*)malloc(sizeof(char) * dbCvter.outputDatabase.length() + 1);
    strcpy(outputDatabase, dbCvter.outputDatabase.c_str());
    ptr_outputDatabase = outputDatabase;

    return outputDatabase;
}


XLIB void freeHeapMemoryGeneratedByFilter() {
    free(ptr_outputDatabase);
}


#ifdef __cplusplus
}
#endif
