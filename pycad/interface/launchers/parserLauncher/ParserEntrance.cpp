//
// Created by liuxingbo03 on 2021/12/1.
//

#ifdef _WIN32
#define XLIB __declspec(dllexport)
#else
#define XLIB
#endif

// #include "nlohmann/json.hpp"

#include "pycad/Configuration.h"
#include "pycad/tools/BLockTools.h"
#include "pycad/preprocess/Preprocessor.h"
#include "pycad/parse/Tree.h"

// #include "pycad/filter/Filter.h"
// #include "pycad/plot/Painter.h"

#include "pycad/interface/converters/configConverter/ConfigConverter.h"
#include "pycad/interface/converters/databaseConverter/DatabaseConverter.h"


// using json = nlohmann::json;


#ifdef __cplusplus
extern "C"
{
#endif


static char* ptr_outputDatabase = nullptr;


XLIB const char* parserEntrance(const char* configJsonstrForPaths, const char* configJsonstr) {
    // 配置文件指针
    ConfigConverter cfgCvter = ConfigConverter();
    cfgCvter.jsonstr2Config(configJsonstrForPaths, configJsonstr); //jsonstr 转换为 config
    Configuration& config = cfgCvter.config;
    Configuration* ptr_config = &config;

    // 根 rootBlock
    BLockTools blockTools = BLockTools(config.jsonPathFromPython);
    BLock* ptr_rootBlock = blockTools.rootBlock;

    // 预处理
    Preprocessor preprocessor = Preprocessor(config, blockTools);
    preprocessor.preprocess();

    // // for-test //TODO: 根据judgements["layerHistory"]["wanted"] 因图像是正确的 故文字是正确的 | 但是这里不能在console上打印出正确的文字
    // for (auto& crime : config.judgements["BRHandleHistory"]["blackList"]) {
    //     cout << crime << endl;
    // }

    // database //出于语言特性和运行效率的考量，这里将Tree的实例封装在Tree::blossom里 //TODO: 这只是个示例，真正封装的时候，应该将Tree::bloom中的*(tree.database)取出来直接输入到Painter中去； //Tree的准备工作太多，如果在这里实例化，会比较累赘
    Database database = Database{};
    Database* ptr_database = &database;
    Tree::bloom(ptr_database, ptr_config, ptr_rootBlock); //TODO: 是否省去Tree的返回值Database而作为属性？

    // database 转换为 jsonstr
    DatabaseConverter dbCvter = DatabaseConverter();
    dbCvter.database2jsonstr(database);
    char* outputDatabase = (char*)malloc(sizeof(char) * dbCvter.outputDatabase.length() + 1);
    strcpy(outputDatabase, dbCvter.outputDatabase.c_str());
    ptr_outputDatabase = outputDatabase;

    return outputDatabase;

    // // // 筛选器对象
    // Filter filter = Filter(config);
    // filter.filterDatabase(database);
    //
    // // 绘画器对象
    // // Painter painter = Painter(config, database);
    // Painter painter = Painter(config, filter.newDatabase);
    // painter.paint(); //TODO: 这只是个示例，真正封装的时候，应该将painter.screen返回
    // Mat& img = painter.screen;
}


XLIB void freeHeapMemoryGeneratedByParser() {
    // free(charPointer);
    free(ptr_outputDatabase);
    // cout << "charPointer: " << &charPointer[0] << endl;
    // printf("charPointer: %x", &charPointer[0]);
    // printf("p_for_free: %x", p_for_free);
    // cout << "content of charPointer: " << *charPointer << endl;
}


#ifdef __cplusplus
}
#endif
