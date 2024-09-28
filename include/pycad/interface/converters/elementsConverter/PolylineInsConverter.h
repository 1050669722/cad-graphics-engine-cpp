//
// Created by liuxingbo03 on 2022/1/6.
//

#ifndef ATUOCADDATAANALYSISPYCADCPP_POLYLINEINSCONVERTER_H
#define ATUOCADDATAANALYSISPYCADCPP_POLYLINEINSCONVERTER_H


#include "pycad/interface/converters/elementsConverter/BaseInsConverter.h"


class PolylineInsConverter : public BaseInsConverter<PolylineIns> {
    /*
     * 将 PolylineIns实例的jsonstr 转换为 cpp中的PolylineIns类
     * 或者反过来
     */
private:
    // 成员属性

    // 成员方法

public:
    // 成员属性

    // 成员方法
    PolylineInsConverter();
    ~PolylineInsConverter();
    void polylineIns2Jsonstr(PolylineIns& polylineIns);
};


#endif //ATUOCADDATAANALYSISPYCADCPP_POLYLINEINSCONVERTER_H
