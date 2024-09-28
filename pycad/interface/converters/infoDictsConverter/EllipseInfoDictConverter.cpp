//
// Created by liuxingbo03 on 2022/1/7.
//

#include "pycad/interface/converters/infoDictsConverter/EllipseInfoDictConverter.h"


EllipseInfoDictConverter::EllipseInfoDictConverter() = default;


EllipseInfoDictConverter::~EllipseInfoDictConverter() = default;


void EllipseInfoDictConverter::getClosed() {
    Value* closed{}; if (doc->HasMember("closed")) closed = &((*doc)["closed"]);
    infoDict.closed = closed->GetBool();
}


void EllipseInfoDictConverter::getCenter() {
    Value* center{}; if (doc->HasMember("center")) center = &((*doc)["center"]);
    getPointAnalogue(infoDict.center, center);
}


void EllipseInfoDictConverter::getStartAngle() {
    Value* startAngle{}; if (doc->HasMember("startAngle")) startAngle = &((*doc)["startAngle"]);
    infoDict.startAngle = startAngle->GetDouble();
}


void EllipseInfoDictConverter::getEndAngle() {
    Value* endAngle{}; if (doc->HasMember("endAngle")) endAngle = &((*doc)["endAngle"]);
    infoDict.endAngle = endAngle->GetDouble();
}


void EllipseInfoDictConverter::getMajorAxis() {
    Value* majorAxis{};
    if (doc->HasMember("majorAxis")) {
        majorAxis = &((*doc)["majorAxis"]);
    } //TODO: 为了兼容字段拼写
    else if (doc->HasMember("maiorAxis")) {
        majorAxis = &((*doc)["maiorAxis"]);
    }
    getPointAnalogue(infoDict.majorAxis, majorAxis);
}


void EllipseInfoDictConverter::getMinorAxis() {
    Value* minorAxis{}; if (doc->HasMember("minorAxis")) minorAxis = &((*doc)["minorAxis"]);
    getPointAnalogue(infoDict.minorAxis, minorAxis);
}


void EllipseInfoDictConverter::getRadiusRatio() {
    Value* radiusRatio{};
    if (doc->HasMember("radiusRatio")) {
        radiusRatio = &((*doc)["radiusRatio"]);
    } //TODO: 为了兼容字段拼写
    else if (doc->HasMember("rediusRatio")) {
        radiusRatio = &((*doc)["rediusRatio"]);
    }
    infoDict.radiusRatio = radiusRatio->GetDouble();
}


void EllipseInfoDictConverter::getNormal() {
    Value* normal{}; if (doc->HasMember("normal")) normal = &((*doc)["normal"]);
    getPointAnalogue(infoDict.normal, normal);
}


void EllipseInfoDictConverter::jsonstr2EllipseInfoDict(const char *jsonstr) {
    /*
     * 根据jsonstr的构造，
     * 获取其中的数据信息；
     */
    // // 统计时间
    // // timer t_ogm;
    // clock_t start = clock();

    // // 打印提示信息 //TODO 应该做成装饰模式
    // cout << "[INFO] jsonstr2EllipseInfoDict ..." << endl;

    // jsonstr2BaseFieldsInJsonInfoDictAndElementInses
    jsonstr2BaseFieldsInJsonInfoDictAndElementInses(jsonstr);

    // closed
    getClosed();

    // center
    getCenter();

    // startAngle
    getStartAngle();

    // endAngle
    getEndAngle();

    // majorAxis
    getMajorAxis();

    // minorAxis
    getMinorAxis();

    // radiusRatio
    getRadiusRatio();

    // normal
    getNormal();

    // // 统计时间
    // // double time_cost = t_ogm.elapsed(); //秒
    // double time_cost = (double)(clock() - start) / CLOCKS_PER_SEC; //秒
    // cout << "[INFO] jsonstr2EllipseInfoDict done, " << time_cost << "s has elapsed" << endl; //TODO: 装饰器怎么写出来？
}
