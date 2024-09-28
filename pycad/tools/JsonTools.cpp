//
// Created by liuxingbo03 on 2021/10/8.
//

#include <iostream>
#include <string>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
// #include "rapidjson/stringbuffer.h"
#include <vector>
#include <map>
// #include <sys/stat.h>
// #include <filesystem>

// #include <boost/timer.hpp>
#include <ctime>

#include "pycad/Structs.h"
#include "pycad/tools/JsonTools.h"
#include "pycad/Utils.h"


using namespace std;
using namespace rapidjson;
// using namespace boost;


// void JsonTools::readJsonFile(string &result, const wchar_t *filename) {
//     // 文件指针
//     FILE *fp = _wfopen(filename, L"rb, ccs=utf-8"); //这个utf-8应该是对应json文件的编码形式
//     // FILE *fp = _wfopen(filename, L"rb, ccs=unicode"); //这个utf-8应该是对应json文件的编码形式
//     // FILE *fp = _wfopen(filename, L"rb, ccs=gb2312"); //这个utf-8应该是对应json文件的编码形式
//     // FILE *fp = fopen(filename, "rb, ccs=utf-8");
//     // FILE *fp = fopen(filename, "rb, ccs=gbk");
//     // FILE *fp = _wfopen(reinterpret_cast<const wchar_t *>(filename), reinterpret_cast<const wchar_t *>("rb, ccs=unicode"));
//     // FILE *fp = fopen(filename, "rb, ccs=unicode");
//
//     // 检查打开是否正确
//     if (!fp) {
//         printf("[ERROR] Failed when open file: %s, code: %d", filename, errno);
//         result = ""; //return "";
//     }
//
//     // 预设jsonstr的最大占用空间
//     auto strSize = (size_t)(Utils::getFileSize(filename) * 1.2); //TODO: 能否写成1.0？ //1024 * 1024 * 1024; //1073741824 //1GB
//     char* buf = new char[strSize];
//     auto n = fread(buf, 1, strSize, fp); //int n
//     fclose(fp);
//
//     // 输出结果 释放内存
//     if (n >= 0) {
//         result.append(buf, 0, n);
//     }
//     delete[] buf;
// }


void JsonTools::readJsonFile(string &result, string &filenameFromPython) {
    // 文件指针
    FILE *fp = fopen(filenameFromPython.c_str(), "rb, ccs=utf-8"); //这个utf-8应该是对应json文件的编码形式

    // 检查打开是否正确
    if (!fp) {
        printf("[ERROR] Failed when open file: %s, code: %d", filenameFromPython.c_str(), errno);
        result = ""; //return "";
    }

    // 预设jsonstr的最大占用空间
    auto strSize = (size_t)(Utils::getFileSize(filenameFromPython) * 1.2); //1024 * 1024 * 1024; //1073741824 //1GB
    char* buf = new char[strSize];
    auto n = fread(buf, 1, strSize, fp); //int n
    fclose(fp);

    // 输出结果 释放内存
    if (n >= 0) {
        result.append(buf, 0, n);
    }
    delete[] buf;
}


string JsonTools::readJsonFile(const wchar_t* filename) {
    // 结果字符串
    string result;
    // 读取
    readJsonFile(result, filename);
    // 返回
    return result;
}


string JsonTools::readJsonFile(string &filenameFromPython) {
    // 结果字符串
    string result;
    // 读取
    readJsonFile(result, filenameFromPython);
    // 返回
    return result;
}


void JsonTools::getPosition(map<string, double> &resPosition, Value &position) {
    if (!position.IsObject()) {
        printf("[ERROR] This should be an object");
    }

    double x = position["x"].GetDouble();
    double y = position["y"].GetDouble();
    double z = position["z"].GetDouble();

    resPosition.insert(pair<string, double>("x", x));
    resPosition.insert(pair<string, double>("y", y));
    resPosition.insert(pair<string, double>("z", z));
}


map<string, double> JsonTools::getPosition(Value& position) {
    map<string, double> resPosition;
    getPosition(resPosition, position);
    return resPosition;
}


void JsonTools::getVertex(map<string, double>& resVertex, Value& vertex) {
    getPosition(resVertex, vertex);

    double bulge = vertex["bulge"].GetDouble();
    double startWidth = vertex["startWidth"].GetDouble();
    double endWidth = vertex["endWidth"].GetDouble();

    resVertex.insert(pair<string, double>("bulge", bulge));
    resVertex.insert(pair<string, double>("startWidth", startWidth));
    resVertex.insert(pair<string, double>("endWidth", endWidth));
}


map<string, double> JsonTools::getVertex(Value &vertex) {
    map<string, double> resVertex;
    getVertex(resVertex, vertex);
    return resVertex;
}


void JsonTools::getVertexs(vector<map<string, double>> &resVertexs, Value &vertexs) {
    if (!vertexs.IsArray()) {
        printf("[ERROR] This should be an array");
    }

    for(int i = 0; i < vertexs.Size(); i++){
        Value& vertex = vertexs[i];
        auto resVertex = getVertex(vertex);
        resVertexs.push_back(resVertex);
    }
}


vector<map<string, double>> JsonTools::getVertexs(Value& vertexs) {
    vector<map<string, double>> resVertexs;
    getVertexs(resVertexs, vertexs);
    return resVertexs;
}


void JsonTools::getBaseFields(Value& baseFieldsData, BaseFieldsInJsonInfoDictAndElementInses& BaseFields) {
    // BaseFieldsInJsonInfoDictAndElementInses BaseFields;

    // data应该是一个字典
    if (!baseFieldsData.IsObject()) {
        printf("[ERROR] This should be an object");
    }

    const char* id = baseFieldsData["id"].GetString();
    const char* handle = baseFieldsData["handle"].GetString();
    const char* name = baseFieldsData["name"].GetString();
    const char* layer = baseFieldsData["layer"].GetString();
    bool visible = true; //兼容 //应该有默认值true
    if (baseFieldsData.HasMember("visible")) visible = baseFieldsData["visible"].GetBool();
    // bool visible = baseFieldsData["visible"].GetBool();
    Value& coordmin = baseFieldsData["min"];
    Value& coordmax = baseFieldsData["max"];

    BaseFields.id = id; //baseFieldsData["id"].GetString();
    BaseFields.handle = handle; //baseFieldsData["handle"].GetString();
    BaseFields.name = name; //baseFieldsData["name"].GetString();
    BaseFields.layer = layer; //baseFieldsData["layer"].GetString();
    BaseFields.visible = visible; //baseFieldsData["visibleHistory"].GetBool();
    getPosition(BaseFields.min, coordmin);
    getPosition(BaseFields.max, coordmax);
    // getPosition(*(BaseFields.min), coordmin); //TODO: 为什么这样做不行呢？
    // getPosition(*(BaseFields.max), coordmax);
    // getPosition(BaseFields.min, coordmin);
    // getPosition(BaseFields.max, coordmax);

    // xdatas应该是一个数组
    // vector<string> BaseFields_xdatas{};
    Value xdatas;//兼容 //原来是Value& xdatas;
    if (baseFieldsData.HasMember("xdatas")) {
        xdatas = baseFieldsData["xdatas"];
        if (!xdatas.IsArray()) {
            printf("[ERROR] There is something wrong with the format of jsonstr");
        }
        for (int b = 0; b < xdatas.Size(); ++b) {
            string xdata = xdatas[b].GetString();
            // BaseFields_xdatas.push_back(xdata);
            BaseFields.xdatas.push_back(xdata);
        }
    }
    // BaseFields.xdatas = &BaseFields_xdatas;

    // return BaseFields;
}


