//
// Created by liuxingbo03 on 2022/1/4.
//

#ifndef ATUOCADDATAANALYSISPYCADCPP_BASEFIELDSINJSONINFODICTANDELEMENTINSESCONVERTER_H
#define ATUOCADDATAANALYSISPYCADCPP_BASEFIELDSINJSONINFODICTANDELEMENTINSESCONVERTER_H


#include <iostream>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
// #include <boost/timer.hpp>
#include <ctime>

#include "pycad/Structs.h"


using namespace std;
using namespace rapidjson;
// using namespace boost;


template <class T>
class BaseFieldsInJsonInfoDictAndElementInsesConverter {
    /*
     * 将 BaseFieldsInJsonInfoDictAndElementInses实例的jsonstr 转换为 cpp中的BaseFieldsInJsonInfoDictAndElementInses类
     */
private:
    // 成员属性

    // 成员方法
    void initDoc();
    void getId();
    void getHandle();
    void getName();
    void getLayer();
    void getVisible();
    void getMin();
    void getMax();
    void getXdatas();
    void getIsDyn();
    void getIsFromHatch();

protected:
    // 成员属性
    Document* doc = nullptr;

    // 成员方法
    static void getPointAnalogue(map<string, double>& pointAnalogueContainer, Value* pointAnalogue);
    // void childInheritFromBase(BaseFieldsInJsonInfoDictAndElementInses& childInfoDict) const;

public:
    // 成员属性
    T infoDict{};

    // 成员方法
    BaseFieldsInJsonInfoDictAndElementInsesConverter();
    ~BaseFieldsInJsonInfoDictAndElementInsesConverter();
    void jsonstr2BaseFieldsInJsonInfoDictAndElementInses(const char* jsonstr);
};


#endif //ATUOCADDATAANALYSISPYCADCPP_BASEFIELDSINJSONINFODICTANDELEMENTINSESCONVERTER_H
