//
// Created by liuxingbo03 on 2022/1/7.
//

#ifndef ATUOCADDATAANALYSISPYCADCPP_SPLINEINSCONVERTER_H
#define ATUOCADDATAANALYSISPYCADCPP_SPLINEINSCONVERTER_H


#include "pycad/interface/converters/elementsConverter/BaseInsConverter.h"


class SplineInsConverter : public BaseInsConverter<SplineIns> {
    /*
     * 将 SplineIns实例的jsonstr 转换为 cpp中的SplineIns类
     * 或者反过来
     */
private:
    // 成员属性

    // 成员方法

public:
    // 成员属性

    // 成员方法
    SplineInsConverter();
    ~SplineInsConverter();
    void splineIns2Jsonstr(SplineIns& splineIns);
};


#endif //ATUOCADDATAANALYSISPYCADCPP_SPLINEINSCONVERTER_H