void JsonTools::getEntitiyInfoDicts(vector<string> &entityInfoDicts, Value &block_entities) {
    if (!block_entities.IsArray()) {
        printf("[ERROR] There is something wrong with the format of jsonstr"); //: %s\n", jsonstr
        // return -1;
    }
    // 遍历每一个entity
    for (int j = 0; j < block_entities.Size(); ++j) {
        string entity = block_entities[j].GetString();
        entityInfoDicts.push_back(entity);
    }
}


void JsonTools::getMtextInfoDicts(vector<MtextInfoDict> &mtextInfoDicts, Value &block_mtexts) {
    if (!block_mtexts.IsArray()) {
        printf("[ERROR] There is something wrong with the format of jsonstr"); //: %s\n", jsonstr
        // return -1;
    }
    // 遍历每一个mtext
    for (int j = 0; j < block_mtexts.Size(); ++j) {
        Value& mtext = block_mtexts[j];
        if (!mtext.IsObject()) {
            printf("[ERROR] This should be an object");
            // return -1;
        }

        double mtext_actualHeight; if (mtext.HasMember("actualHeight")) mtext_actualHeight = mtext["actualHeight"].GetDouble();
        double mtext_actualWidth; if (mtext.HasMember("actualWidth")) mtext_actualWidth = mtext["actualWidth"].GetDouble();
        double mtext_rotation; if (mtext.HasMember("rotation")) mtext_rotation = mtext["rotation"].GetDouble();
        const char* mtext_contents; if (mtext.HasMember("contents")) mtext_contents = mtext["contents"].GetString();
        Value* mtext_direction{}; if (mtext.HasMember("direction")) mtext_direction = &(mtext["direction"]);
        Value* mtext_location{}; if (mtext.HasMember("location")) mtext_location = &(mtext["location"]);
        Value* mtext_normal{}; if (mtext.HasMember("normal")) mtext_normal = &(mtext["normal"]);

        // blocks.mtexts.base = getBase(mtextInfoDicts);
        mtextInfoDicts.emplace_back(); //blockInfoDict.mtextInfoDicts.push_back(MtextInfoDict());
        getBaseFields(mtext, mtextInfoDicts[j]);
        mtextInfoDicts[j].actualHeight = mtext_actualHeight; //mtext["actualHeight"].GetDouble();
        mtextInfoDicts[j].actualWidth = mtext_actualWidth; //mtext["actualWidth"].GetDouble();
        mtextInfoDicts[j].rotation = mtext_rotation; //mtext["rotation"].GetDouble();
        mtextInfoDicts[j].contents = mtext_contents; //mtext["contents"].GetString();
        mtextInfoDicts[j].direction = getPosition(*mtext_direction);
        mtextInfoDicts[j].location = getPosition(*mtext_location);
        mtextInfoDicts[j].normal = getPosition(*mtext_normal);
    }
}


void JsonTools::getTextInfoDicts(vector<TextInfoDict> &textInfoDicts, Value &block_texts) {
    if (!block_texts.IsArray()) {
        printf("[ERROR] There is something wrong with the format of jsonstr"); //: %s\n", jsonstr
        // return -1;
    }
    // 遍历每一个text
    for (int j = 0; j < block_texts.Size(); ++j) {
        Value& text = block_texts[j];
        if (!text.IsObject()) {
            printf("[ERROR] This should be an object");
            // return -1;
        }

        double text_height; if (text.HasMember("height")) text_height = text["height"].GetDouble();
        double text_widthFactor; if (text.HasMember("widthFactor")) text_widthFactor = text["widthFactor"].GetDouble();
        const char* text_text; if (text.HasMember("text")) text_text = text["text"].GetString();
        Value* position{}; if (text.HasMember("position")) position = &(text["position"]); //TODO: 如果这里真没有position字段，还需要写异常处理，或者在它被送入的下一个函数中写异常处理
        double text_rotation = text["rotation"].GetDouble();
        Value* normal{}; if (text.HasMember("normal")) normal = &(text["normal"]);
        const char* text_horizontalMode; if (text.HasMember("horizontalMode")) text_horizontalMode = text["horizontalMode"].GetString();
        const char* text_verticalMode; if (text.HasMember("verticalMode")) text_verticalMode = text["verticalMode"].GetString();

        // blocks.texts.base = getBaseFields(coordtext);
        textInfoDicts.emplace_back(); //blockInfoDict.textInfoDicts.push_back(TextInfoDict()); //
        getBaseFields(text, textInfoDicts[j]);
        textInfoDicts[j].height = text_height; //text["height"].GetDouble();
        textInfoDicts[j].widthFactor = text_widthFactor; //text["widthFactor"].GetDouble();
        textInfoDicts[j].text = text_text; //text["text"].GetString();
        textInfoDicts[j].position = getPosition(*position);
        textInfoDicts[j].rotation = text_rotation; //text["rotation"].GetDouble();
        textInfoDicts[j].normal = getPosition(*normal);
        textInfoDicts[j].horizontalMode = text_horizontalMode; //text["horizontalMode"].GetString();
        textInfoDicts[j].verticalMode = text_verticalMode; //text["verticalMode"].GetString();
    }
}


void JsonTools::getPolylineInfoDicts(vector<PolylineInfoDict> &polylineInfoDicts, Value &block_polylines) {
    if (!block_polylines.IsArray()) {
        printf("[ERROR] There is something wrong with the format of jsonstr"); //: %s\n", jsonstr
        // return -1;
    }
    // 遍历每一个polyline
    for (int j = 0; j < block_polylines.Size(); ++j) {
        Value& polyline = block_polylines[j];
        if (!polyline.IsObject()) {
            printf("[ERROR] This should be an object");
            // return -1;
        }

        bool polyline_closed; if (polyline.HasMember("closed")) polyline_closed = polyline["closed"].GetBool();
        Value* polyline_vertexs{}; if (polyline.HasMember("vertexs")) polyline_vertexs = &(polyline["vertexs"]);
        if (!((*polyline_vertexs).IsArray())) {
            printf("[ERROR] There is something wrong with the format of jsonstr: %s"); //\n", jsonstr
            // return -1;
        }

        // blocks.polylines.base = getBaseFields(polyline);
        polylineInfoDicts.emplace_back(); //blockInfoDict.polylineInfoDicts.push_back(PolylineInfoDict());
        getBaseFields(polyline, polylineInfoDicts[j]);
        polylineInfoDicts[j].closed = polyline_closed; //polyline["closed"].GetBool();
        polylineInfoDicts[j].vertexs = getVertexs(*polyline_vertexs);
    }
}


