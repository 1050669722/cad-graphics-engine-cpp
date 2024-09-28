//
// Created by liuxingbo03 on 2022/1/7.
//

#include "pycad/interface/converters/infoDictsConverter/SplineInfoDictConverter.h"


SplineInfoDictConverter::SplineInfoDictConverter() = default;


SplineInfoDictConverter::~SplineInfoDictConverter() = default;


void SplineInfoDictConverter::getClosed() {
    Value* closed{}; if (doc->HasMember("closed")) closed = &((*doc)["closed"]);
    infoDict.closed = closed->GetBool();
}


void SplineInfoDictConverter::getDegree() {
    Value* degree{}; if (doc->HasMember("degree")) degree = &((*doc)["degree"]);
    infoDict.degree = degree->GetInt();
}


void SplineInfoDictConverter::getStartFitTangent() {
    Value* startFitTangent{}; if (doc->HasMember("startFitTangent")) startFitTangent = &((*doc)["startFitTangent"]);
    getPointAnalogue(infoDict.startFitTangent, startFitTangent);
}


void SplineInfoDictConverter::getEndFitTangent() {
    Value* endFitTangent{}; if (doc->HasMember("endFitTangent")) endFitTangent = &((*doc)["endFitTangent"]);
    getPointAnalogue(infoDict.endFitTangent, endFitTangent);
}


void SplineInfoDictConverter::getHasFitData() {
    Value* hasFitData{}; if (doc->HasMember("hasFitData")) hasFitData = &((*doc)["hasFitData"]);
    infoDict.hasFitData = hasFitData->GetBool();
}


void SplineInfoDictConverter::getIsRational() {
    Value* isRational{}; if (doc->HasMember("isRational")) isRational = &((*doc)["isRational"]);
    infoDict.isRational = isRational->GetBool();
}


void SplineInfoDictConverter::getControlPointNub() {
    Value* controlPointNub{}; if (doc->HasMember("controlPointNub")) controlPointNub = &((*doc)["controlPointNub"]);
    infoDict.controlPointNub = controlPointNub->GetInt();
}


void SplineInfoDictConverter::getControlPoints() {
    Value* controlPoints{}; if (doc->HasMember("controlPoints")) controlPoints = &((*doc)["controlPoints"]);
    assert(controlPoints->IsArray());
    for (auto& controlPoint : controlPoints->GetArray()) {
        map<string, double> pointAnalogueContainer{};
        getPointAnalogue(pointAnalogueContainer, &controlPoint);
        infoDict.controlPoints.push_back(pointAnalogueContainer);
    }
}


void SplineInfoDictConverter::getFitPointNub() {
    Value* fitPointNub{}; if (doc->HasMember("fitPointNub")) fitPointNub = &((*doc)["fitPointNub"]);
    infoDict.fitPointNub = fitPointNub->GetInt();
}


void SplineInfoDictConverter::getFitPoints() {
    Value* fitPoints{}; if (doc->HasMember("fitPoints")) fitPoints = &((*doc)["fitPoints"]);
    assert(fitPoints->IsArray());
    for (auto& fitPoint : fitPoints->GetArray()) {
        map<string, double> pointAnalogueContainer{};
        getPointAnalogue(pointAnalogueContainer, &fitPoint);
        infoDict.fitPoints.push_back(pointAnalogueContainer);
    }
}


void SplineInfoDictConverter::jsonstr2SplineInfoDict(const char *jsonstr) {
    /*
     * 根据jsonstr的构造，
     * 获取其中的数据信息；
     */
    // // 统计时间
    // // timer t_ogm;
    // clock_t start = clock();

    // // 打印提示信息 //TODO 应该做成装饰模式
    // cout << "[INFO] jsonstr2SplineInfoDict ..." << endl;

    // jsonstr2BaseFieldsInJsonInfoDictAndElementInses
    jsonstr2BaseFieldsInJsonInfoDictAndElementInses(jsonstr);

    // closed
    getClosed();

    // degree
    getDegree();

    // startFitTangent
    getStartFitTangent();

    // endFitTangent
    getEndFitTangent();

    // hasFitData
    getHasFitData();

    // isRational
    getIsRational();

    // controlPointNub
    getControlPointNub();

    // controlPoints
    getControlPoints();

    // fitPointNub
    getFitPointNub();

    // fitPoints
    getFitPoints();

    // // 统计时间
    // // double time_cost = t_ogm.elapsed(); //秒
    // double time_cost = (double)(clock() - start) / CLOCKS_PER_SEC; //秒
    // cout << "[INFO] jsonstr2SplineInfoDict done, " << time_cost << "s has elapsed" << endl; //TODO: 装饰器怎么写出来？
}
