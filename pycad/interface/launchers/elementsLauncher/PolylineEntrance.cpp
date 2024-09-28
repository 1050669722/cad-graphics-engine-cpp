//
// Created by liuxingbo03 on 2022/1/4.
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
#include "pycad/interface/converters/infoDictsConverter/PolylineInfoDictConverter.h"
#include "pycad/interface/converters/elementsConverter/PolylineInsConverter.h"


#ifdef __cplusplus
extern "C"
{
#endif


static char* ptr_outputElementIns = nullptr;


XLIB const char* polylineEntrance(const char* polylineInfoDictJsonstr) {
    // infoDict
    PolylineInfoDictConverter polylineInfoDictConverter = PolylineInfoDictConverter();
    polylineInfoDictConverter.jsonstr2PolylineInfoDict(polylineInfoDictJsonstr);
    PolylineInfoDict& polylineInfoDict = polylineInfoDictConverter.infoDict;

    // // --- for-test -------------------------------------------------
    // printf("id: %s\n", polylineInfoDict.id.c_str());
    // printf("handle: %s\n", polylineInfoDict.handle.c_str());
    // printf("name: %s\n", polylineInfoDict.name.c_str());
    // printf("layer: %s\n", polylineInfoDict.layer.c_str());
    // printf("visible: %d\n", polylineInfoDict.visible);
    //
    // printf("min x: %lf\n", polylineInfoDict.min["x"]);
    // printf("min y: %lf\n", polylineInfoDict.min["y"]);
    // printf("min z: %lf\n", polylineInfoDict.min["z"]);
    //
    // printf("max x: %lf\n", polylineInfoDict.max["x"]);
    // printf("max y: %lf\n", polylineInfoDict.max["y"]);
    // printf("max z: %lf\n", polylineInfoDict.max["z"]);
    //
    // for (auto& xdata : polylineInfoDict.xdatas) {
    //     printf("xdata: %s\n", xdata.c_str());
    // }
    //
    // printf("isDyn: %d\n", polylineInfoDict.isDyn);
    // printf("isFromHatch: %d\n", polylineInfoDict.isFromHatch);
    //
    // printf("closed: %d\n", polylineInfoDict.closed);
    // for (auto& vertex : polylineInfoDict.vertexs) {
    //     printf("vertex x: %lf\n", vertex["x"]);
    //     printf("vertex y: %lf\n", vertex["y"]);
    //     printf("vertex z: %lf\n", vertex["z"]);
    // }
    // // --------------------------------------------------------------

    // 实例化
    string defaultHandleFromPolylineEntrance = "defaultHandleFromPolylineEntrance";
    map<string, double> defaultOriginFromPolylineEntrance{
            pair<string, double>{"x", 0.0},
            pair<string, double>{"y", 0.0},
            pair<string, double>{"z", 0.0}
    };
    PolylineIns polylineIns = PolylineIns(polylineInfoDict, defaultHandleFromPolylineEntrance.c_str(), defaultOriginFromPolylineEntrance);

    // // --- for-test -------------------------------------------------
    // for (auto& atom_x : polylineIns.coord_x) {
    //     printf("atom_x:\n");
    //     for (auto& x : atom_x) {
    //         printf("x: %lf\n", x);
    //     }
    //     printf("\n");
    // }
    //
    // for (auto& atom_y : polylineIns.coord_y) {
    //     printf("atom_y:\n");
    //     for (auto& y : atom_y) {
    //         printf("y: %lf\n", y);
    //     }
    //     printf("\n");
    // }
    // // --------------------------------------------------------------

    // polylineIns 转换为 jsonstr
    PolylineInsConverter polylineInsConverter = PolylineInsConverter();
    polylineInsConverter.polylineIns2Jsonstr(polylineIns);
    char* outputElementIns = (char*)malloc(sizeof(char) * polylineInsConverter.outputElementIns.length() + 1);
    strcpy(outputElementIns, polylineInsConverter.outputElementIns.c_str());
    ptr_outputElementIns = outputElementIns;

    return outputElementIns;
}


XLIB void freeHeapMemoryGeneratedByPolyline() {
    free(ptr_outputElementIns);
}


#ifdef __cplusplus
}
#endif