void JsonTools::getLineInfoDicts(vector<LineInfoDict> &lineInfoDicts, Value &block_lines) {
    if (!block_lines.IsArray()) {
        printf("[ERROR] There is something wrong with the format of jsonstr"); //: %s\n", jsonstr
        // return -1;
    }
    // 遍历每一个line
    for (int j = 0; j < block_lines.Size(); ++j) {
        Value& line = block_lines[j];
        if (!line.IsObject()) {
            printf("[ERROR] This should be an object");
            // return -1;
        }

        bool lines_closed; if (line.HasMember("closed")) lines_closed = line["closed"].GetBool();
        Value* lines_startPoint{}; if (line.HasMember("startPoint")) lines_startPoint = &(line["startPoint"]);
        Value* lines_endPoint{}; if (line.HasMember("endPoint")) lines_endPoint = &(line["endPoint"]);
        Value* lines_normal{}; if (line.HasMember("normal")) lines_normal = &(line["normal"]);

        // blocks.lines.base = getBaseFields(line);
        lineInfoDicts.emplace_back(); //blockInfoDict.lineInfoDicts.push_back(LineInfoDict());
        getBaseFields(line, lineInfoDicts[j]);
        lineInfoDicts[j].closed = lines_closed; //line["closed"].GetBool();
        lineInfoDicts[j].startPoint = getPosition(*lines_startPoint);
        lineInfoDicts[j].endPoint = getPosition(*lines_endPoint);
        lineInfoDicts[j].normal = getPosition(*lines_normal);
    }
}


void JsonTools::getArcInfoDicts(vector<ArcInfoDict> &arcInfoDicts, Value &block_arcs) {
    if (!block_arcs.IsArray()) {
        printf("[ERROR] There is something wrong with the format of jsonstr"); //: %s\n", jsonstr
        // return -1;
    }
    // 遍历每一个arc
    for (int j = 0; j < block_arcs.Size(); ++j) {
        Value &arc = block_arcs[j];
        if (!arc.IsObject()) {
            printf("[ERROR] This should be an object");
            // return -1;
        }

        bool arc_closed; if (arc.HasMember("closed")) arc_closed = arc["closed"].GetBool();
        Value* arc_center{}; if (arc.HasMember("center")) arc_center = &(arc["center"]);
        double arc_radius; if (arc.HasMember("radius")) arc_radius = arc["radius"].GetDouble();
        Value* arc_startPoint{}; if (arc.HasMember("startPoint")) arc_startPoint = &(arc["startPoint"]);
        Value* arc_endPoint{}; if (arc.HasMember("endPoint")) arc_endPoint = &(arc["endPoint"]);
        Value* arc_normal{}; if (arc.HasMember("normal")) arc_normal = &(arc["normal"]);

        // blocks.arcs.base = getBaseFields(arc);
        arcInfoDicts.emplace_back(); //blockInfoDict.arcInfoDicts.push_back(ArcInfoDict());
        getBaseFields(arc, arcInfoDicts[j]);
        arcInfoDicts[j].closed = arc_closed; //arc["closed"].GetBool();
        arcInfoDicts[j].center = getPosition(*arc_center);
        arcInfoDicts[j].radius = arc_radius; //arc["radius"].GetDouble();
        arcInfoDicts[j].startPoint = getPosition(*arc_startPoint);
        arcInfoDicts[j].endPoint = getPosition(*arc_endPoint);
        arcInfoDicts[j].normal = getPosition(*arc_normal);
    }
}


void JsonTools::getCircleInfoDicts(vector<CircleInfoDict> &circleInfoDicts, Value &block_circles) {
    if (!block_circles.IsArray()) {
        printf("[ERROR] There is something wrong with the format of jsonstr"); //: %s\n", jsonstr
        // return -1;
    }
    // 遍历每一个circle
    for (int j = 0; j < block_circles.Size(); ++j) {
        Value& circle = block_circles[j];
        if (!circle.IsObject()) {
            printf("[ERROR] This should be an object");
            // return -1;
        }
        bool circle_closed; if (circle.HasMember("closed")) circle_closed = circle["closed"].GetBool();
        Value* circle_center{}; if (circle.HasMember("center")) circle_center = &(circle["center"]);
        double circle_radius; if (circle.HasMember("radius")) circle_radius = circle["radius"].GetDouble();
        Value* circle_normal{}; if (circle.HasMember("normal")) circle_normal = &(circle["normal"]);

        // blocks.circles.base = getBaseFields(circle);
        circleInfoDicts.emplace_back(); //blockInfoDict.circleInfoDicts.push_back(CircleInfoDict());
        getBaseFields(circle, circleInfoDicts[j]);
        circleInfoDicts[j].closed = circle_closed; //circle["closed"].GetBool();
        circleInfoDicts[j].center = getPosition(*circle_center);
        circleInfoDicts[j].radius = circle_radius; //circle["radius"].GetDouble();
        circleInfoDicts[j].normal = getPosition(*circle_normal);
    }
}


void JsonTools::getEllipseInfoDicts(vector<EllipseInfoDict> &ellipseInfoDicts, Value &block_ellipses) {
    if (!block_ellipses.IsArray()) {
        printf("[ERROR] There is something wrong with the format of jsonstr"); //: %s\n", jsonstr
        // return -1;
    }
    // 遍历每一个ellipse
    for (int j = 0; j < block_ellipses.Size(); ++j) {
        Value& ellipse = block_ellipses[j];
        if (!ellipse.IsObject()) {
            printf("[ERROR] This should be an object");
            // return -1;
        }

        bool ellipse_closed; if (ellipse.HasMember("closed")) ellipse_closed = ellipse["closed"].GetBool();
        Value* ellipse_center{}; if (ellipse.HasMember("center")) ellipse_center = &(ellipse["center"]);
        double ellipse_startAngle; if (ellipse.HasMember("startAngle")) ellipse_startAngle = ellipse["startAngle"].GetDouble();
        double ellipse_endAngle; if (ellipse.HasMember("endAngle")) ellipse_endAngle = ellipse["endAngle"].GetDouble();
        // Value& ellipse_majorAxis = ellipse["maiorAxis"]; //TODO: 后续应该修改为majorAxis
        // Value& ellipse_majorAxis = ellipse.HasMember("majorAxis") ? ellipse["majorAxis"] : ellipse["maiorAxis"]; //兼容
        Value* ellipse_majorAxis = ellipse.HasMember("majorAxis") ? &(ellipse["majorAxis"]) : &(ellipse["maiorAxis"]);
        Value* ellipse_minorAxis{}; if (ellipse.HasMember("minorAxis")) ellipse_minorAxis = &(ellipse["minorAxis"]);
        // double ellipse_radiusRatio; if (ellipse.HasMember("rediusRatio")) ellipse_radiusRatio = ellipse["rediusRatio"].GetDouble();
        double ellipse_radiusRatio = ellipse.HasMember("radiusRatio") ? ellipse["radiusRatio"].GetDouble() : ellipse["rediusRatio"].GetDouble(); //TODO: 字段拼写兼容？
        Value* ellipse_normal{}; if (ellipse.HasMember("normal")) ellipse_normal = &(ellipse["normal"]);

        // blocks.ellipses.base = getBaseFields(ellipse);
        ellipseInfoDicts.emplace_back(); //blockInfoDict.ellipseInfoDicts.push_back(EllipseInfoDict());
        getBaseFields(ellipse, ellipseInfoDicts[j]);
        ellipseInfoDicts[j].closed = ellipse_closed; //ellipse["closed"].GetBool();
        ellipseInfoDicts[j].center = getPosition(*ellipse_center);
        ellipseInfoDicts[j].startAngle = ellipse_startAngle; //ellipse["startAngle"].GetDouble();
        ellipseInfoDicts[j].endAngle = ellipse_endAngle; //ellipse["endAngle"].GetDouble();
        ellipseInfoDicts[j].majorAxis = getPosition(*ellipse_majorAxis);
        ellipseInfoDicts[j].minorAxis = getPosition(*ellipse_minorAxis);
        ellipseInfoDicts[j].radiusRatio = ellipse_radiusRatio; //ellipse["rediusRatio"].GetDouble();
        ellipseInfoDicts[j].normal = getPosition(*ellipse_normal);
    }
}


