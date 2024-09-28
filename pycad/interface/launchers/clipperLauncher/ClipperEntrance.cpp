//
// Created by liuxingbo03 on 2021/12/28.
//

#ifdef _WIN32
#define XLIB __declspec(dllexport)
#else
#define XLIB
#endif

#include "pycad/filter/Filter.h"
#include "pycad/clip/Clipper.h"

#include "pycad/interface/converters/configConverter/ConfigConverter.h"
#include "pycad/interface/converters/databaseConverter/DatabaseConverter.h"

#ifdef __cplusplus
extern "C"
{
#endif


static char* ptr_outputDatabase = nullptr;


XLIB const char* clipperEntrance(const char* configJsonstrForPaths, const char* configJsonstr, const char* databaseJsonstr) {
    // 配置
    ConfigConverter cfgCvter = ConfigConverter();
    cfgCvter.jsonstr2Config(configJsonstrForPaths, configJsonstr); //jsonstr 转换为 config
    Configuration& config = cfgCvter.config;

    // jsonstr2database
    DatabaseConverter dbCvter = DatabaseConverter();
    string jsonstr = databaseJsonstr;
    dbCvter.jsonstr2database(jsonstr);
    Database& database = dbCvter.inputDatabase;

    // 区域筛选器对象
    Clipper clipper = Clipper(config);
    clipper.clipDatabase(database);
    Database& newDatabase = clipper.newDatabase;

    // database 转换为 jsonstr
    dbCvter.database2jsonstr(newDatabase);
    char* outputDatabase = (char*)malloc(sizeof(char) * dbCvter.outputDatabase.length() + 1);
    strcpy(outputDatabase, dbCvter.outputDatabase.c_str());
    ptr_outputDatabase = outputDatabase;

    return outputDatabase;
}


XLIB void freeHeapMemoryGeneratedByClipper() {
    free(ptr_outputDatabase);
}


#ifdef __cplusplus
}
#endif
