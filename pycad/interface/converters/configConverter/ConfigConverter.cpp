//
// Created by liuxingbo03 on 2021/11/30.
//

#include <opencv2/opencv.hpp>

#include "pycad/interface/converters/configConverter/ConfigConverter.h"


using namespace cv;


ConfigConverter::ConfigConverter() = default;


ConfigConverter::~ConfigConverter() {
    delete docForPaths;
    delete doc;
    for (wchar_t* wch : wchs) {
        delete wch;
    }
}


const wchar_t* ConfigConverter::getWCh(const char* ch)
{
    const size_t chSize = strlen(ch) + 1;
    auto* wch = new wchar_t[chSize]; //TODO: 删除这个注释，wchar_t wch[chSize];
    mbstowcs(wch, ch, chSize);

    wchs.push_back(wch);

    return wch;
}


bool ConfigConverter::is_str_utf8(const char* str)
{
    unsigned int nBytes = 0;//UFT8可用1-6个字节编码,ASCII用一个字节
    unsigned char chr = *str;
    bool bAllAscii = true;

    for (unsigned int i = 0; str[i] != '\0'; ++i){
        chr = *(str + i);
        //判断是否ASCII编码,如果不是,说明有可能是UTF8,ASCII用7位编码,最高位标记为0,0xxxxxxx
        if (nBytes == 0 && (chr & 0x80) != 0){
            bAllAscii = false;
        }

        if (nBytes == 0) {
            //如果不是ASCII码,应该是多字节符,计算字节数
            if (chr >= 0x80) {

                if (chr >= 0xFC && chr <= 0xFD){
                    nBytes = 6;
                }
                else if (chr >= 0xF8){
                    nBytes = 5;
                }
                else if (chr >= 0xF0){
                    nBytes = 4;
                }
                else if (chr >= 0xE0){
                    nBytes = 3;
                }
                else if (chr >= 0xC0){
                    nBytes = 2;
                }
                else{
                    return false;
                }

                nBytes--;
            }
        }
        else{
            //多字节符的非首字节,应为 10xxxxxx
            if ((chr & 0xC0) != 0x80){
                return false;
            }
            //减到为零为止
            nBytes--;
        }
    }

    //违返UTF8编码规则
    if (nBytes != 0)  {
        return false;
    }

    if (bAllAscii){ //如果全部都是ASCII, 也是UTF8
        return true;
    }

    return true;
}


bool ConfigConverter::is_str_gbk(const char* str)
{
    unsigned int nBytes = 0;//GBK可用1-2个字节编码,中文两个 ,英文一个
    unsigned char chr = *str;
    bool bAllAscii = true; //如果全部都是ASCII,

    for (unsigned int i = 0; str[i] != '\0'; ++i){
        chr = *(str + i);
        if ((chr & 0x80) != 0 && nBytes == 0){// 判断是否ASCII编码,如果不是,说明有可能是GBK
            bAllAscii = false;
        }

        if (nBytes == 0) {
            if (chr >= 0x80) {
                if (chr >= 0x81 && chr <= 0xFE){
                    nBytes = +2;
                }
                else{
                    return false;
                }

                nBytes--;
            }
        }
        else{
            if (chr < 0x40 || chr>0xFE){
                return false;
            }
            nBytes--;
        }//else end
    }

    if (nBytes != 0)  {		//违返规则
        return false;
    }

    if (bAllAscii){ //如果全部都是ASCII, 也是GBK
        return true;
    }

    return true;
}


void ConfigConverter::initDoc() {
    docForPaths = new Document();
    doc = new Document();
}


void ConfigConverter::getJsonPath() {
    Value* jsonPath{}; if (docForPaths->HasMember("jsonPath")) jsonPath = &((*docForPaths)["jsonPath"]);
    config.jsonPathFromPython = jsonPath->GetString();
    // config.jsonPath = getWCh(jsonPath->GetString());
    // string tmp = jsonPath->GetString();
    // TODO: 字符串编码转换
    // config.jsonPath = getWCh(tmp.c_str()); //("D:/cad_draw_engine_cpp/cad-graphics-engine-cpp/AtuoCADDataAnalysisPyCADCpp/json/15.集团版YJ160-11(T2-17X)_t3.json"); //getWCh(jsonPath->GetString());
    // cout << tmp << endl;
    // cout << tmp.c_str() << endl;
    // cout << "is tmp.c_str() a utf8 or gbk: " << is_str_utf8(tmp.c_str()) << " " << is_str_gbk(tmp.c_str()) << endl;
    // cout << config.jsonPath << endl;

    // const char* tmp = "D:/cad_draw_engine_cpp/cad-graphics-engine-cpp/AtuoCADDataAnalysisPyCADCpp/json/15.集团版YJ160-11(T2-17X)_t3.json";
    // config.jsonPath = getWCh(tmp);
    // cout << tmp << endl; //这两行内容
    // cout << getWCh(tmp) << endl; //是一样的
}