void JsonTools::getBlockReferenceInfoDicts(vector<BlockReferenceInfoDict> &blockReferenceInfoDicts, Value &block_blockReferences) {
    if (!block_blockReferences.IsArray()) {
        printf("[ERROR] There is something wrong with the format of jsonstr"); //: %s\n", jsonstr
        // return -1;
    }
    // 遍历每一个blockReference
    for (int j = 0; j < block_blockReferences.Size(); ++j) {
        Value &blockReference = block_blockReferences[j];
        if (!blockReference.IsObject()) {
            printf("[ERROR] This should be an object");
            // return -1;
        }

        const char* blockReference_blockTableRecord; if (blockReference.HasMember("blockTableRecord")) blockReference_blockTableRecord = blockReference["blockTableRecord"].GetString();
        Value* blockReference_position{}; if (blockReference.HasMember("position")) blockReference_position = &(blockReference["position"]);
        double blockReference_rotation; if (blockReference.HasMember("rotation")) blockReference_rotation = blockReference["rotation"].GetDouble();
        Value* blockReference_scaleFactor{}; if (blockReference.HasMember("scaleFactors")) blockReference_scaleFactor = &(blockReference["scaleFactors"]);
        Value* blockReference_normal{}; if (blockReference.HasMember("normal")) blockReference_normal = &(blockReference["normal"]);
        bool blockReference_isDynBlock; if (blockReference.HasMember("isDynBlock")) blockReference_isDynBlock = blockReference["isDynBlock"].GetBool();

        // blocks.blockReferences.base = getBaseFields(blockReference);
        blockReferenceInfoDicts.emplace_back(); //blockInfoDict.blockReferenceInfoDicts.push_back(BlockReferenceInfoDict());
        getBaseFields(blockReference, blockReferenceInfoDicts[j]);
        blockReferenceInfoDicts[j].blockTableRecord = blockReference_blockTableRecord; //blockReference["blockTableRecord"].GetString();
        blockReferenceInfoDicts[j].position = getPosition(*blockReference_position);
        blockReferenceInfoDicts[j].rotation = blockReference_rotation; //blockReference["rotation"].GetDouble();
        blockReferenceInfoDicts[j].scaleFactor = getPosition(*blockReference_scaleFactor);
        blockReferenceInfoDicts[j].normal = getPosition(*blockReference_normal);
        blockReferenceInfoDicts[j].isDynBlock = blockReference_isDynBlock; //blockReference["isDynBlock"].GetBool();

        // block["blockReferences"]["attributes"]
        Value* blockReference_attributes{}; if (blockReference.HasMember("attributes")) blockReference_attributes = &(blockReference["attributes"]);
        if (!(blockReference_attributes->IsArray())) {
            printf("[ERROR] There is something wrong with the format of jsonstr"); //: %s\n", jsonstr
            // return -1;
        }
        // 遍历每一个attribute
        for (int m = 0; m < blockReference_attributes->Size(); ++m) {
            Value &bR_attribute = (*blockReference_attributes)[m];
            if (!bR_attribute.IsObject()) {
                printf("[ERROR] This should be an object");
                // return -1;
            }
            double bR_a_height; if (bR_attribute.HasMember("height")) bR_a_height = bR_attribute["height"].GetDouble();
            double bR_a_widthFactor; if (bR_attribute.HasMember("widthFactor")) bR_a_widthFactor = bR_attribute["widthFactor"].GetDouble();
            const char* bR_a_text; if (bR_attribute.HasMember("text")) bR_a_text = bR_attribute["text"].GetString();
            Value* bR_a_position{}; if (bR_attribute.HasMember("position")) bR_a_position = &(bR_attribute["position"]);
            double bR_a_rotation; if (bR_attribute.HasMember("rotation")) bR_a_rotation = bR_attribute["rotation"].GetDouble();
            Value* bR_a_normal{}; if (bR_attribute.HasMember("normal")) bR_a_normal = &(bR_attribute["normal"]);
            const char* bR_a_horizontalMode; if (bR_attribute.HasMember("horizontalMode")) bR_a_horizontalMode = bR_attribute["horizontalMode"].GetString();
            const char* bR_a_verticalMode; if (bR_attribute.HasMember("verticalMode")) bR_a_verticalMode = bR_attribute["verticalMode"].GetString();
            const char* bR_a_tag; if (bR_attribute.HasMember("tag")) bR_a_tag = bR_attribute["tag"].GetString();

            // blocks.blockReferences.attributes.base = getBaseFields(bR_attribute);
            blockReferenceInfoDicts[j].attributes.emplace_back(); //blockInfoDict.blockReferenceInfoDicts[j].attributes.push_back(AttributeInfoDict());
            getBaseFields(bR_attribute, blockReferenceInfoDicts[j].attributes[m]);
            blockReferenceInfoDicts[j].attributes[m].height = bR_a_height; //bR_attribute["height"].GetDouble();
            blockReferenceInfoDicts[j].attributes[m].widthFactor = bR_a_widthFactor; //bR_attribute["widthFactor"].GetDouble();
            blockReferenceInfoDicts[j].attributes[m].text = bR_a_text; //bR_attribute["text"].GetString();
            blockReferenceInfoDicts[j].attributes[m].position = getPosition(*bR_a_position);
            blockReferenceInfoDicts[j].attributes[m].rotation = bR_a_rotation; //bR_attribute["rotation"].GetDouble();
            blockReferenceInfoDicts[j].attributes[m].normal = getPosition(*bR_a_normal);
            blockReferenceInfoDicts[j].attributes[m].horizontalMode = bR_a_horizontalMode; //bR_attribute["horizontalMode"].GetString();
            blockReferenceInfoDicts[j].attributes[m].verticalMode = bR_a_verticalMode; //bR_attribute["verticalMode"].GetString();
            blockReferenceInfoDicts[j].attributes[m].tag = bR_a_tag; //bR_attribute["tag"].GetString();
        }

        // block["blockReferences"]["spatialFilter"]
        Value* blockReference_spatialFilter{};//兼容 //原本是Value& blockReference_spatialFilter
        if (blockReference.HasMember("spatialFilter")) {
            blockReference_spatialFilter = &(blockReference["spatialFilter"]);
            if (!blockReference_spatialFilter->IsObject()) {
                printf("[ERROR] This should be an object");
                // return -1;
            }

            int bR_s_pointsSize; if (blockReference_spatialFilter->HasMember("pointsSize")) bR_s_pointsSize = (*blockReference_spatialFilter)["pointsSize"].GetInt();
            Value* bR_s_normal{}; if (blockReference_spatialFilter->HasMember("normal")) bR_s_normal = &((*blockReference_spatialFilter)["normal"]);
            Value* bR_s_point{}; if (blockReference_spatialFilter->HasMember("point")) bR_s_point = &((*blockReference_spatialFilter)["point"]);
            bool bR_s_enable; if (blockReference_spatialFilter->HasMember("enable")) bR_s_enable = (*blockReference_spatialFilter)["enable"].GetBool();
            bool bR_s_isInverted; if (blockReference_spatialFilter->HasMember("isInverted")) bR_s_isInverted = (*blockReference_spatialFilter)["isInverted"].GetBool();
            int bR_s_clippingFront; if (blockReference_spatialFilter->HasMember("clippingFront")) bR_s_clippingFront = (*blockReference_spatialFilter)["clippingFront"].GetInt();
            int bR_s_clippingBack; if (blockReference_spatialFilter->HasMember("clippingBack")) bR_s_clippingBack = (*blockReference_spatialFilter)["clippingBack"].GetInt();

            blockReferenceInfoDicts[j].spatialFilter.pointsSize = bR_s_pointsSize; //(*blockReference_spatialFilter)["pointsSize"].GetInt();
            blockReferenceInfoDicts[j].spatialFilter.normal = getPosition(*bR_s_normal);
            blockReferenceInfoDicts[j].spatialFilter.point = getPosition(*bR_s_point);
            blockReferenceInfoDicts[j].spatialFilter.enable = bR_s_enable; //(*blockReference_spatialFilter)["enable"].GetBool();
            blockReferenceInfoDicts[j].spatialFilter.isInverted = bR_s_isInverted; //(*blockReference_spatialFilter)["isInverted"].GetBool();
            blockReferenceInfoDicts[j].spatialFilter.clippingFront = bR_s_clippingFront; //(*blockReference_spatialFilter)["clippingFront"].GetInt();
            blockReferenceInfoDicts[j].spatialFilter.clippingBack = bR_s_clippingBack; //(*blockReference_spatialFilter)["clippingBack"].GetInt();
        }

        // TODO isFromHatch的容错机制怎么实现？
        // // block["blockReferences"]["isFromHatch"]
        // try { // TODO 这里try语法的正确性有待验证
        //     Value& blockReference_isFromHatch = blockReference["isFromHatch"];
        //     bool bR_isFromHatch = blockReference_isFromHatch.GetBool();
        //     blockInfoDict.blockReferences[j].isFromHatch = bR_isFromHatch;
        // }
        // catch (int v) {
        //     blockInfoDict.blockReferences[j].isFromHatch = false;
        // };

        // TODO 之后与ODA协商之后再完成这些字段
        //boundaryPoints
        //clipSpace
        //inverseBlockRefForm
    }
}


