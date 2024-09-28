//
// Created by liuxingbo03 on 2021/12/22.
//

#ifdef _WIN32
#define XLIB __declspec(dllexport)
#else
#define XLIB
#endif

#include "pycad/plot/Painter.h"

#include "pycad/interface/converters/configConverter/ConfigConverter.h"
#include "pycad/interface/converters/databaseConverter/DatabaseConverter.h"

#ifdef __cplusplus
extern "C"
{
#endif


static int* ptr_outputImgHeight = nullptr; //TODO: 如果是static的堆内存会带来什么问题吗？
static int* ptr_outputImgWidth = nullptr;
static uchar* ptr_outputImg = nullptr;


XLIB uchar* painterEntrance(const char* configJsonstrForPaths, const char* configJsonstr, const char* databaseJsonstr) {
    // 配置
    ConfigConverter cfgCvter = ConfigConverter();
    cfgCvter.jsonstr2Config(configJsonstrForPaths, configJsonstr); //jsonstr 转换为 config
    Configuration& config = cfgCvter.config;

    // jsonstr2database
    DatabaseConverter dbCvter = DatabaseConverter();
    string jsonstr = databaseJsonstr;
    dbCvter.jsonstr2database(jsonstr);
    Database& database = dbCvter.inputDatabase;

    // 绘画器对象
    Painter painter = Painter(config, database);
    painter.paint(); //TODO: 这只是个示例，真正封装的时候，应该将painter.screen返回
    Mat& img = painter.screen;

    // height 转换为 int*
    int* outputImgHeight = (int*)malloc(sizeof(int) * 1);
    memcpy(outputImgHeight, &(img.rows), sizeof(int) * 1);
    ptr_outputImgHeight = outputImgHeight;

    // width 转换为 int*
    int* outputImgWidth = (int*)malloc(sizeof(int) * 1);
    memcpy(outputImgWidth, &(img.cols), sizeof(int) * 1);
    ptr_outputImgWidth = outputImgWidth;

    // img 转换为 uchar*
    auto* outputImg = (uchar*)malloc(sizeof(uchar) * img.rows * img.cols * 3);
    memcpy(outputImg, img.data, sizeof(uchar) * img.rows * img.cols * 3);
    ptr_outputImg = outputImg;

    return outputImg;
}


XLIB int getOutputImgHeight() {
    return *ptr_outputImgHeight;
}


XLIB int getOutputImgWidth() {
    return *ptr_outputImgWidth;
}


XLIB void freeHeapMemoryGeneratedByPainter() {
    free(ptr_outputImg);
    free(ptr_outputImgHeight);
    free(ptr_outputImgWidth);
}


#ifdef __cplusplus
}
#endif