void ConfigConverter::getCache() {
    Value* cache{}; if (docForPaths->HasMember("cache")) cache = &((*docForPaths)["cache"]);
    config.cache = cache->GetString();
}


void ConfigConverter::getColor(Value& colorMap, const char* color) {
    auto RGB = colorMap[color].GetArray();
    Scalar scalar{RGB[2].GetDouble(), RGB[1].GetDouble(), RGB[0].GetDouble()}; //那个config针对pygame，这个config针对opencv
    aliasOfColorMap.insert(pair<string, Scalar>(color, scalar));
}


void ConfigConverter::getColorMap() {
    Value* colorMap{}; if (doc->HasMember("colorMap")) colorMap = &((*doc)["colorMap"]);
    assert(colorMap->IsObject());
    aliasOfColorMap.clear();
    getColor(*colorMap, "r");
    getColor(*colorMap, "g");
    getColor(*colorMap, "b");
    getColor(*colorMap, "m");
    getColor(*colorMap, "white");
    getColor(*colorMap, "yellow");
    getColor(*colorMap, "orange");
    getColor(*colorMap, "pink");
    getColor(*colorMap, "violet");
    getColor(*colorMap, "black");
    getColor(*colorMap, "SlateBlue");
    getColor(*colorMap, "SaddleBrown");
    getColor(*colorMap, "grassgreen");
    getColor(*colorMap, "chocolate");
    getColor(*colorMap, "mazarine");
    getColor(*colorMap, "TurquoiseBlue");
    config.colorMap = aliasOfColorMap;
}


void ConfigConverter::getRestoreScaleFactor() {
    Value* restoreScaleFactor{}; if (doc->HasMember("restoreScaleFactor")) restoreScaleFactor = &((*doc)["restoreScaleFactor"]);
    config.restoreScaleFactor = restoreScaleFactor->GetDouble();
}


void ConfigConverter::getResotreResolution() {
    Value* resotreResolution{}; if (doc->HasMember("resotreResolution")) resotreResolution = &((*doc)["resotreResolution"]);
    if (resotreResolution->IsDouble()) {
        config.resotreResolution = resotreResolution->GetDouble();
    }
    else { //TODO: 这个else已经不需要了，resotreResolution, savePath, saveFileName均在Configuration.py中设置缺省值
        config.resotreResolution = 0.0; //对应python版本的config的此字段为空的情况
    }
}


void ConfigConverter::getRedundantBorder() {
    Value* redundantBorder{}; if (doc->HasMember("redundantBorder")) redundantBorder = &((*doc)["redundantBorder"]);
    config.redundantBorder = redundantBorder->GetInt();
}


void ConfigConverter::getIsSaveFig() {
    Value* isSaveFig{}; if (doc->HasMember("isSaveFig")) isSaveFig = &((*doc)["isSaveFig"]);
    config.isSaveFig = isSaveFig->GetBool();
}


void ConfigConverter::getSavePath() {
    Value* savePath{}; if (docForPaths->HasMember("savePath")) savePath = &((*docForPaths)["savePath"]);
    config.savePath = savePath->GetString();
}


void ConfigConverter::getSaveFileName() {
    Value* saveFileName{}; if (docForPaths->HasMember("saveFileName")) saveFileName = &((*docForPaths)["saveFileName"]);
    config.saveFileName = saveFileName->GetString();
}


void ConfigConverter::getVerbose() {
    Value* verbose{}; if (doc->HasMember("verbose")) verbose = &((*doc)["verbose"]);
    config.verbose = verbose->GetBool();
}