void JsonTools::getPointInfoDicts(vector<PointInfoDict> &pointInfoDicts, Value &block_points) {
    if (!block_points.IsArray()) {
        printf("[ERROR] There is something wrong with the format of jsonstr"); //: %s\n", jsonstr
        // return -1;
    }
    // 遍历每一个point
    for (int j = 0; j < block_points.Size(); ++j) {
        Value& point = block_points[j];
        if (!point.IsObject()) {
            printf("[ERROR] This should be an object");
            // return -1;
        }

        Value* point_position{}; if (point.HasMember("position")) point_position = &(point["position"]);
        double point_ecsRotation; if (point.HasMember("ecsRotation")) point_ecsRotation = point["ecsRotation"].GetDouble();
        Value* point_normal{}; if (point.HasMember("normal")) point_normal = &(point["normal"]);

        // blocks.points.base = getBaseFields(point);
        pointInfoDicts.emplace_back(); //blockInfoDict.pointInfoDicts.push_back(PointInfoDict());
        getBaseFields(point, pointInfoDicts[j]);
        pointInfoDicts[j].position = getPosition(*point_position);
        pointInfoDicts[j].ecsRotation = point_ecsRotation;
        pointInfoDicts[j].normal = getPosition(*point_normal);
    }
}


void JsonTools::getSplineInfoDicts(vector<SplineInfoDict> &splineInfoDicts, Value &block_splines) {
    if (!block_splines.IsArray()) {
        printf("[ERROR] There is something wrong with the format of jsonstr"); //: %s\n", jsonstr
        // return -1;
    }
    // 遍历每一个spline
    for (int j = 0; j < block_splines.Size(); ++j) {
        Value& spline = block_splines[j];
        if (!spline.IsObject()) {
            printf("[ERROR] This should be an object");
            // return -1;
        }

        bool spline_closed; if (spline.HasMember("closed")) spline_closed = spline["closed"].GetBool();
        int spline_degree; if (spline.HasMember("degree")) spline_degree = spline["degree"].GetInt();
        Value* spline_startFitTangent{}; if (spline.HasMember("startFitTangent")) spline_startFitTangent = &(spline["startFitTangent"]);
        Value* spline_endFitTangent{}; if (spline.HasMember("endFitTangent")) spline_endFitTangent = &(spline["endFitTangent"]);
        bool spline_hasFitData; if (spline.HasMember("hasFitData")) spline_hasFitData = spline["hasFitData"].GetBool();
        bool spline_isRational; if (spline.HasMember("isRational")) spline_isRational = spline["isRational"].GetBool();
        int spline_controlPointNub; if (spline.HasMember("controlPointNub")) spline_controlPointNub = spline["controlPointNub"].GetInt();
        int spline_fitPointNub; if (spline.HasMember("fitPointNub")) spline_fitPointNub = spline["fitPointNub"].GetInt();

        // blocks.splines.base = getBaseFields(spline);
        splineInfoDicts.emplace_back(); //blockInfoDict.splineInfoDicts.push_back(SplineInfoDict());
        getBaseFields(spline, splineInfoDicts[j]);
        splineInfoDicts[j].closed = spline_closed;
        splineInfoDicts[j].degree = spline_degree;
        splineInfoDicts[j].startFitTangent = getPosition(*spline_startFitTangent);
        splineInfoDicts[j].endFitTangent = getPosition(*spline_endFitTangent);
        splineInfoDicts[j].hasFitData = spline_hasFitData;
        splineInfoDicts[j].isRational = spline_isRational;
        splineInfoDicts[j].controlPointNub = spline_controlPointNub;
        splineInfoDicts[j].fitPointNub = spline_fitPointNub;

        // block["splines"]["controlPoints"]
        Value* spline_controlPoints{}; if (spline.HasMember("controlPoints")) spline_controlPoints = &(spline["controlPoints"]);
        if (!spline_controlPoints->IsArray()) {
            printf("[ERROR] There is something wrong with the format of jsonstr"); //: %s\n", jsonstr
            // return -1;
        }
        // 遍历每一个controlPoint
        for (int m = 0; m < spline_controlPoints->Size(); ++m) {
            Value& c_point = (*spline_controlPoints)[m];
            if (!c_point.IsObject()) {
                printf("[ERROR] This should be an object");
                // return -1;
            }
            splineInfoDicts[j].controlPoints.push_back(getPosition(c_point));
        }
        // TODO 检查ODA有没有给出fitpoint
    }
}


