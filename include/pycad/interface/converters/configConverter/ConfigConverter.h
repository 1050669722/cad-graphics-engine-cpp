//
// Created by liuxingbo03 on 2021/11/30.
//

#ifndef ATUOCADDATAANALYSISPYCADCPP_CONFIGCONVERTER_H
#define ATUOCADDATAANALYSISPYCADCPP_CONFIGCONVERTER_H


#include "rapidjson/document.h"
#include "rapidjson/writer.h"

#include "pycad/Configuration.h"


using namespace rapidjson;


class ConfigConverter {
    /*
     * 将 config实例的jsonstr 转换为 cpp中的Configuration类
     * TODO: 接口，需要放在各个类中去吗？例如将这个类的关键方法放到Configuration中作为一个静态方法？
     */
private:
    // 成员属性
    Document* docForPaths = nullptr;
    Document* doc = nullptr;
    vector<wchar_t*> wchs;

    // 成员方法
    const wchar_t* getWCh(const char* ch);

    static bool is_str_utf8(const char* str);
    static bool is_str_gbk(const char* str);

    void initDoc();
    void getJsonPath();
    void getCache();
    void getColor(Value& colorMap, const char* color);
    void getColorMap();
    void getRestoreScaleFactor();
    void getResotreResolution();
    void getRedundantBorder();
    void getIsSaveFig();
    void getSavePath();
    void getSaveFileName();
    void getVerbose();

    void getRegions();
    void getInsideOrOutside();

    void getJudgementsInterOrUnion();
    void getCrimeList(Value& accuse, const char* accusation, const char* crimeMark);
    void getAccusation(Value& judgements, const char* accusation);
    void getJudgements();
    void getWhitelistForType();

public:
    // 成员属性
    map<string, Scalar> aliasOfColorMap;
    vector<string> aliasOfCrimeList;
    vector<string> aliasOfWhitelistForType;
    Configuration config; //对于此处的值，全部采用覆写，而不是追加

    // 成员方法
    ConfigConverter();
    ~ConfigConverter();
    void jsonstr2Config(const char *jsonstrForPaths, const char *jsonstr);
};


#endif //ATUOCADDATAANALYSISPYCADCPP_CONFIGCONVERTER_H