void ConfigConverter::getRegions() {
    Value* regions{}; if (doc->HasMember("regions")) regions = &((*doc)["regions"]);
    assert(regions->IsArray());

    if (regions->Size() == 2) {
        for (int i = 0; i < regions->Size(); ++i) {
            assert((*regions)[i].IsArray());
            for (int j = 0; j < (*regions)[i].Size(); ++j) {
                config.region.push_back((*regions)[i][j].GetDouble());
            }
        }
    }
    else if (regions->Size() == 0) {
        config.region = vector<double>{};
    }
    else {
        cout << "[WARNING] The value of \"regions\" has an incorrect length" << endl;
    }
}


void ConfigConverter::getInsideOrOutside() {
    Value* insideOrOutside{}; if (doc->HasMember("insideOrOutside")) insideOrOutside = &((*doc)["insideOrOutside"]);
    config.insideOrOutside = insideOrOutside->GetString();
}


void ConfigConverter::getJudgementsInterOrUnion() {
    Value* judgementsInterOrUnion{}; if (doc->HasMember("judgementsInterOrUnion")) judgementsInterOrUnion = &((*doc)["judgementsInterOrUnion"]);
    config.judgementsInterOrUnion = judgementsInterOrUnion->GetString();
}


void ConfigConverter::getCrimeList(Value& accuse, const char* accusation, const char* crimeMark) {
    // 以旧换新
    map<string, string> accusationMap{
        pair<string, string>("blockHandlesPedigree", "pedigreeLogger"),
        pair<string, string>("layers", "layerHistory"),
        pair<string, string>("BRHandles", "BRHandleHistory")
    };

    // 填写crimeList
    auto crimeList = accuse[crimeMark].GetArray();
    aliasOfCrimeList.clear();
    for (auto& crime : crimeList) {
        aliasOfCrimeList.emplace_back(crime.GetString());
    }
    config.judgements[accusationMap[accusation]][crimeMark] = aliasOfCrimeList;
}


void ConfigConverter::getAccusation(Value& judgements, const char* accusation) {
    Value* accuse{}; if (judgements.HasMember(accusation)) accuse = &(judgements[accusation]);
    assert(accuse->IsObject());
    getCrimeList(*accuse, accusation, "wanted");
    getCrimeList(*accuse, accusation, "blackList");
}


void ConfigConverter::getJudgements() {
    /*
     * 这里不修改python版本的yaml文件的字段，而将其中的字段赋值到新config的对应字段
     * blockHandlesPedigree - pedigreeLogger
     * layers - layerHistory
     * BRHandles - BRHandleHistory
     */
    Value* judgements{}; if (doc->HasMember("judgements")) judgements = &((*doc)["judgements"]);
    assert(judgements->IsObject());
    getAccusation(*judgements, "blockHandlesPedigree");
    getAccusation(*judgements, "layers");
    getAccusation(*judgements, "BRHandles");
}


void ConfigConverter::getWhitelistForType() {
    Value* whitelistForType{}; if (doc->HasMember("whitelistForType")) whitelistForType = &((*doc)["whitelistForType"]);
    assert(whitelistForType->IsArray());
    aliasOfWhitelistForType.clear();
    for (auto& type : whitelistForType->GetArray()) {
        aliasOfWhitelistForType.emplace_back(type.GetString());
    }
    config.whitelistForType = aliasOfWhitelistForType;
}


void ConfigConverter::jsonstr2Config(const char *jsonstrForPaths, const char *jsonstr) {
    /*
     * 根据jsonstr的构造，
     * 获取其中的数据信息；
     */
    // 实例化Document对象
    initDoc();

    // 填充doc中的内容
    assert(!(docForPaths->Parse(jsonstrForPaths).HasParseError()));
    assert(!(doc->Parse(jsonstr).HasParseError())); //TODO: 这一句好像必须得有，因为有Parse

    // jsonPath
    getJsonPath();

    // cacae
    getCache();

    // colorMap
    getColorMap();

    // restoreScaleFactor
    getRestoreScaleFactor();

    // resotreResolution
    getResotreResolution();

    // redundantBorder
    getRedundantBorder();

    // isSaveFig
    getIsSaveFig();

    // savePath
    getSavePath();

    // saveFileName
    getSaveFileName();

    // verbose
    getVerbose();

    // fontfile //TODO

    // fontsize //TODO

    // vacantTextInfo //TODO

    // judgementsInterOrUnion
    getJudgementsInterOrUnion();

    // judgements
    getJudgements();

    // regions //TODO
    getRegions();

    // insideOrOutside //TODO
    getInsideOrOutside();

    // whitelistForType
    getWhitelistForType();
}