void JsonTools::getHatchInfoDicts(vector<HatchInfoDict> &hatchInfoDicts, Value &block_hatchs) {
    if (!block_hatchs.IsArray()) {
        printf("[ERROR] There is something wrong with the format of jsonstr"); //: %s\n", jsonstr
        // return -1;
    }
    // 遍历每一个hatch
    for (int j = 0; j < block_hatchs.Size(); ++j) {
        Value &hatch = block_hatchs[j];
        if (!hatch.IsObject()) {
            printf("[ERROR] This should be an object");
            // return -1;
        }

        const char* hatch_hatchType; if (hatch.HasMember("hatchType")) hatch_hatchType = hatch["hatchType"].GetString();
        const char* hatch_patternName; if (hatch.HasMember("patternName")) hatch_patternName = hatch["patternName"].GetString();
        double hatch_patternAngle; if (hatch.HasMember("patternAngle")) hatch_patternAngle = hatch["patternAngle"].GetDouble();
        double hatch_patternScale; if (hatch.HasMember("patternScale")) hatch_patternScale = hatch["patternScale"].GetDouble();
        double hatch_patternSpace; if (hatch.HasMember("patternSpace")) hatch_patternSpace = hatch["patternSpace"].GetDouble();
        bool hatch_patternDouble; if (hatch.HasMember("patternDouble")) hatch_patternDouble = hatch["patternDouble"].GetBool();
        const char* hatch_hatchStyle; if (hatch.HasMember("hatchStyle")) hatch_hatchStyle = hatch["hatchStyle"].GetString();
        const char* hatch_color; if (hatch.HasMember("color")) hatch_color = hatch["color"].GetString();
        Value* hatch_originPoint{}; if (hatch.HasMember("originPoint")) hatch_originPoint = &(hatch["originPoint"]);

        // blocks.hatchs.base = getBaseFields(hatchs);
        hatchInfoDicts.emplace_back(); //blockInfoDict.hatchInfoDicts.push_back(HatchInfoDict());
        getBaseFields(hatch, hatchInfoDicts[j]);
        hatchInfoDicts[j].hatchType = hatch_hatchType;
        hatchInfoDicts[j].patternName = hatch_patternName;
        hatchInfoDicts[j].patternAngle = hatch_patternAngle;
        hatchInfoDicts[j].patternScale = hatch_patternScale;
        hatchInfoDicts[j].patternSpace = hatch_patternSpace;
        hatchInfoDicts[j].patternDouble = hatch_patternDouble;
        hatchInfoDicts[j].hatchStyle = hatch_hatchStyle;
        hatchInfoDicts[j].color = hatch_color;
        hatchInfoDicts[j].originPoint = getPosition(*hatch_originPoint);

        // hatch中那些类似于图元的东西
        if (hatch.HasMember("polylines")) getPolylineInfoDicts(hatchInfoDicts[j].polylineInfoDictsInHatch, hatch["polylines"]);
        if (hatch.HasMember("lines")) getLineInfoDicts(hatchInfoDicts[j].lineInfoDictsInHatch, hatch["lines"]);
        if (hatch.HasMember("arcs")) getArcInfoDicts(hatchInfoDicts[j].arcInfoDictsInHatch, hatch["arcs"]);
        if (hatch.HasMember("circles")) getCircleInfoDicts(hatchInfoDicts[j].circleInfoDictsInHatch, hatch["circles"]);
        if (hatch.HasMember("ellipses")) getEllipseInfoDicts(hatchInfoDicts[j].ellipseInfoDictsInHatch, hatch["ellipses"]);
        if (hatch.HasMember("blockReferences")) getBlockReferenceInfoDicts(hatchInfoDicts[j].blockReferenceInfoDictsInHatch, hatch["blockReferences"]);
        if (hatch.HasMember("points")) getPointInfoDicts(hatchInfoDicts[j].pointInfoDictsInHatch, hatch["points"]);
        if (hatch.HasMember("splines")) getSplineInfoDicts(hatchInfoDicts[j].splineInfoDictsInHatch, hatch["splines"]);

        // // block["hatchs"]["hpolylines"]
        // Value& hatch_polylines = hatch["polylines"];
        // if (!hatch_polylines.IsArray()) {
        //     printf("[ERROR] There is something wrong with the format of jsonstr: %s\n", jsonstr);
        //     return -1;
        // }
        // // 遍历每一个hpolyline
        // for (int m = 0; m < hatch_polylines.Size(); ++m) {
        //     Value& h_polyline = hatch_polylines[m];
        //     if (!h_polyline.IsObject()) {
        //         printf("[ERROR] This should be an object");
        //         return -1;
        //     }
        //
        //     bool h_p_closed = h_polyline["closed"].GetBool();
        //     Value& h_p_vertexs = h_polyline["vertexs"];
        //
        //     // blocks.hatchs.hpolylines.base = getBaseFields(h_polyline);
        //     blockInfoDict.hatchInfoDicts[j].hpolylines.emplace_back(); //blockInfoDict.hatchInfoDicts[j].hpolylines.push_back(HpolylineInfoDict());
        //     getBaseFields(h_polyline, blockInfoDict.hatchInfoDicts[j].hpolylines[m]);
        //     blockInfoDict.hatchInfoDicts[j].hpolylines[m].closed = h_p_closed;
        //     blockInfoDict.hatchInfoDicts[j].hpolylines[m].vertexs = getVertexs(h_p_vertexs);
        // }
        //
        // // block["hatchs"]["splines"]
        // Value& splines = hatch["splines"];
        // // if (!splines.Empty()){
        // //     printf("splines: %d\n",j);
        // // }
        //
        // // TODO 目前ODA仅给出了hatch中的polyline
    }
}


void JsonTools::getAlignedDimensionInfoDicts(vector<AlignedDimensionInfoDict> &alignedDimensionInfoDicts, Value &block_alignedDimensions) {
    if (!block_alignedDimensions.IsArray()) {
        printf("[ERROR] There is something wrong with the format of jsonstr"); //: %s\n", jsonstr
        // return -1;
    }
    // 遍历每一个alignedDimension
    for (int j = 0; j < block_alignedDimensions.Size(); ++j) {
        Value &aligned = block_alignedDimensions[j];
        if (!aligned.IsObject()) {
            printf("[ERROR] This should be an object");
            // return -1;
        }

        const char* aligned_textAttachment; if (aligned.HasMember("textAttachment")) aligned_textAttachment = aligned["textAttachment"].GetString();
        double aligned_textWidth; if (aligned.HasMember("textWidth")) aligned_textWidth = aligned["textWidth"].GetDouble();
        double aligned_textHeight; if (aligned.HasMember("textHeight")) aligned_textHeight = aligned["textHeight"].GetDouble();
        double aligned_textLineSpacingFactor; if (aligned.HasMember("textLineSpacingFactor")) aligned_textLineSpacingFactor = aligned["textLineSpacingFactor"].GetDouble();
        Value* aligned_textPosition{}; if (aligned.HasMember("textPosition")) aligned_textPosition = &(aligned["textPosition"]);
        double aligned_textRotation; if (aligned.HasMember("textRotation")) aligned_textRotation = aligned["textRotation"].GetDouble();
        bool aligned_isUsingDefaultTextPosition; if (aligned.HasMember("isUsingDefaultTextPosition")) aligned_isUsingDefaultTextPosition = aligned["isUsingDefaultTextPosition"].GetBool();
        const char* aligned_dimensionText; if (aligned.HasMember("dimensionText")) aligned_dimensionText = aligned["dimensionText"].GetString();
        double aligned_measurement; if (aligned.HasMember("measurement")) aligned_measurement = aligned["measurement"].GetDouble();
        const char* aligned_searchValue; if (aligned.HasMember("searchValue")) aligned_searchValue = aligned["searchValue"].GetString();
        Value* aligned_dimLinePoint{}; if (aligned.HasMember("dimLinePoint")) aligned_dimLinePoint = &(aligned["dimLinePoint"]);
        Value* aligned_xLine1Point{}; if (aligned.HasMember("xLine1Point")) aligned_xLine1Point = &(aligned["xLine1Point"]);
        Value* aligned_xLine2Point{}; if (aligned.HasMember("xLine2Point")) aligned_xLine2Point = &(aligned["xLine2Point"]);
        double aligned_oblique; if (aligned.HasMember("oblique")) aligned_oblique = aligned["oblique"].GetDouble();

        // blocks.alignedDimensions.base = getBaseFields(aligned);
        alignedDimensionInfoDicts.emplace_back(); //blockInfoDict.alignedDimensionInfoDicts.push_back(AlignedDimensionInfoDict());
        getBaseFields(aligned, alignedDimensionInfoDicts[j]);
        alignedDimensionInfoDicts[j].textAttachment = aligned_textAttachment;
        alignedDimensionInfoDicts[j].textHeight = aligned_textHeight;
        alignedDimensionInfoDicts[j].textWidth = aligned_textWidth;
        alignedDimensionInfoDicts[j].textLineSpacingFactor = aligned_textLineSpacingFactor;
        alignedDimensionInfoDicts[j].textPosition = getPosition(*aligned_textPosition);
        alignedDimensionInfoDicts[j].textRotation = aligned_textRotation;
        alignedDimensionInfoDicts[j].isUsingDefaultTextPosition = aligned_isUsingDefaultTextPosition;
        alignedDimensionInfoDicts[j].dimensionText = aligned_dimensionText;
        alignedDimensionInfoDicts[j].measurement = aligned_measurement;
        alignedDimensionInfoDicts[j].searchValue = aligned_searchValue;
        alignedDimensionInfoDicts[j].dimLinePoint = getPosition(*aligned_dimLinePoint);
        alignedDimensionInfoDicts[j].xLine1Point = getPosition(*aligned_xLine1Point);
        alignedDimensionInfoDicts[j].xLine2Point = getPosition(*aligned_xLine2Point);
        alignedDimensionInfoDicts[j].oblique = aligned_oblique;
    }
}


