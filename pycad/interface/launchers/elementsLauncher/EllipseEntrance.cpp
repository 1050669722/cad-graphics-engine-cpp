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
#include "pycad/interface/converters/infoDictsConverter/EllipseInfoDictConverter.h"
#include "pycad/interface/converters/elementsConverter/EllipseInsConverter.h"


#ifdef __cplusplus
extern "C"
{
#endif


static char* ptr_outputElementIns = nullptr;


XLIB const char* ellipseEntrance(const char* ellipseInfoDictJsonstr) {
    // infoDict
    EllipseInfoDictConverter ellipseInfoDictConverter = EllipseInfoDictConverter();
    ellipseInfoDictConverter.jsonstr2EllipseInfoDict(ellipseInfoDictJsonstr);
    EllipseInfoDict& ellipseInfoDict = ellipseInfoDictConverter.infoDict;

    // // --- for-test -------------------------------------------------
    // printf("id: %s\n", ellipseInfoDict.id.c_str());
    // printf("handle: %s\n", ellipseInfoDict.handle.c_str());
    // printf("name: %s\n", ellipseInfoDict.name.c_str());
    // printf("layer: %s\n", ellipseInfoDict.layer.c_str());
    // printf("visible: %d\n", ellipseInfoDict.visible);
    //
    // printf("min x: %lf\n", ellipseInfoDict.min["x"]);
    // printf("min y: %lf\n", ellipseInfoDict.min["y"]);
    // printf("min z: %lf\n", ellipseInfoDict.min["z"]);
    //
    // printf("max x: %lf\n", ellipseInfoDict.max["x"]);
    // printf("max y: %lf\n", ellipseInfoDict.max["y"]);
    // printf("max z: %lf\n", ellipseInfoDict.max["z"]);
    //
    // for (auto& xdata : ellipseInfoDict.xdatas) {
    //     printf("xdata: %s\n", xdata.c_str());
    // }
    //
    // printf("isDyn: %d\n", ellipseInfoDict.isDyn);
    // printf("isFromHatch: %d\n", ellipseInfoDict.isFromHatch);
    //
    // printf("closed: %d\n", ellipseInfoDict.closed);
    // for (auto& vertex : ellipseInfoDict.vertexs) {
    //     printf("vertex x: %lf\n", vertex["x"]);
    //     printf("vertex y: %lf\n", vertex["y"]);
    //     printf("vertex z: %lf\n", vertex["z"]);
    // }
    // // --------------------------------------------------------------

    // 实例化
    string defaultHandleFromEllipseEntrance = "defaultHandleFromEllipseEntrance";
    map<string, double> defaultOriginFromEllipseEntrance{
            pair<string, double>{"x", 0.0},
            pair<string, double>{"y", 0.0},
            pair<string, double>{"z", 0.0}
    };
    EllipseIns ellipseIns = EllipseIns(ellipseInfoDict, defaultHandleFromEllipseEntrance.c_str(), defaultOriginFromEllipseEntrance);

    // // --- for-test -------------------------------------------------
    // for (auto& atom_x : ellipseIns.coord_x) {
    //     printf("atom_x:\n");
    //     for (auto& x : atom_x) {
    //         printf("x: %lf\n", x);
    //     }
    //     printf("\n");
    // }
    //
    // for (auto& atom_y : ellipseIns.coord_y) {
    //     printf("atom_y:\n");
    //     for (auto& y : atom_y) {
    //         printf("y: %lf\n", y);
    //     }
    //     printf("\n");
    // }
    // // --------------------------------------------------------------

    // ellipseIns 转换为 jsonstr
    EllipseInsConverter ellipseInsConverter = EllipseInsConverter();
    ellipseInsConverter.ellipseIns2Jsonstr(ellipseIns);
    char* outputElementIns = (char*)malloc(sizeof(char) * ellipseInsConverter.outputElementIns.length() + 1);
    strcpy(outputElementIns, ellipseInsConverter.outputElementIns.c_str());
    ptr_outputElementIns = outputElementIns;

    return outputElementIns;
}


XLIB void freeHeapMemoryGeneratedByEllipse() {
    free(ptr_outputElementIns);
}


#ifdef __cplusplus
}
#endif
