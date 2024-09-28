//
// Created by liuxingbo03 on 2022/1/7.
//

#ifdef _WIN32
#define XLIB __declspec(dllexport)
#else
#define XLIB
#endif

// #include "nlohmann/json.hpp"

// #include "pycad/Configuration.h"
// #include "pycad/tools/BLockTools.h"
// #include "pycad/preprocess/Preprocessor.h"
// #include "pycad/parse/Tree.h"

// #include "pycad/filter/Filter.h"
// #include "pycad/plot/Painter.h"

// #include "pycad/interface/configConverter/ConfigConverter.h"
// #include "pycad/interface/databaseConverter/DatabaseConverter.h"


#include "pycad/Structs.h"
#include "pycad/parse/Elements.h"
#include "pycad/interface/converters/infoDictsConverter/CircleInfoDictConverter.h"
#include "pycad/interface/converters/elementsConverter/CircleInsConverter.h"


#ifdef __cplusplus
extern "C"
{
#endif


static char* ptr_outputElementIns = nullptr;


XLIB const char* circleEntrance(const char* circleInfoDictJsonstr) {
    // infoDict
    CircleInfoDictConverter circleInfoDictConverter = CircleInfoDictConverter();
    circleInfoDictConverter.jsonstr2CircleInfoDict(circleInfoDictJsonstr);
    CircleInfoDict& circleInfoDict = circleInfoDictConverter.infoDict;

    // // --- for-test -------------------------------------------------
    // printf("id: %s\n", circleInfoDict.id.c_str());
    // printf("handle: %s\n", circleInfoDict.handle.c_str());
    // printf("name: %s\n", circleInfoDict.name.c_str());
    // printf("layer: %s\n", circleInfoDict.layer.c_str());
    // printf("visible: %d\n", circleInfoDict.visible);
    //
    // printf("min x: %lf\n", circleInfoDict.min["x"]);
    // printf("min y: %lf\n", circleInfoDict.min["y"]);
    // printf("min z: %lf\n", circleInfoDict.min["z"]);
    //
    // printf("max x: %lf\n", circleInfoDict.max["x"]);
    // printf("max y: %lf\n", circleInfoDict.max["y"]);
    // printf("max z: %lf\n", circleInfoDict.max["z"]);
    //
    // for (auto& xdata : circleInfoDict.xdatas) {
    //     printf("xdata: %s\n", xdata.c_str());
    // }
    //
    // printf("isDyn: %d\n", circleInfoDict.isDyn);
    // printf("isFromHatch: %d\n", circleInfoDict.isFromHatch);
    //
    // printf("closed: %d\n", circleInfoDict.closed);
    // for (auto& vertex : circleInfoDict.vertexs) {
    //     printf("vertex x: %lf\n", vertex["x"]);
    //     printf("vertex y: %lf\n", vertex["y"]);
    //     printf("vertex z: %lf\n", vertex["z"]);
    // }
    // // --------------------------------------------------------------

    // 实例化
    string defaultHandleFromCircleEntrance = "defaultHandleFromCircleEntrance";
    map<string, double> defaultOriginFromCircleEntrance{
            pair<string, double>{"x", 0.0},
            pair<string, double>{"y", 0.0},
            pair<string, double>{"z", 0.0}
    };
    CircleIns circleIns = CircleIns(circleInfoDict, defaultHandleFromCircleEntrance.c_str(), defaultOriginFromCircleEntrance);

    // // --- for-test -------------------------------------------------
    // for (auto& atom_x : circleIns.coord_x) {
    //     printf("atom_x:\n");
    //     for (auto& x : atom_x) {
    //         printf("x: %lf\n", x);
    //     }
    //     printf("\n");
    // }
    //
    // for (auto& atom_y : circleIns.coord_y) {
    //     printf("atom_y:\n");
    //     for (auto& y : atom_y) {
    //         printf("y: %lf\n", y);
    //     }
    //     printf("\n");
    // }
    // // --------------------------------------------------------------

    // circleIns 转换为 jsonstr
    CircleInsConverter circleInsConverter = CircleInsConverter();
    circleInsConverter.circleIns2Jsonstr(circleIns);
    char* outputElementIns = (char*)malloc(sizeof(char) * circleInsConverter.outputElementIns.length() + 1);
    strcpy(outputElementIns, circleInsConverter.outputElementIns.c_str());
    ptr_outputElementIns = outputElementIns;

    return outputElementIns;
}


XLIB void freeHeapMemoryGeneratedByCircle() {
    free(ptr_outputElementIns);
}


#ifdef __cplusplus
}
#endif