int JsonTools::parseJsonStr(const char* jsonstr, Infos& infos) {
    /*
     * 根据jsonstr的构造，
     * 获取其中的数据信息；
     */
    // 实例化一个Document对象
    Document doc;

    // 检查 parse error
    if (doc.Parse(jsonstr).HasParseError()) {
        printf("[ERROR] Parse error when parse jsonstr: %s\n", jsonstr);
        return -1;
    }

    // ****************************************************************************************************
    // layers层
    Value* layers{}; if (doc.HasMember("layers")) layers = &(doc["layers"]);

    // 检查 layers 是否为数组
    if (!layers->IsArray()) {
        printf("[ERROR] There is something wrong with the format of jsonstr: %s\n", jsonstr);
        return -1;
    }

    // 遍历这个数组
    for (int i = 0; i < layers->Size(); ++i) {
        // 容器对象中的一个子容器
        LayerInfoDict layerInfoDict;

        // 这个数组中的每一个元素应该是字典
        Value& layer_dict = (*layers)[i];
        if (!layer_dict.IsObject()) {
            printf("[ERROR] This should be an object");
            return -1;
        }

        // 获取字典中的数据信息 id handle name on frozen locked plottable color
        const char* layer_id; if (layer_dict.HasMember("id")) layer_id = layer_dict["id"].GetString();
        const char* layer_handle; if (layer_dict.HasMember("handle")) layer_handle = layer_dict["handle"].GetString();
        const char* layer_name; if (layer_dict.HasMember("name")) layer_name = layer_dict["name"].GetString();
        bool layer_on; if (layer_dict.HasMember("on")) layer_on = layer_dict["on"].GetBool();
        bool layer_frozen; if (layer_dict.HasMember("frozen")) layer_frozen = layer_dict["frozen"].GetBool();
        bool layer_locked; if (layer_dict.HasMember("locked")) layer_locked = layer_dict["locked"].GetBool();
        bool layer_plottable; if (layer_dict.HasMember("plottable")) layer_plottable = layer_dict["plottable"].GetBool();
        // bool layer_plottable = layer_dict["plottable"].GetBool();
        const char* layer_color; if (layer_dict.HasMember("color")) layer_color = layer_dict["color"].GetString();

        layerInfoDict.id = layer_id; //layer_dict["id"].GetString();
        layerInfoDict.handle = layer_handle; //layer_dict["handle"].GetString();
        layerInfoDict.name = layer_name; //layer_dict["name"].GetString();
        layerInfoDict.on = layer_on; //layer_dict["on"].GetBool();
        layerInfoDict.frozen = layer_frozen; //layer_dict["frozen"].GetBool();
        layerInfoDict.locked = layer_locked; //layer_dict["locked"].GetBool();
        layerInfoDict.plottable = layer_plottable; //layer_dict["plottable"].GetBool();
        layerInfoDict.color = layer_color; //layer_dict["color"].GetString();

        // 容器添加子容器 //infos.push_back(layerInfoDict);
        infos.layerInfoDicts.push_back(layerInfoDict);
    }
    // ****************************************************************************************************

    // ****************************************************************************************************
    // blocks层
    Value* blocks{}; if (doc.HasMember("blocks")) blocks = &(doc["blocks"]);

    // 检查 blocks 是否为数组
    if (!blocks->IsArray()) {
        printf("[ERROR] There is something wrong with the format of jsonstr: %s\n", jsonstr);
        return -1;
    }

    // 遍历这个数组
    for (int i = 0; i < blocks->Size(); ++i) {
        // 容器对象中的一个子容器
        BlockInfoDict blockInfoDict;

        // 这个数组中的每一个元素应该是字典
        Value& block_dict = (*blocks)[i];
        if (!block_dict.IsObject()) {
            printf("[ERROR] This should be an object");
            return -1;
        }

        // 获取字典中的数据信息

        // blcok[一般信息]
        const char* block_id; if (block_dict.HasMember("id")) block_id = block_dict["id"].GetString();
        const char* block_handle; if (block_dict.HasMember("handle")) block_handle = block_dict["handle"].GetString();
        const char* block_name; if (block_dict.HasMember("name")) block_name = block_dict["name"].GetString();
        Value* blocks_origin{}; if (block_dict.HasMember("origin")) blocks_origin = &(block_dict["origin"]);
        const char* blockScaling; if (block_dict.HasMember("blockScaling")) blockScaling = block_dict["blockScaling"].GetString();

        blockInfoDict.id = block_id; //block_dict["id"].GetString();
        blockInfoDict.handle = block_handle; //block_dict["handle"].GetString();
        blockInfoDict.name = block_name; //block_dict["name"].GetString();
        blockInfoDict.origin = getPosition(*blocks_origin);
        blockInfoDict.blockScaling = blockScaling; //block_dict["blockScaling"].GetString();

        // block["entities"]
        getEntitiyInfoDicts(blockInfoDict.entityInfoDicts, block_dict["entities"]);

        // block["mtexts"] 72
        getMtextInfoDicts(blockInfoDict.mtextInfoDicts, block_dict["mtexts"]);

        // block["texts"]
        getTextInfoDicts(blockInfoDict.textInfoDicts, block_dict["texts"]);

        // block["polylines"]
        getPolylineInfoDicts(blockInfoDict.polylineInfoDicts, block_dict["polylines"]);

        // block["lines"]
        getLineInfoDicts(blockInfoDict.lineInfoDicts, block_dict["lines"]);

        // block["arcs"]
        getArcInfoDicts(blockInfoDict.arcInfoDicts, block_dict["arcs"]);

        // block["circles"]
        getCircleInfoDicts(blockInfoDict.circleInfoDicts, block_dict["circles"]);

        // block["ellipses"]
        getEllipseInfoDicts(blockInfoDict.ellipseInfoDicts, block_dict["ellipses"]);

        // block["blockReferences"] //TODO blockReference应该还有其它很奇怪的字段 需要更多的json来测试
        getBlockReferenceInfoDicts(blockInfoDict.blockReferenceInfoDicts, block_dict["blockReferences"]);

        // block["points"] 79
        getPointInfoDicts(blockInfoDict.pointInfoDicts, block_dict["points"]);

        // block["splines"] 0
        getSplineInfoDicts(blockInfoDict.splineInfoDicts, block_dict["splines"]);

        // block["hatchs"] 0
        getHatchInfoDicts(blockInfoDict.hatchInfoDicts, block_dict["hatchs"]);

        // block["AlignedDimensions"] 36
        getAlignedDimensionInfoDicts(blockInfoDict.alignedDimensionInfoDicts, block_dict["alignedDimensions"]);

        // TODO other Dimensions

        // Value& entities = block_dict["entities"];
        // if (!entities.Empty()){
        //     printf("entities: %d\n",j);
        // }

        // 容器添加子容器
        infos.blockInfoDicts.push_back(blockInfoDict);
    }
    // ****************************************************************************************************

    // ****************************************************************************************************
    // layouts
    Value* layouts{}; if (doc.HasMember("layouts")) layouts = &(doc["layouts"]);

    // 检查 layouts 是否为数组
    if (!layouts->IsArray()) {
        printf("[ERROR] There is something wrong with the format of jsonstr: %s\n", jsonstr);
        return -1;
    }

    // 遍历这个数组
    for (int i = 0; i < layouts->Size(); ++i) {
        // 容器对象中的一个子容器
        LayoutInfoDict layoutInfoDict;

        // 这个数组中的每一个元素应该是字典
        Value& layout_dict = (*layouts)[i];
        if (!layout_dict.IsObject()) {
            printf("[ERROR] This should be an object");
            return -1;
        }

        // 获取字典中的数据信息
        const char* layout_id; if (layout_dict.HasMember("id")) layout_id = layout_dict["id"].GetString();
        const char* layout_handle; if (layout_dict.HasMember("handle")) layout_handle = layout_dict["handle"].GetString();
        const char* layout_name; if (layout_dict.HasMember("name")) layout_name = layout_dict["name"].GetString();
        const char* layout_blockTableRecordHandle; if (layout_dict.HasMember("blockTableRecordHandle")) layout_blockTableRecordHandle = layout_dict["blockTableRecordHandle"].GetString();
        double layout_tabOrder; if (layout_dict.HasMember("tabOrder")) layout_tabOrder = layout_dict["tabOrder"].GetDouble();
        bool layout_tabSelected; if (layout_dict.HasMember("tabSelected")) layout_tabSelected = layout_dict["tabSelected"].GetBool();
        Value* layout_min{}; if (layout_dict.HasMember("min")) layout_min = &(layout_dict["min"]);
        Value* layout_max{}; if (layout_dict.HasMember("max")) layout_max = &(layout_dict["max"]);

        layoutInfoDict.id = layout_id; //layout_dict["id"].GetString();
        layoutInfoDict.handle = layout_handle; //layout_dict["handle"].GetString();
        layoutInfoDict.name = layout_name; //layout_dict["name"].GetString();
        layoutInfoDict.blockTableRecordHandle = layout_blockTableRecordHandle; //layout_dict["blockTableRecordHandle"].GetString();
        layoutInfoDict.tabOrder = layout_tabOrder; //layout_dict["tabOrder"].GetDouble();
        layoutInfoDict.tabSelected = layout_tabSelected; //layout_dict["tabSelected"].GetBool();
        layoutInfoDict.min = getPosition(*layout_min);
        layoutInfoDict.max = getPosition(*layout_max);

        // 容器添加子容器
        infos.layoutInfoDicts.push_back(layoutInfoDict); //TODO: 值传递 修改为 地址传递
    }
    // ****************************************************************************************************

    // ****************************************************************************************************
    // groups
    Value* groups{}; if (doc.HasMember("groups")) groups = &(doc["groups"]);

    // 检查 groups 是否为数组
    if (!groups->IsArray()) {
        printf("[ERROR] There is something wrong with the format of jsonstr: %s\n", jsonstr);
        return -1;
    }
    // ****************************************************************************************************

    // 返回
    return 0;
}


