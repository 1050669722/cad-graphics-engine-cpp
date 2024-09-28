//
// Created by liuxingbo03 on 2022/1/7.
//

#ifndef ATUOCADDATAANALYSISPYCADCPP_CIRCLEINSCONVERTER_H
#define ATUOCADDATAANALYSISPYCADCPP_CIRCLEINSCONVERTER_H


#include "pycad/interface/converters/elementsConverter/BaseInsConverter.h"


class CircleInsConverter : public BaseInsConverter<CircleIns> {
    /*
     * 将 CircleIns实例的jsonstr 转换为 cpp中的CircleIns类
     * 或者反过来
     */
private:
    // 成员属性

    // 成员方法

public:
    // 成员属性

    // 成员方法
    CircleInsConverter();
    ~CircleInsConverter();
    void circleIns2Jsonstr(CircleIns& circleIns);
};


#endif //ATUOCADDATAANALYSISPYCADCPP_CIRCLEINSCONVERTER_H
