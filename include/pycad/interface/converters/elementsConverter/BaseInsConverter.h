//
// Created by liuxingbo03 on 2022/1/6.
//

#ifndef ATUOCADDATAANALYSISPYCADCPP_BASEINSCONVERTER_H
#define ATUOCADDATAANALYSISPYCADCPP_BASEINSCONVERTER_H


#include <iostream>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
// #include <boost/timer.hpp>
#include <ctime>

#include "pycad/Structs.h"
#include "pycad/parse/Elements.h"


using namespace std;
using namespace rapidjson;
// using namespace boost;


template <class T>
class BaseInsConverter {
    /*
     * 将 BaseIns实例的jsonstr 转换为 cpp中的BaseIns类
     * 或者反过来
     */
private:
    // 成员属性

    // 成员方法
    void initDocOutput();
    void sendAtom(Value& dstArrayAtom, Atom& srcAtom);
    void sendCoord(Value& dstArrayCoord, Coord& srcCoord);
    void mntCoord(const char* fieldName, Coord &srcCoord);

protected:
    // 成员属性
    Document* docOutput = nullptr;
    Document::AllocatorType* allocator = nullptr;

    // 成员方法

public:
    // 成员属性
    string outputElementIns{};

    // 成员方法
    BaseInsConverter();
    ~BaseInsConverter();
    void baseIns2Jsonstr(T& elementIns);
};


#endif //ATUOCADDATAANALYSISPYCADCPP_BASEINSCONVERTER_H
