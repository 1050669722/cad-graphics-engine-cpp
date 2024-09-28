//
// Created by liuxingbo03 on 2022/1/7.
//

#ifndef ATUOCADDATAANALYSISPYCADCPP_ARCINSCONVERTER_H
#define ATUOCADDATAANALYSISPYCADCPP_ARCINSCONVERTER_H


#include "pycad/interface/converters/elementsConverter/BaseInsConverter.h"


class ArcInsConverter : public BaseInsConverter<ArcIns> {
    /*
     * 将 ArcIns实例的jsonstr 转换为 cpp中的ArcIns类
     * 或者反过来
     */
private:
    // 成员属性

    // 成员方法

public:
    // 成员属性

    // 成员方法
    ArcInsConverter();
    ~ArcInsConverter();
    void arcIns2Jsonstr(ArcIns& arcIns);
};


#endif //ATUOCADDATAANALYSISPYCADCPP_ARCINSCONVERTER_H