// void JsonTools::readJson(Infos &infos, const wchar_t* jsonpath) {
//     // 统计时间
//     // timer t_ogm;
//     clock_t start = clock();
//
//     // 打印提示信息 //TODO 应该做成装饰模式
//     cout << "[INFO] readJson ..." << endl;
//
//     // jsonfile -> jsonstr
//     string jsonstr;
//     readJsonFile(jsonstr, jsonpath); //string jsonstr = this->readJsonFile(jsonPath);
//
//     // jsonstr -> infos容器
//     int parseCode = parseJsonStr(jsonstr.c_str(), infos);
//
//     // 统计时间
//     // double time_cost = t_ogm.elapsed(); //秒
//     double time_cost = (double)(clock() - start) / CLOCKS_PER_SEC; //秒
//     cout << "[INFO] readJson done, " << time_cost << "s has elapsed" << endl; //TODO: 装饰器怎么写出来？
// }


void JsonTools::readJson(Infos &infos, string &jsonpathFromPython) {
    // 统计时间
    // timer t_ogm;
    clock_t start = clock();

    // 打印提示信息 //TODO 应该做成装饰模式
    cout << "[INFO] readJson ..." << endl;

    // jsonfile -> jsonstr
    string jsonstr;
    readJsonFile(jsonstr, jsonpathFromPython); //string jsonstr = this->readJsonFile(jsonPath);

    // jsonstr -> infos容器
    int parseCode = parseJsonStr(jsonstr.c_str(), infos);

    // 统计时间
    // double time_cost = t_ogm.elapsed(); //秒
    double time_cost = (double)(clock() - start) / CLOCKS_PER_SEC; //秒
    cout << "[INFO] readJson done, " << time_cost << "s has elapsed" << endl; //TODO: 装饰器怎么写出来？
}


Infos JsonTools::readJson(const wchar_t* jsonpath) {
    // infos容器
    Infos infos;
    // json -> infos
    readJson(infos, jsonpath);
    // 返回
    return infos;
}


Infos JsonTools::readJson(string &jsonpathFromPython) {
    // infos容器
    Infos infos;
    // json -> infos
    readJson(infos, jsonpathFromPython);
    // 返回
    return infos;
}


// int main() {
//     // jsonPath
//     // TODO 文件名含有中文时，读取会错误
//     const char* jsonPath = "../json/15.YJ160-11(T2-17X)_t3.json";
//
//     // infos
//     Infos infos = JsonTools().readJson(jsonPath);
//
//     return 0;
// }
