//
// Created by liuxingbo03 on 2022/1/5.
//

#include "pycad/interface/converters/infoDictsConverter/PolylineInfoDictConverter.h"


PolylineInfoDictConverter::PolylineInfoDictConverter() = default; //TODO: 这里是否会调用父类的构造函数？


PolylineInfoDictConverter::~PolylineInfoDictConverter() = default; //TODO: 这里是否会调用父类的析构函数？


void PolylineInfoDictConverter::getClosed() {
    Value* closed{}; if (doc->HasMember("closed")) closed = &((*doc)["closed"]);
    infoDict.closed = closed->GetBool();
}


void PolylineInfoDictConverter::getVertexs() {
    Value* vertexs{}; if (doc->HasMember("vertexs")) vertexs = &((*doc)["vertexs"]);
    assert(vertexs->IsArray());
    for (auto& vertex : vertexs->GetArray()) {
        map<string, double> pointAnalogueContainer{};
        getPointAnalogue(pointAnalogueContainer, &vertex);
        pointAnalogueContainer["bulge"] = vertex["bulge"].GetDouble();
        pointAnalogueContainer["startWidth"] = vertex["startWidth"].GetDouble();
        pointAnalogueContainer["endWidth"] = vertex["endWidth"].GetDouble();
        infoDict.vertexs.push_back(pointAnalogueContainer);
    }
}


void PolylineInfoDictConverter::jsonstr2PolylineInfoDict(const char *jsonstr) {
    /*
     * 根据jsonstr的构造，
     * 获取其中的数据信息；
     */
    // // 统计时间
    // // timer t_ogm;
    // clock_t start = clock();

    // // 打印提示信息 //TODO 应该做成装饰模式
    // cout << "[INFO] jsonstr2PolylineInfoDict ..." << endl;

    // jsonstr2BaseFieldsInJsonInfoDictAndElementInses
    jsonstr2BaseFieldsInJsonInfoDictAndElementInses(jsonstr);

    // // childInheritFromBase
    // childInheritFromBase(polylineInfoDict);

    // closed
    getClosed();

    // vertexs
    getVertexs();

    // // 统计时间
    // // double time_cost = t_ogm.elapsed(); //秒
    // double time_cost = (double)(clock() - start) / CLOCKS_PER_SEC; //秒
    // cout << "[INFO] jsonstr2PolylineInfoDict done, " << time_cost << "s has elapsed" << endl; //TODO: 装饰器怎么写出来？
}
