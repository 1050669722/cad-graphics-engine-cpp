//
// Created by liuxingbo03 on 2022/1/7.
//

#ifndef ATUOCADDATAANALYSISPYCADCPP_SPLINEINFODICTCONVERTER_H
#define ATUOCADDATAANALYSISPYCADCPP_SPLINEINFODICTCONVERTER_H


#include "pycad/interface/converters/infoDictsConverter/BaseFieldsInJsonInfoDictAndElementInsesConverter.h"


class SplineInfoDictConverter : public BaseFieldsInJsonInfoDictAndElementInsesConverter<SplineInfoDict>{
    /*
     * 将 SplineInfoDict实例的jsonstr 转换为 cpp中的SplineInfoDict类
     * 或者反过来
     */
private:
    // 成员属性

    // 成员方法
    void getClosed();
    void getDegree();
    void getStartFitTangent();
    void getEndFitTangent();
    void getHasFitData();
    void getIsRational();
    void getControlPointNub();
    void getControlPoints();
    void getFitPointNub();
    void getFitPoints();

public:
    // 成员属性

    // 成员方法
    SplineInfoDictConverter();
    ~SplineInfoDictConverter();
    void jsonstr2SplineInfoDict(const char* jsonstr);
};


#endif //ATUOCADDATAANALYSISPYCADCPP_SPLINEINFODICTCONVERTER_H
