//
// Created by liuxingbo03 on 2022/1/5.
//

#ifndef ATUOCADDATAANALYSISPYCADCPP_POLYLINEINFODICTCONVERTER_H
#define ATUOCADDATAANALYSISPYCADCPP_POLYLINEINFODICTCONVERTER_H


#include "pycad/interface/converters/infoDictsConverter/BaseFieldsInJsonInfoDictAndElementInsesConverter.h"


class PolylineInfoDictConverter : public BaseFieldsInJsonInfoDictAndElementInsesConverter<PolylineInfoDict> {
    /*
     * 将 PolylineInfoDict实例的jsonstr 转换为 cpp中的PolylineInfoDict类
     * 或者反过来
     */
private:
    // 成员属性

    // 成员方法
    void getClosed();
    void getVertexs();

public:
    // 成员属性
    // PolylineInfoDict polylineInfoDict{};

    // 成员方法
    PolylineInfoDictConverter();
    ~PolylineInfoDictConverter();
    void jsonstr2PolylineInfoDict(const char* jsonstr);
};


#endif //ATUOCADDATAANALYSISPYCADCPP_POLYLINEINFODICTCONVERTER_H
