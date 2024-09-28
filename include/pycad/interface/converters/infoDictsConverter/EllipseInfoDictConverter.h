//
// Created by liuxingbo03 on 2022/1/7.
//

#ifndef ATUOCADDATAANALYSISPYCADCPP_ELLIPSEINFODICTCONVERTER_H
#define ATUOCADDATAANALYSISPYCADCPP_ELLIPSEINFODICTCONVERTER_H


#include "pycad/interface/converters/infoDictsConverter/BaseFieldsInJsonInfoDictAndElementInsesConverter.h"


class EllipseInfoDictConverter : public BaseFieldsInJsonInfoDictAndElementInsesConverter<EllipseInfoDict> {
    /*
     * 将 EllipseInfoDict实例的jsonstr 转换为 cpp中的EllipseInfoDict类
     * 或者反过来
     */
private:
    // 成员属性

    // 成员方法
    void getClosed();
    void getCenter();
    void getStartAngle();
    void getEndAngle();
    void getMajorAxis();
    void getMinorAxis();
    void getRadiusRatio();
    void getNormal();

public:
    // 成员属性

    // 成员方法
    EllipseInfoDictConverter();
    ~EllipseInfoDictConverter();
    void jsonstr2EllipseInfoDict(const char* jsonstr);
};


#endif //ATUOCADDATAANALYSISPYCADCPP_ELLIPSEINFODICTCONVERTER_H
