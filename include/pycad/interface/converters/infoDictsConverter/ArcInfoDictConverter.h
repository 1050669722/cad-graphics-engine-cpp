//
// Created by liuxingbo03 on 2022/1/7.
//

#ifndef ATUOCADDATAANALYSISPYCADCPP_ARCINFODICTCONVERTER_H
#define ATUOCADDATAANALYSISPYCADCPP_ARCINFODICTCONVERTER_H


#include "pycad/interface/converters/infoDictsConverter/BaseFieldsInJsonInfoDictAndElementInsesConverter.h"


class ArcInfoDictConverter : public BaseFieldsInJsonInfoDictAndElementInsesConverter<ArcInfoDict> {
    /*
     * 将 ArcInfoDict实例的jsonstr 转换为 cpp中的ArcInfoDict类
     * 或者反过来
     */
private:
    // 成员属性

    // 成员方法
    void getClosed();
    void getCenter();
    void getRadius();
    void getStartPoint();
    void getEndPoint();
    void getNormal();

public:
    // 成员属性

    // 成员方法
    ArcInfoDictConverter();
    ~ArcInfoDictConverter();
    void jsonstr2ArcInfoDict(const char* jsonstr);
};


#endif //ATUOCADDATAANALYSISPYCADCPP_ARCINFODICTCONVERTER_H
