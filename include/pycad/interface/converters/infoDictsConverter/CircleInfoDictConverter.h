//
// Created by liuxingbo03 on 2022/1/7.
//

#ifndef ATUOCADDATAANALYSISPYCADCPP_CIRCLEINFODICTCONVERTER_H
#define ATUOCADDATAANALYSISPYCADCPP_CIRCLEINFODICTCONVERTER_H


#include "pycad/interface/converters/infoDictsConverter/BaseFieldsInJsonInfoDictAndElementInsesConverter.h"


class CircleInfoDictConverter : public BaseFieldsInJsonInfoDictAndElementInsesConverter<CircleInfoDict> {
    /*
     * 将 CircleInfoDict实例的jsonstr 转换为 cpp中的CircleInfoDict类
     * 或者反过来
     */
private:
    // 成员属性

    // 成员方法
    void getClosed();
    void getCenter();
    void getRadius();
    void getNormal();

public:
    // 成员属性

    // 成员方法
    CircleInfoDictConverter();
    ~CircleInfoDictConverter();
    void jsonstr2CircleInfoDict(const char* jsonstr);
};


#endif //ATUOCADDATAANALYSISPYCADCPP_CIRCLEINFODICTCONVERTER_H
