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
#include "pycad/interface/converters/infoDictsConverter/ArcInfoDictConverter.h"
#include "pycad/interface/converters/elementsConverter/ArcInsConverter.h"


#ifdef __cplusplus
extern "C"
{
#endif


static char* ptr_outputElementIns = nullptr;


XLIB const char* arcEntrance(const char* arcInfoDictJsonstr) {
    // infoDict
    ArcInfoDictConverter arcInfoDictConverter = ArcInfoDictConverter();
    arcInfoDictConverter.jsonstr2ArcInfoDict(arcInfoDictJsonstr);
    ArcInfoDict& arcInfoDict = arcInfoDictConverter.infoDict;

    // // --- for-test -------------------------------------------------
    // printf("id: %s\n", arcInfoDict.id.c_str());
    // printf("handle: %s\n", arcInfoDict.handle.c_str());
    // printf("name: %s\n", arcInfoDict.name.c_str());
    // printf("layer: %s\n", arcInfoDict.layer.c_str());
    // printf("visible: %d\n", arcInfoDict.visible);
    //
    // printf("min x: %lf\n", arcInfoDict.min["x"]);
    // printf("min y: %lf\n", arcInfoDict.min["y"]);
    // printf("min z: %lf\n", arcInfoDict.min["z"]);
    //
    // printf("max x: %lf\n", arcInfoDict.max["x"]);
    // printf("max y: %lf\n", arcInfoDict.max["y"]);
    // printf("max z: %lf\n", arcInfoDict.max["z"]);
    //
    // for (auto& xdata : arcInfoDict.xdatas) {
    //     printf("xdata: %s\n", xdata.c_str());
    // }
    //
    // printf("isDyn: %d\n", arcInfoDict.isDyn);
    // printf("isFromHatch: %d\n", arcInfoDict.isFromHatch);
    //
    // printf("closed: %d\n", arcInfoDict.closed);
    // for (auto& vertex : arcInfoDict.vertexs) {
    //     printf("vertex x: %lf\n", vertex["x"]);
    //     printf("vertex y: %lf\n", vertex["y"]);
    //     printf("vertex z: %lf\n", vertex["z"]);
    // }
    // // --------------------------------------------------------------

    // 实例化
    string defaultHandleFromArcEntrance = "defaultHandleFromArcEntrance";
    map<string, double> defaultOriginFromArcEntrance{
            pair<string, double>{"x", 0.0},
            pair<string, double>{"y", 0.0},
            pair<string, double>{"z", 0.0}
    };
    ArcIns arcIns = ArcIns(arcInfoDict, defaultHandleFromArcEntrance.c_str(), defaultOriginFromArcEntrance);

    // // --- for-test -------------------------------------------------
    // for (auto& atom_x : arcIns.coord_x) {
    //     printf("atom_x:\n");
    //     for (auto& x : atom_x) {
    //         printf("x: %lf\n", x);
    //     }
    //     printf("\n");
    // }
    //
    // for (auto& atom_y : arcIns.coord_y) {
    //     printf("atom_y:\n");
    //     for (auto& y : atom_y) {
    //         printf("y: %lf\n", y);
    //     }
    //     printf("\n");
    // }
    // // --------------------------------------------------------------

    // arcIns 转换为 jsonstr
    ArcInsConverter arcInsConverter = ArcInsConverter();
    arcInsConverter.arcIns2Jsonstr(arcIns);
    char* outputElementIns = (char*)malloc(sizeof(char) * arcInsConverter.outputElementIns.length() + 1);
    strcpy(outputElementIns, arcInsConverter.outputElementIns.c_str());
    ptr_outputElementIns = outputElementIns;

    return outputElementIns;
}


XLIB void freeHeapMemoryGeneratedByArc() {
    free(ptr_outputElementIns);
}


#ifdef __cplusplus
}
#endif
