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
#include "pycad/interface/converters/infoDictsConverter/SplineInfoDictConverter.h"
#include "pycad/interface/converters/elementsConverter/SplineInsConverter.h"


#ifdef __cplusplus
extern "C"
{
#endif


static char* ptr_outputElementIns = nullptr;


XLIB const char* splineEntrance(const char* splineInfoDictJsonstr) {
    // infoDict
    SplineInfoDictConverter splineInfoDictConverter = SplineInfoDictConverter();
    splineInfoDictConverter.jsonstr2SplineInfoDict(splineInfoDictJsonstr);
    SplineInfoDict& splineInfoDict = splineInfoDictConverter.infoDict;

    // // --- for-test -------------------------------------------------
    // printf("id: %s\n", splineInfoDict.id.c_str());
    // printf("handle: %s\n", splineInfoDict.handle.c_str());
    // printf("name: %s\n", splineInfoDict.name.c_str());
    // printf("layer: %s\n", splineInfoDict.layer.c_str());
    // printf("visible: %d\n", splineInfoDict.visible);
    //
    // printf("min x: %lf\n", splineInfoDict.min["x"]);
    // printf("min y: %lf\n", splineInfoDict.min["y"]);
    // printf("min z: %lf\n", splineInfoDict.min["z"]);
    //
    // printf("max x: %lf\n", splineInfoDict.max["x"]);
    // printf("max y: %lf\n", splineInfoDict.max["y"]);
    // printf("max z: %lf\n", splineInfoDict.max["z"]);
    //
    // for (auto& xdata : splineInfoDict.xdatas) {
    //     printf("xdata: %s\n", xdata.c_str());
    // }
    //
    // printf("isDyn: %d\n", splineInfoDict.isDyn);
    // printf("isFromHatch: %d\n", splineInfoDict.isFromHatch);
    //
    // printf("closed: %d\n", splineInfoDict.closed);
    // for (auto& vertex : splineInfoDict.vertexs) {
    //     printf("vertex x: %lf\n", vertex["x"]);
    //     printf("vertex y: %lf\n", vertex["y"]);
    //     printf("vertex z: %lf\n", vertex["z"]);
    // }
    // // --------------------------------------------------------------

    // 实例化
    string defaultHandleFromSplineEntrance = "defaultHandleFromSplineEntrance";
    map<string, double> defaultOriginFromSplineEntrance{
            pair<string, double>{"x", 0.0},
            pair<string, double>{"y", 0.0},
            pair<string, double>{"z", 0.0}
    };
    SplineIns splineIns = SplineIns(splineInfoDict, defaultHandleFromSplineEntrance.c_str(), defaultOriginFromSplineEntrance);

    // // --- for-test -------------------------------------------------
    // for (auto& atom_x : splineIns.coord_x) {
    //     printf("atom_x:\n");
    //     for (auto& x : atom_x) {
    //         printf("x: %lf\n", x);
    //     }
    //     printf("\n");
    // }
    //
    // for (auto& atom_y : splineIns.coord_y) {
    //     printf("atom_y:\n");
    //     for (auto& y : atom_y) {
    //         printf("y: %lf\n", y);
    //     }
    //     printf("\n");
    // }
    // // --------------------------------------------------------------

    // splineIns 转换为 jsonstr
    SplineInsConverter splineInsConverter = SplineInsConverter();
    splineInsConverter.splineIns2Jsonstr(splineIns);
    char* outputElementIns = (char*)malloc(sizeof(char) * splineInsConverter.outputElementIns.length() + 1);
    strcpy(outputElementIns, splineInsConverter.outputElementIns.c_str());
    ptr_outputElementIns = outputElementIns;

    return outputElementIns;
}


XLIB void freeHeapMemoryGeneratedBySpline() {
    free(ptr_outputElementIns);
}


#ifdef __cplusplus
}
#endif
