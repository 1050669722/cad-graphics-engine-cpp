//
// Created by liuxingbo03 on 2022/1/7.
//

#include "pycad/interface/converters/infoDictsConverter/ArcInfoDictConverter.h"


ArcInfoDictConverter::ArcInfoDictConverter() = default;


ArcInfoDictConverter::~ArcInfoDictConverter() = default;


void ArcInfoDictConverter::getClosed() {
    Value* closed{}; if (doc->HasMember("closed")) closed = &((*doc)["closed"]);
    infoDict.closed = closed->GetBool();
}


void ArcInfoDictConverter::getCenter() {
    Value* center{}; if (doc->HasMember("center")) center = &((*doc)["center"]);
    getPointAnalogue(infoDict.center, center);
}


void ArcInfoDictConverter::getRadius() {
    Value* radius{}; if (doc->HasMember("radius")) radius = &((*doc)["radius"]);
    infoDict.radius = radius->GetDouble();
}


void ArcInfoDictConverter::getStartPoint() {
    Value* startPoint{}; if (doc->HasMember("startPoint")) startPoint = &((*doc)["startPoint"]);
    getPointAnalogue(infoDict.startPoint, startPoint);
}


void ArcInfoDictConverter::getEndPoint() {
    Value* endPoint{}; if (doc->HasMember("endPoint")) endPoint = &((*doc)["endPoint"]);
    getPointAnalogue(infoDict.endPoint, endPoint);
}


void ArcInfoDictConverter::getNormal() {
    Value* normal{}; if (doc->HasMember("normal")) normal = &((*doc)["normal"]);
    getPointAnalogue(infoDict.normal, normal);
}


void ArcInfoDictConverter::jsonstr2ArcInfoDict(const char *jsonstr) {
    /*
     * 根据jsonstr的构造，
     * 获取其中的数据信息；
     */
    // // 统计时间
    // // timer t_ogm;
    // clock_t start = clock();

    // // 打印提示信息 //TODO 应该做成装饰模式
    // cout << "[INFO] jsonstr2ArcInfoDict ..." << endl;

    // jsonstr2BaseFieldsInJsonInfoDictAndElementInses
    jsonstr2BaseFieldsInJsonInfoDictAndElementInses(jsonstr);

    // closed
    getClosed();

    // center
    getCenter();

    // radius
    getRadius();

    // startPoint
    getStartPoint();

    // endPoint
    getEndPoint();

    // normal
    getNormal();

    // // 统计时间
    // // double time_cost = t_ogm.elapsed(); //秒
    // double time_cost = (double)(clock() - start) / CLOCKS_PER_SEC; //秒
    // cout << "[INFO] jsonstr2ArcInfoDict done, " << time_cost << "s has elapsed" << endl; //TODO: 装饰器怎么写出来？
}
