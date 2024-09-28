//
// Created by liuxingbo03 on 2022/1/7.
//

#ifndef ATUOCADDATAANALYSISPYCADCPP_ELLIPSEINSCONVERTER_H
#define ATUOCADDATAANALYSISPYCADCPP_ELLIPSEINSCONVERTER_H


#include "pycad/interface/converters/elementsConverter/BaseInsConverter.h"


class EllipseInsConverter : public BaseInsConverter<EllipseIns> {
    /*
     * 将 EllipseIns实例的jsonstr 转换为 cpp中的EllipseIns类
     * 或者反过来
     */
private:
    // 成员属性

    // 成员方法

public:
    // 成员属性

    // 成员方法
    EllipseInsConverter();
    ~EllipseInsConverter();
    void ellipseIns2Jsonstr(EllipseIns& ellipseIns);
};


#endif //ATUOCADDATAANALYSISPYCADCPP_ELLIPSEINSCONVERTER_H
