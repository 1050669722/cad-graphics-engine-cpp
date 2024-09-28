//
// Created by liuxingbo03 on 2022/1/4.
//

#include "pycad/interface/converters/infoDictsConverter/BaseFieldsInJsonInfoDictAndElementInsesConverter.h"


template class BaseFieldsInJsonInfoDictAndElementInsesConverter<PolylineInfoDict>;
template class BaseFieldsInJsonInfoDictAndElementInsesConverter<SplineInfoDict>;
template class BaseFieldsInJsonInfoDictAndElementInsesConverter<ArcInfoDict>;
template class BaseFieldsInJsonInfoDictAndElementInsesConverter<CircleInfoDict>;
template class BaseFieldsInJsonInfoDictAndElementInsesConverter<EllipseInfoDict>;


template <class T>
BaseFieldsInJsonInfoDictAndElementInsesConverter<T>::BaseFieldsInJsonInfoDictAndElementInsesConverter() = default;


template <class T>
BaseFieldsInJsonInfoDictAndElementInsesConverter<T>::~BaseFieldsInJsonInfoDictAndElementInsesConverter() {
    delete doc;
}


template <class T>
void BaseFieldsInJsonInfoDictAndElementInsesConverter<T>::initDoc() {
    doc = new Document();
}


template <class T>
void BaseFieldsInJsonInfoDictAndElementInsesConverter<T>::getId() {
    Value* id{}; if (doc->HasMember("id")) id = &((*doc)["id"]);
    infoDict.id = id->GetString();
}


template <class T>
void BaseFieldsInJsonInfoDictAndElementInsesConverter<T>::getHandle() {
    Value* handle{}; if (doc->HasMember("handle")) handle = &((*doc)["handle"]);
    infoDict.handle = handle->GetString();
}


template <class T>
void BaseFieldsInJsonInfoDictAndElementInsesConverter<T>::getName() {
    Value* name{}; if (doc->HasMember("name")) name = &((*doc)["name"]);
    infoDict.name = name->GetString();
}


template <class T>
void BaseFieldsInJsonInfoDictAndElementInsesConverter<T>::getLayer() {
    Value* layer{}; if (doc->HasMember("layer")) layer = &((*doc)["layer"]);
    infoDict.layer = layer->GetString();
}


template <class T>
void BaseFieldsInJsonInfoDictAndElementInsesConverter<T>::getVisible() {
    Value* visible{}; if (doc->HasMember("visible")) visible = &((*doc)["visible"]);
    infoDict.visible = visible->GetBool();
}


template <class T>
void BaseFieldsInJsonInfoDictAndElementInsesConverter<T>::getPointAnalogue(map<string, double> &pointAnalogueContainer, Value *pointAnalogue) {
    assert(pointAnalogue->IsObject());
    pointAnalogueContainer["x"] = (*pointAnalogue)["x"].GetDouble();
    pointAnalogueContainer["y"] = (*pointAnalogue)["y"].GetDouble();
    pointAnalogueContainer["z"] = (*pointAnalogue)["z"].GetDouble();
}


template <class T>
void BaseFieldsInJsonInfoDictAndElementInsesConverter<T>::getMin() {
    Value* min{}; if (doc->HasMember("min")) min = &((*doc)["min"]);
    getPointAnalogue(infoDict.min, min);
}


template <class T>
void BaseFieldsInJsonInfoDictAndElementInsesConverter<T>::getMax() {
    Value* max{}; if (doc->HasMember("max")) max = &((*doc)["max"]);
    getPointAnalogue(infoDict.max, max);
}


template <class T>
void BaseFieldsInJsonInfoDictAndElementInsesConverter<T>::getXdatas() {
    Value* xdatas{}; if (doc->HasMember("xdatas")) xdatas = &((*doc)["xdatas"]);
    assert(xdatas->IsArray());
    for (auto& xdata : xdatas->GetArray()) {
        infoDict.xdatas.emplace_back(xdata.GetString());
    }
}


template <class T>
void BaseFieldsInJsonInfoDictAndElementInsesConverter<T>::getIsDyn() {
    Value* isDyn{};
    if (doc->HasMember("isDyn")) { //TODO: 项目中，所有的get逻辑应该都需要修改，最好能统一到一个函数中
        isDyn = &((*doc)["isDyn"]);
        infoDict.isDyn = isDyn->GetBool();
    }
    else {
        printf("[WARNING] the \"doc\" do not has member \"isDyn\"\n");
    }
}


template <class T>
void BaseFieldsInJsonInfoDictAndElementInsesConverter<T>::getIsFromHatch() {
    Value* isFromHatch{};
    if (doc->HasMember("isFromHatch")) {
        isFromHatch = &((*doc)["isFromHatch"]);
        infoDict.isFromHatch = isFromHatch->GetBool();
    }
    else {
        printf("[WARNING] the \"doc\" do not has member \"isFromHatch\"\n");
    }
}


// template <class T>
// void BaseFieldsInJsonInfoDictAndElementInsesConverter<T>::childInheritFromBase(BaseFieldsInJsonInfoDictAndElementInses &childInfoDict) const {
//     /*
//      * TODO: 有些字段应该直接取地址
//      */
//     childInfoDict.id = infoDict.id;
//     childInfoDict.handle = infoDict.handle;
//     childInfoDict.name = infoDict.name;
//     childInfoDict.layer = infoDict.layer;
//     childInfoDict.visible = infoDict.visible;
//     childInfoDict.min = infoDict.min;
//     childInfoDict.max = infoDict.max;
//     childInfoDict.xdatas = infoDict.xdatas;
//     childInfoDict.isDyn = infoDict.isDyn;
//     childInfoDict.isFromHatch = infoDict.isFromHatch;
// }


template <class T>
void BaseFieldsInJsonInfoDictAndElementInsesConverter<T>::jsonstr2BaseFieldsInJsonInfoDictAndElementInses(const char *jsonstr) {
    /*
     * 根据jsonstr的构造，
     * 获取其中的数据信息；
     */
    // 实例化Document对象
    initDoc();

    // 填充doc中的内容
    assert(!(doc->Parse(jsonstr).HasParseError())); //TODO: 这一句好像必须得有，因为有Parse

    // id
    getId();

    // handle
    getHandle();

    // name
    getName();

    // layer
    getLayer();

    // visible
    getVisible();

    // min
    getMin();

    // max
    getMax();

    // xdatas
    getXdatas();

    // // isDyn //TODO: 这个字段是在原本的json文件中没有的
    // getIsDyn();

    // // isFromHatch //TODO: 这个字段是在原本的json文件中没有的
    // getIsFromHatch();
}
