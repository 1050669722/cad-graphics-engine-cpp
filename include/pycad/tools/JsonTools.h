//
// Created by liuxingbo03 on 2021/10/9.
//

#ifndef ATUOCADDATAANALYSISPYCADCPP_REVITJSONREADER_H
#define ATUOCADDATAANALYSISPYCADCPP_REVITJSONREADER_H


#include "pycad/Structs.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"


using namespace std;
using namespace rapidjson;


class JsonTools {
public:
    // static void readJsonFile(string& result, const wchar_t* filename);
    static void readJsonFile(string& result, string& filenameFromPython);
    static string readJsonFile(const wchar_t* filename); //TODO: 考虑移动至Utils中去？
    static string readJsonFile(string& filenameFromPython); //TODO: 考虑移动至Utils中去？

    static void getPosition(map<string, double>& resPosition, Value& position);
    static map<string, double> getPosition(Value& position);

    static void getVertex(map<string, double>& resVertex, Value& vertex);
    static map<string, double> getVertex(Value& vertex);

    static void getVertexs(vector<map<string, double>>& resVertexs, Value& vertexs);
    static vector<map<string, double>> getVertexs(Value& vertexs);

    static void getBaseFields(Value& baseFieldsData, BaseFieldsInJsonInfoDictAndElementInses& BaseFields); // BaseFieldsInJsonInfoDictAndElementInses getBaseFields(rapidjson::Value& baseFieldsData);
    static void getEntitiyInfoDicts(vector<string>& entityInfoDicts, Value& block_entities);
    static void getMtextInfoDicts(vector<MtextInfoDict>& mtextInfoDicts, Value& block_mtexts);
    static void getTextInfoDicts(vector<TextInfoDict>& textInfoDicts, Value& block_texts);
    static void getPolylineInfoDicts(vector<PolylineInfoDict>& polylineInfoDicts, Value& block_polylines);
    static void getLineInfoDicts(vector<LineInfoDict>& lineInfoDicts, Value& block_lines);
    static void getArcInfoDicts(vector<ArcInfoDict>& arcInfoDicts, Value& block_arcs);
    static void getCircleInfoDicts(vector<CircleInfoDict>& circleInfoDicts, Value& block_circles);
    static void getEllipseInfoDicts(vector<EllipseInfoDict>& ellipseInfoDicts, Value& block_ellipses);
    static void getBlockReferenceInfoDicts(vector<BlockReferenceInfoDict>& blockReferenceInfoDicts, Value& block_blockReferences);
    static void getPointInfoDicts(vector<PointInfoDict>& pointInfoDicts, Value& block_points);
    static void getSplineInfoDicts(vector<SplineInfoDict>& splineInfoDicts, Value& block_splines);
    static void getHatchInfoDicts(vector<HatchInfoDict>& hatchInfoDicts, Value& block_hatchs);
    static void getAlignedDimensionInfoDicts(vector<AlignedDimensionInfoDict>& alignedDimensionInfoDicts, Value& block_alignedDimensions);

    static int parseJsonStr(const char* jsonstr, Infos& infos);

    // static void readJson(Infos& infos, const wchar_t* jsonpath);
    static void readJson(Infos& infos, string& jsonpathFromPython);
    static Infos readJson(const wchar_t* jsonpath);
    static Infos readJson(string& jsonpathFromPython);
};


#endif //ATUOCADDATAANALYSISPYCADCPP_REVITJSONREADER_H
