//
// Created by liuxingbo03 on 2021/12/13.
//

#include <iostream>
#include <fstream>
#include <thread>
// #include <boost/timer.hpp>
#include <ctime>

#include "pycad/interface/converters/databaseConverter/DatabaseConverter.h"
#include "pycad/Utils.h"


using namespace std;
// using json = nlohmann::json;
// using namespace boost;


DatabaseConverter::DatabaseConverter() = default;


DatabaseConverter::~DatabaseConverter() {
    delete docOutputDatabase;
    delete docInputDatabase;
}


// namespace dumpCore {
//     void to_json(json& j, const Criterion& criterions) {
//         j = json{
//                 {"belongToWhichBlock", criterions.belongToWhichBlock},
//                 {"layerHistory", criterions.layerHistory},
//                 {"BRHandleHistory", criterions.BRHandleHistory},
//                 {"pedigreeLogger", criterions.pedigreeLogger},
//                 {"visibleHistory", criterions.visibleHistory},
//                 {"isDynHistory", criterions.isDynHistory},
//                 {"isFromHatchHistory", criterions.isFromHatchHistory}
//         };
//     }
//
//     void to_json(json& j, const BaseFieldInDatabase& base) {
//         j = json{
//                 {"coords_x", base.coords_x},
//                 {"coords_y", base.coords_y},
//                 {"colors", base.colors},
//                 {"handles", base.handles}
//         };
//         for (auto& criterion : base.criterions) //TODO: 后续开发完善之后，应该每一种图元都有criterions
//         {
//             json j_criterion;
//             to_json(j_criterion, criterion);
//             j["criterions"].push_back(j_criterion);
//         }
//     }
// }
//
//
// void to_json(json &j, const Lines &lines) {
//     dumpCore::to_json(j, lines);
// }
//
//
// void to_json(json &j, const Polylines &polylines) {
//     dumpCore::to_json(j, polylines);
//     j["ori_coords_x"] = polylines.ori_coords_x;
//     j["ori_coords_y"] = polylines.ori_coords_y; //j = json{{"ori_coords_x", polylines.ori_coords_x},{"ori_coords_y", polylines.ori_coords_y}};
// }
//
//
// void to_json(json &j, const Circles &circles) {
//     dumpCore::to_json(j, circles);
// }
//
//
// void to_json(json &j, const Arcs &arcs) {
//     dumpCore::to_json(j, arcs);
// }
//
//
// void to_json(json &j, const Ellipses &ellipses) {
//     dumpCore::to_json(j, ellipses);
// }
//
//
// void to_json(json &j, const Points &points) {
//     dumpCore::to_json(j, points);
// }
//
//
// void to_json(json &j, const Splines &splines) {
//     dumpCore::to_json(j, splines);
// }
//
//
// void to_json(json &j, const Solids &solids) {
//     dumpCore::to_json(j, solids);
// }
//
//
// void to_json(json &j, const Texts &texts) {
//     dumpCore::to_json(j, texts);
// }
//
//
// void to_json(json &j, const Mtexts &mtexts) {
//     dumpCore::to_json(j, mtexts);
// }
//
//
// void to_json(json &j, const ATtributes &attributes) {
//     dumpCore::to_json(j, attributes);
// }
//
//
// void to_json(json &j, const AlignedDimensions &alignedDimensions) {
//     dumpCore::to_json(j, alignedDimensions);
// }
//
//
// void to_json(json &j, const RotatedDimensions &rotatedDimensions) {
//     dumpCore::to_json(j, rotatedDimensions);
// }
//
//
// void to_json(json &j, const LineAngularDimensions &lineAngularDimensions) {
//     dumpCore::to_json(j, lineAngularDimensions);
// }
//
//
// void to_json(json &j, const ArcDimensions &arcDimensions) {
//     dumpCore::to_json(j, arcDimensions);
// }
//
//
// void to_json(json &j, const RadialDimensions &radialDimensions) {
//     dumpCore::to_json(j, radialDimensions);
// }
//
//
// void to_json(json &j, const DiametricDimensions &diametricDimensions) {
//     dumpCore::to_json(j, diametricDimensions);
// }
//
//
// void to_json(json &j, const OrdinateDimensions &ordinateDimensions) {
//     dumpCore::to_json(j, ordinateDimensions);
// }
//
//
// void to_json(json &j, const RadialDimensionLarges &radialDimensionLarges) {
//     dumpCore::to_json(j, radialDimensionLarges);
// }
//
//
// void dumps(Database& database, string& jsonstr) {
//     json j;
//     j = json{
//             {"lines", database.lines},
//             {"polylines", database.polylines},
//             {"circles", database.circles},
//             {"arcs", database.arcs},
//             {"ellipses", database.ellipses},
//             {"points", database.points},
//             {"splines", database.splines},
//             {"solids", database.solids},
//             {"texts", database.texts},
//             {"mtexts", database.mtexts},
//             {"attributes", database.attributes},
//             {"alignedDimensions", database.alignedDimensions},
//             {"rotatedDimensions", database.rotatedDimensions},
//             {"lineAngularDimensions", database.lineAngularDimensions},
//             {"arcDimensions", database.arcDimensions},
//             {"radialDimensions", database.radialDimensions},
//             {"diametricDimensions", database.diametricDimensions},
//             {"ordinateDimensions", database.ordinateDimensions},
//             {"radialDimensionLarges", database.radialDimensionLarges}
//     };
//     jsonstr = j.dump();
//     // string s = j.dump(); //TODO: 考虑删除这三行
//     // char* outputDatabase = (char*)malloc(sizeof(char) * (s.length() + 1));
//     // strcpy(outputDatabase, s.c_str());
// }
//
//
// void dump(Database& database, string& filepath) {
//     // 序列化 到 jsonstr
//     string jsonstr;
//     dumps(database, jsonstr);
//
//     // 当该路径指示的文件尚不存在时
//     if (!Utils::isExist(filepath)) {
//         // 将 jsonstr 输出到 文件
//         ofstream ofs; //创建一个文件输出对象
//         ofs.open(filepath.c_str()); //将对象与文件关联
//         ofs << jsonstr;
//         ofs.close(); //取消关联
//     }
// }
//
//
// void DatabaseConverter::database2jsonstr(Database &database) {
//     // 统计时间
//     // timer t_ogm;
//     clock_t start = clock();
//
//     // 打印提示信息 //TODO 应该做成装饰模式
    // cout << "[INFO] database2jsonstr ..." << endl;
//
//     // string s;
//     // dumps(database, s);
//     // outputDatabase = (char*)malloc(sizeof(char) * (s.length() + 1)); //这里开辟的堆内存，后面一定得回收
//     // strcpy(outputDatabase, s.c_str());
//     // cout << "outputDatabase: " << &outputDatabase[0] << endl;
//     // printf("outputDatabase: %x", &outputDatabase[0]);
//     // cout << "the content of outputDatabase: " << *outputDatabase << endl;
//
//     dumps(database, outputDatabase);
//
//     // 统计时间
//     // double time_cost = t_ogm.elapsed(); //秒
//     double time_cost = (double)(clock() - start) / CLOCKS_PER_SEC; //秒
    // cout << "[INFO] database2jsonstr done, " << time_cost << "s has elapsed" << endl; //TODO: 装饰器怎么写出来？
// }


// *** init docOutputDatabase *******************************
void DatabaseConverter::initDocOutputDatabase() {
    /*
     * TODO: 如果连续使用同一DatabaseConverter对象的方法database2jsonstr多次，会不会出现问题？
     */
    // 实例化指针所指向的对象
    docOutputDatabase = new Document();
    // 设置docOutputDatabase为一个空的Object
    docOutputDatabase->SetObject();
    // 分配器指针
    allocator = &(docOutputDatabase->GetAllocator());
}
// *********************************************************


// *** send colors *************************************************************************************************
void DatabaseConverter::sendColors(Value &dstObject, vector<string> &srcColors) {
    Value arrayColors(kArrayType); //能复用吗？
    for (string& color : srcColors) {
        Value str; //能复用吗？
        str.SetString(color.c_str(), *allocator);
        arrayColors.PushBack(str, *allocator);
    }
    dstObject.AddMember("colors", arrayColors, *allocator);
}
// *****************************************************************************************************************


// *** send coords *************************************************************************************************
void DatabaseConverter::sendAtom(Value &dstArrayAtom, Atom &srcAtom) {
    for (double& v : srcAtom) {
        dstArrayAtom.PushBack(v, *allocator);
    }
}

void DatabaseConverter::sendCoord(Value& dstArrayCoord, Coord& srcCoord) {
    for (Atom& atom : srcCoord) {
        Value arrayAtom(kArrayType);
        sendAtom(arrayAtom, atom);
        dstArrayCoord.PushBack(arrayAtom, *allocator);
    }
}

void DatabaseConverter::sendCoords(Value &dstObject, const char* fieldName, Coords& srcCoords) {
    Value arrayCoords(kArrayType);
    for (Coord& coord : srcCoords) {
        Value arrayCoord(kArrayType);
        sendCoord(arrayCoord, coord);
        arrayCoords.PushBack(arrayCoord, *allocator);
    }
    Value str;
    str.SetString(fieldName, *allocator);
    dstObject.AddMember(str, arrayCoords, *allocator);
}
// *****************************************************************************************************************


// *** send criterions *********************************************************************************************
void DatabaseConverter::sendBRHandleHistory(Value &dstObject, BLockReferenceHandleHistory &srcBRHandleHistory) {
    Value arrayBRHandleHistory(kArrayType);
    for (string& BRHandle : srcBRHandleHistory) {
        Value str;
        str.SetString(BRHandle.c_str(), *allocator);
        arrayBRHandleHistory.PushBack(str, *allocator);
    }
    dstObject.AddMember("BRHandleHistory", arrayBRHandleHistory, *allocator);
}

void DatabaseConverter::sendBelongToWhichBlock(Value &dstObject, string &srcBelongToWhichBlock) {
    Value str;
    str.SetString(srcBelongToWhichBlock.c_str(), *allocator);
    dstObject.AddMember("belongToWhichBlock", str, *allocator);
}

void DatabaseConverter::sendIsDynHistory(Value &dstObject, IsDynHistory &srcIsDynHistory) {
    Value arrayIsDynHistory(kArrayType);
    for (bool& isDyn : srcIsDynHistory) {
        arrayIsDynHistory.PushBack(isDyn, *allocator);
    }
    dstObject.AddMember("isDynHistory", arrayIsDynHistory, *allocator);
}

void DatabaseConverter::sendIsFromHatchHistory(Value &dstObject, IsFromHatchHistory &srcIsFromHatchHistory) {
    Value arrayIsFromHatchHistory(kArrayType);
    for (bool& isFromHatch : srcIsFromHatchHistory) {
        arrayIsFromHatchHistory.PushBack(isFromHatch, *allocator);
    }
    dstObject.AddMember("isFromHatchHistory", arrayIsFromHatchHistory, *allocator);
}

void DatabaseConverter::sendLayerHistory(Value &dstObject, LayerHistory &srcLayerHistory) {
    Value arrayLayerHistory(kArrayType);
    for (string& layer : srcLayerHistory) {
        Value str;
        str.SetString(layer.c_str(), *allocator);
        arrayLayerHistory.PushBack(str, *allocator);
    }
    dstObject.AddMember("layerHistory", arrayLayerHistory, *allocator);
}

void DatabaseConverter::sendPedigreeLogger(Value &dstObject, PedigreeLogger &srcPedigreeLogger) {
    Value arrayPedigreeLogger(kArrayType);
    for (string& pedigree : srcPedigreeLogger) {
        Value str;
        str.SetString(pedigree.c_str(), *allocator);
        arrayPedigreeLogger.PushBack(str, *allocator);
    }
    dstObject.AddMember("pedigreeLogger", arrayPedigreeLogger, *allocator);
}

void DatabaseConverter::sendVisibleHistory(Value &dstObject, VisibleHistory &srcVisibleHistory) {
    Value arrayVisibleHistory(kArrayType);
    for (bool& visible : srcVisibleHistory) {
        arrayVisibleHistory.PushBack(visible, *allocator);
    }
    dstObject.AddMember("visibleHistory", arrayVisibleHistory, *allocator);
}

void DatabaseConverter::sendCriterion(Value &dstCriterion, Criterion &srcCriterion) {
    sendBRHandleHistory(dstCriterion, srcCriterion.BRHandleHistory);
    sendBelongToWhichBlock(dstCriterion, srcCriterion.belongToWhichBlock);
    sendIsDynHistory(dstCriterion, srcCriterion.isDynHistory);
    sendIsFromHatchHistory(dstCriterion, srcCriterion.isFromHatchHistory);
    sendLayerHistory(dstCriterion, srcCriterion.layerHistory);
    sendPedigreeLogger(dstCriterion, srcCriterion.pedigreeLogger);
    sendVisibleHistory(dstCriterion, srcCriterion.visibleHistory);
}

void DatabaseConverter::sendCriterions(Value &dstObject, vector<Criterion> &srcCriterions) {
    Value dstCriterions(kArrayType);
    for (Criterion& criterion : srcCriterions) {
        Value dstCriterion(kObjectType);
        sendCriterion(dstCriterion, criterion);
        dstCriterions.PushBack(dstCriterion, *allocator);
    }
    dstObject.AddMember("criterions", dstCriterions, *allocator);
}
// *****************************************************************************************************************


// *** send handles ************************************************************************************************
void DatabaseConverter::sendHandles(Value &dstObject, vector<string> &srcHandles) {
    Value arrayHandles(kArrayType);
    for (string& handle : srcHandles) {
        Value str;
        str.SetString(handle.c_str(), *allocator);
        arrayHandles.PushBack(str, *allocator);
    }
    dstObject.AddMember("handles", arrayHandles, *allocator);
}
// *****************************************************************************************************************


// *** send infos **************************************************************************************************
void DatabaseConverter::sendInfos(Value &dstObject, vector<string> &srcInfos) {
    Value arrayInfos(kArrayType);
    for (string& info : srcInfos) {
        Value str;
        str.SetString(info.c_str(), *allocator);
        arrayInfos.PushBack(str, *allocator);
    }
    dstObject.AddMember("info", arrayInfos, *allocator); //TODO: 若后续字段有调整，这里应该修改
}
// *****************************************************************************************************************


// *** send rotas **************************************************************************************************
void DatabaseConverter::sendRotas(Value &dstObject, vector<double> &srcRotas) {
    Value arrayRotas(kArrayType);
    for (double& rota : srcRotas) {
        arrayRotas.PushBack(rota, *allocator);
    }
    dstObject.AddMember("rota", arrayRotas, *allocator); //TODO: 若后续字段有调整，这里应该修改
}
// *****************************************************************************************************************


// *** send widthFactors *******************************************************************************************
void DatabaseConverter::sendWidthFactors(Value &dstObject, vector<double> &srcWidthFactors) {
    Value arrayWidthFactors(kArrayType);
    for (double& widthFactor : srcWidthFactors) {
        arrayWidthFactors.PushBack(widthFactor, *allocator);
    }
    dstObject.AddMember("widthFactor", arrayWidthFactors, *allocator); //TODO: 若后续字段有调整，这里应该修改
}
// *****************************************************************************************************************


// *** send horizontalModes ****************************************************************************************
void DatabaseConverter::sendHorizontalModes(Value &dstObject, vector<string> &srcHorizontalModes) {
    Value arrayHorizontalModes(kArrayType);
    for (string& horizontalMode : srcHorizontalModes) {
        Value str;
        str.SetString(horizontalMode.c_str(), *allocator);
        arrayHorizontalModes.PushBack(str, *allocator);
    }
    dstObject.AddMember("horizontalModes", arrayHorizontalModes, *allocator);
}
// *****************************************************************************************************************


// *** send verticalModes ******************************************************************************************
void DatabaseConverter::sendVerticalModes(Value &dstObject, vector<string> &srcVerticalModes) {
    Value arrayVerticalModes(kArrayType);
    for (string& verticalMode : srcVerticalModes) {
        Value str;
        str.SetString(verticalMode.c_str(), *allocator);
        arrayVerticalModes.PushBack(str, *allocator);
    }
    dstObject.AddMember("verticalModes", arrayVerticalModes, *allocator);
}
// *****************************************************************************************************************


// *** send lineNums ***********************************************************************************************
void DatabaseConverter::sendLineNums(Value &dstObject, vector<int> &srcLineNums) {
    Value arrayLineNums(kArrayType);
    for (int& lineNum : srcLineNums) {
        arrayLineNums.PushBack(lineNum, *allocator);
    }
    dstObject.AddMember("lineNums", arrayLineNums, *allocator);
}
// *****************************************************************************************************************


// *** send measurements *******************************************************************************************
void DatabaseConverter::sendMeasurements(Value &dstObject, vector<double> &srcMeasurements) {
    Value arrayMeasurements(kArrayType);
    for (double& measurement : srcMeasurements) {
        arrayMeasurements.PushBack(measurement, *allocator);
    }
    dstObject.AddMember("measurements", arrayMeasurements, *allocator);
}
// *****************************************************************************************************************


// sendBaseField
void DatabaseConverter::sendBaseField(Value &dstBaseField, BaseFieldInDatabase &srcBaseField) {
    sendColors(dstBaseField, srcBaseField.colors);
    sendCoords(dstBaseField, "coords_x", srcBaseField.coords_x);
    sendCoords(dstBaseField, "coords_y", srcBaseField.coords_y);
    sendCriterions(dstBaseField, srcBaseField.criterions);
    sendHandles(dstBaseField, srcBaseField.handles);
}


// sendAlignedDimensions
void DatabaseConverter::sendAlignedDimensions(Database &database) {
    Value objectAlignedDimensions(kObjectType);
    sendBaseField(objectAlignedDimensions, database.alignedDimensions);
    sendMeasurements(objectAlignedDimensions, database.alignedDimensions.measurements);
    docOutputDatabase->AddMember("alignedDimensions", objectAlignedDimensions, *allocator);
}


// sendArcDimensions
void DatabaseConverter::sendArcDimensions(Database& database) {
    Value objectArcDimensions(kObjectType);
    sendBaseField(objectArcDimensions, database.arcDimensions);
    docOutputDatabase->AddMember("arcDimensions", objectArcDimensions, *allocator);
}


// sendArcs
void DatabaseConverter::sendArcs(Database &database) {
    Value objectArcs(kObjectType);
    sendBaseField(objectArcs, database.arcs);
    docOutputDatabase->AddMember("arcs", objectArcs, *allocator);
}


// sendAttributes
void DatabaseConverter::sendAttributes(Database &database) {
    Value objectAttributes(kObjectType);
    sendBaseField(objectAttributes, database.attributes);
    sendRotas(objectAttributes, database.attributes.rota); //TODO: 若后续字段有调整，此函数中"rota"应该修改过来
    sendInfos(objectAttributes, database.attributes.info); //TODO: 若后续字段有调整，此函数中"info"应该修改过来
    sendWidthFactors(objectAttributes, database.attributes.widthFactor); //TODO: 若后续字段有调整，此函数中"info"应该修改过来
    sendHorizontalModes(objectAttributes, database.attributes.horizontalModes);
    sendVerticalModes(objectAttributes, database.attributes.verticalModes);
    docOutputDatabase->AddMember("attributes", objectAttributes, *allocator);
}


// sendCircles
void DatabaseConverter::sendCircles(Database &database) {
    Value objectCircles(kObjectType);
    sendBaseField(objectCircles, database.circles);
    docOutputDatabase->AddMember("circles", objectCircles, *allocator);
}


// sendDiametricDimensions
void DatabaseConverter::sendDiametricDimensions(Database &database) {
    Value objectDiametricDimensions(kObjectType);
    sendBaseField(objectDiametricDimensions, database.diametricDimensions);
    docOutputDatabase->AddMember("diametricDimensions", objectDiametricDimensions, *allocator);
}


// sendEllipses
void DatabaseConverter::sendEllipses(Database &database) {
    Value objectEllipses(kObjectType);
    sendBaseField(objectEllipses, database.ellipses);
    docOutputDatabase->AddMember("ellipses", objectEllipses, *allocator);
}


// sendLineAngularDimensions
void DatabaseConverter::sendLineAngularDimensions(Database &database) {
    Value objectLineAngularDimensions(kObjectType);
    sendBaseField(objectLineAngularDimensions, database.lineAngularDimensions);
    docOutputDatabase->AddMember("lineAngularDimensions", objectLineAngularDimensions, *allocator);
}


// sendLines
void DatabaseConverter::sendLines(Database &database) {
    Value objectLines(kObjectType); //能复用吗？
    sendBaseField(objectLines, database.lines);
    docOutputDatabase->AddMember("lines", objectLines, *allocator);
}


// sendMtexts
void DatabaseConverter::sendMtexts(Database &database) {
    Value objectMtexts(kObjectType);
    sendBaseField(objectMtexts, database.mtexts);
    sendRotas(objectMtexts, database.mtexts.rota); //TODO: 若后续字段有调整，此函数中"rota"应该修改过来
    sendInfos(objectMtexts, database.mtexts.info); //TODO: 若后续字段有调整，此函数中"info"应该修改过来
    sendWidthFactors(objectMtexts, database.mtexts.widthFactor); //TODO: 若后续字段有调整，此函数中"info"应该修改过来
    sendHorizontalModes(objectMtexts, database.mtexts.horizontalModes); //TODO: 目前.json文件中，关于mtexts，没有这两个字段，这里只是为了迎合python版本
    sendVerticalModes(objectMtexts, database.mtexts.verticalModes);
    sendLineNums(objectMtexts, database.mtexts.lineNums);
    docOutputDatabase->AddMember("mtexts", objectMtexts, *allocator);
}


// sendOrdinateDimensions
void DatabaseConverter::sendOrdinateDimensions(Database &database) {
    Value objectOrdinateDimensions(kObjectType);
    sendBaseField(objectOrdinateDimensions, database.ordinateDimensions);
    docOutputDatabase->AddMember("ordinateDimensions", objectOrdinateDimensions, *allocator);
}


// sendPoints
void DatabaseConverter::sendPoints(Database &database) {
    Value objectPoints(kObjectType);
    sendBaseField(objectPoints, database.points);
    docOutputDatabase->AddMember("points", objectPoints, *allocator);
}


// sendPolylines
void DatabaseConverter::sendPolylines(Database& database) {
    Value objectPolylines(kObjectType);
    sendBaseField(objectPolylines, database.polylines);
    sendCoords(objectPolylines, "ori_coords_x", database.polylines.ori_coords_x);
    sendCoords(objectPolylines, "ori_coords_y", database.polylines.ori_coords_y);
    docOutputDatabase->AddMember("polylines", objectPolylines, *allocator);
}


// sendRadialDimensionLarges
void DatabaseConverter::sendRadialDimensionLarges(Database& database) {
    Value objectRadialDimensionLarges(kObjectType);
    sendBaseField(objectRadialDimensionLarges, database.radialDimensionLarges);
    docOutputDatabase->AddMember("radialDimensionLarges", objectRadialDimensionLarges, *allocator);
}


// sendRadialDimensions
void DatabaseConverter::sendRadialDimensions(Database &database) {
    Value objectRadialDimensions(kObjectType);
    sendBaseField(objectRadialDimensions, database.radialDimensions);
    docOutputDatabase->AddMember("radialDimensions", objectRadialDimensions, *allocator);
}


// sendRotatedDimensions
void DatabaseConverter::sendRotatedDimensions(Database &database) {
    Value objectRotatedDimensions(kObjectType);
    sendBaseField(objectRotatedDimensions, database.rotatedDimensions);
    docOutputDatabase->AddMember("rotatedDimensions", objectRotatedDimensions, *allocator);
}


// sendSolids
void DatabaseConverter::sendSolids(Database &database) {
    Value objectSolids(kObjectType);
    sendBaseField(objectSolids, database.solids);
    docOutputDatabase->AddMember("solids", objectSolids, *allocator);
}


// sendSplines
void DatabaseConverter::sendSplines(Database &database) {
    Value objectSplines(kObjectType);
    sendBaseField(objectSplines, database.splines);
    docOutputDatabase->AddMember("splines", objectSplines, *allocator);
}


// sendTexts
void DatabaseConverter::sendTexts(Database &database) {
    Value objectTexts(kObjectType);
    sendBaseField(objectTexts, database.texts);
    sendRotas(objectTexts, database.texts.rota); //TODO: 若后续字段有调整，此函数中"rota"应该修改过来
    sendInfos(objectTexts, database.texts.info); //TODO: 若后续字段有调整，此函数中"info"应该修改过来
    sendWidthFactors(objectTexts, database.texts.widthFactor); //TODO: 若后续字段有调整，此函数中"info"应该修改过来
    sendHorizontalModes(objectTexts, database.texts.horizontalModes);
    sendVerticalModes(objectTexts, database.texts.verticalModes);
    docOutputDatabase->AddMember("texts", objectTexts, *allocator);
}


void DatabaseConverter::database2jsonstr(Database &database) {
    // 统计时间
    // timer t_ogm;
    clock_t start = clock();

    // 打印提示信息 //TODO 应该做成装饰模式
    cout << "[INFO] database2jsonstr ..." << endl;

    // 实例化Document对象
    initDocOutputDatabase();

    // alignedDimensions
    sendAlignedDimensions(database);

    // arcDimensions
    sendArcDimensions(database);

    // arcs
    sendArcs(database);

    // attributes
    sendAttributes(database);

    // circles
    sendCircles(database);

    // diametricDimensions
    sendDiametricDimensions(database);

    // ellipses
    sendEllipses(database);

    // lineAngularDimensions
    sendLineAngularDimensions(database);

    // lines
    sendLines(database);

    // mtexts
    sendMtexts(database);

    // ordinateDimensions
    sendOrdinateDimensions(database);

    // points
    sendPoints(database);

    // polylines
    sendPolylines(database);

    // radialDimensionLarges
    sendRadialDimensionLarges(database);

    // radialDimensions
    sendRadialDimensions(database);

    // rotatedDimensions
    sendRotatedDimensions(database);

    // solids
    sendSolids(database);

    // splines
    sendSplines(database);

    // texts
    sendTexts(database);

    // 形成jsonstr
    StringBuffer s;
    Writer<StringBuffer> writer(s);
    docOutputDatabase->Accept(writer);
    outputDatabase = string(s.GetString());

    // 统计时间
    // double time_cost = t_ogm.elapsed(); //秒
    double time_cost = (double)(clock() - start) / CLOCKS_PER_SEC; //秒
    cout << "[INFO] database2jsonstr done, " << time_cost << "s has elapsed" << endl; //TODO: 装饰器怎么写出来？
}


// ********************************************************************************************************************
// ********************************************************************************************************************


// *** init docInputDatabase *******************************
void DatabaseConverter::initDocInputDatabase() {
    // 实例化指针所指向的对象
    docInputDatabase = new Document();
}
// *********************************************************


// *** get colors *************************************************************************************************
void DatabaseConverter::getColors(vector<string>& dstColors, Value& srcColors) {
    if (!srcColors.IsArray()) {
        printf("[ERROR] This should be an array");
    }
    for (int i = 0; i < srcColors.Size(); ++i) {
        dstColors.emplace_back(srcColors[i].GetString()); //push_back=>emplace_back
    }
}
// ****************************************************************************************************************


// *** get coords *************************************************************************************************
void DatabaseConverter::getAtom(Atom& dstAtom, Value& srcAtom) {
    if (!srcAtom.IsArray()) {
        printf("[ERROR] This should be an array");
    }
    for (int i = 0; i < srcAtom.Size(); ++i) {
        dstAtom.push_back(srcAtom[i].GetDouble());
    }
}

void DatabaseConverter::getCoord(Coord& dstCoord, Value& srcCoord) {
    if (!srcCoord.IsArray()) {
        printf("[ERROR] This should be an array");
    }
    for (int i = 0; i < srcCoord.Size(); ++i) {
        Atom resAtom{};
        getAtom(resAtom, srcCoord[i]);
        dstCoord.push_back(resAtom);
    }
}

void DatabaseConverter::getCoords(Coords& dstCoords, Value& srcCoords) {
    if (!srcCoords.IsArray()) {
        printf("[ERROR] This should be an array");
    }
    for (int i = 0; i < srcCoords.Size(); ++i) {
        Coord resCoord{};
        getCoord(resCoord, srcCoords[i]);
        dstCoords.push_back(resCoord);
    }
}
// ****************************************************************************************************************


// *** get criterions *********************************************************************************************
void DatabaseConverter::getBRHandleHistory(BLockReferenceHandleHistory &dstBRHandleHistory, Value &srcBRHandleHistory) {
    if (!srcBRHandleHistory.IsArray()) {
        printf("[ERROR] This should be an array");
    }
    for (int i = 0; i < srcBRHandleHistory.Size(); ++i) {
        dstBRHandleHistory.push_back(srcBRHandleHistory[i].GetString());
    }
}

void DatabaseConverter::getBelongToWhichBlock(string &dstBelongToWhichBlock, Value &srcBelongToWhichBlock) {
    dstBelongToWhichBlock = srcBelongToWhichBlock.GetString();
}

void DatabaseConverter::getIsDynHistory(IsDynHistory& dstIsDynHistory, Value& srcIsDynHistory) {
    if (!srcIsDynHistory.IsArray()) {
        printf("[ERROR] This should be an array");
    }
    for (int i = 0; i < srcIsDynHistory.Size(); ++i) {
        dstIsDynHistory.push_back(srcIsDynHistory[i].GetBool());
    }
}

void DatabaseConverter::getIsFromHatchHistory(IsFromHatchHistory &dstIsFromHatchHistory, Value &srcIsFromHatchHistory) {
    if (!srcIsFromHatchHistory.IsArray()) {
        printf("[ERROR] This should be an array");
    }
    for (int i = 0; i < srcIsFromHatchHistory.Size(); ++i) {
        dstIsFromHatchHistory.push_back(srcIsFromHatchHistory[i].GetBool());
    }
}

void DatabaseConverter::getLayerHistory(LayerHistory &dstLayerHistory, Value &srcLayerHistory) {
    if (!srcLayerHistory.IsArray()) {
        printf("[ERROR] This should be an array");
    }
    for (int i = 0; i < srcLayerHistory.Size(); ++i) {
        dstLayerHistory.push_back(srcLayerHistory[i].GetString());
    }
}

void DatabaseConverter::getPedigreeLogger(PedigreeLogger &dstPedigreeLogger, Value &srcPedigreeLogger) {
    if (!srcPedigreeLogger.IsArray()) {
        printf("[ERROR] This should be an array");
    }
    for (int i = 0; i < srcPedigreeLogger.Size(); ++i) {
        dstPedigreeLogger.push_back(srcPedigreeLogger[i].GetString());
    }
}

void DatabaseConverter::getVisibleHistory(VisibleHistory &dstVisibleHistory, Value &srcVisibleHistory) {
    if (!srcVisibleHistory.IsArray()) {
        printf("[ERROR] This should be an array");
    }
    for (int i = 0; i < srcVisibleHistory.Size(); ++i) {
        dstVisibleHistory.push_back(srcVisibleHistory[i].GetBool());
    }
}

void DatabaseConverter::getCriterion(Criterion& dstCriterion, Value& srcCriterion) {
    if (!srcCriterion.IsObject()) {
        printf("[ERROR] This should be an object");
    }
    getBRHandleHistory(dstCriterion.BRHandleHistory, srcCriterion["BRHandleHistory"]);
    getBelongToWhichBlock(dstCriterion.belongToWhichBlock, srcCriterion["belongToWhichBlock"]);
    getIsDynHistory(dstCriterion.isDynHistory, srcCriterion["isDynHistory"]);
    getIsFromHatchHistory(dstCriterion.isFromHatchHistory, srcCriterion["isFromHatchHistory"]);
    getLayerHistory(dstCriterion.layerHistory, srcCriterion["layerHistory"]);
    getPedigreeLogger(dstCriterion.pedigreeLogger, srcCriterion["pedigreeLogger"]);
    getVisibleHistory(dstCriterion.visibleHistory, srcCriterion["visibleHistory"]);
}

void DatabaseConverter::getCriterions(vector<Criterion> &dstCriterions, Value &srcCriterions) {
    if (!srcCriterions.IsArray()) {
        printf("[ERROR] This should be an array");
    }
    for (int i = 0; i < srcCriterions.Size(); ++i) {
        Criterion resCriterion{};
        getCriterion(resCriterion, srcCriterions[i]);
        dstCriterions.push_back(resCriterion);
    }
}
// ****************************************************************************************************************


// *** get handles ************************************************************************************************
void DatabaseConverter::getHandles(vector<string>& dstHandles, Value& srcHandles) {
    if (!srcHandles.IsArray()) {
        printf("[ERROR] This should be an array");
    }
    for (int i = 0; i < srcHandles.Size(); ++i) {
        dstHandles.emplace_back(srcHandles[i].GetString());
    }
}
// ****************************************************************************************************************


// *** get Infos **************************************************************************************************
void DatabaseConverter::getInfos(vector<string> &dstInfos, Value &srcInfos) {
    if (!srcInfos.IsArray()) {
        printf("[ERROR] This should be an array"); //TODO: 与json相关源码中的这些[ERROR]应该变成throw runtime_error("[ERROR]")
    }
    for (int i = 0; i < srcInfos.Size(); ++i) {
        dstInfos.emplace_back(srcInfos[i].GetString());
    }
}
// ****************************************************************************************************************


// *** get Rotas **************************************************************************************************
void DatabaseConverter::getRotas(vector<double> &dstRotas, Value &srcRotas) {
    if (!srcRotas.IsArray()) {
        printf("[ERROR] This should be an array");
    }
    for (int i = 0; i < srcRotas.Size(); ++i) {
        dstRotas.emplace_back(srcRotas[i].GetDouble());
    }
}
// ****************************************************************************************************************


// *** get widthFactors *******************************************************************************************
void DatabaseConverter::getWidthFactors(vector<double> &dstWidthFactors, Value &srcWidthFactors) {
    if (!srcWidthFactors.IsArray()) {
        printf("[ERROR] This should be an array");
    }
    for (int i = 0; i < srcWidthFactors.Size(); ++i) {
        dstWidthFactors.emplace_back(srcWidthFactors[i].GetDouble());
    }
}
// ****************************************************************************************************************


// *** get horizontalModes ****************************************************************************************
void DatabaseConverter::getHorizontalModes(vector<string> &dstHorizontalModes, Value &srcHorizontalModes) {
    if (!srcHorizontalModes.IsArray()) {
        printf("[ERROR] This should be an array");
    }
    for (int i = 0; i < srcHorizontalModes.Size(); ++i) {
        dstHorizontalModes.emplace_back(srcHorizontalModes[i].GetString());
    }
}
// ****************************************************************************************************************


// *** get verticalModes ******************************************************************************************
void DatabaseConverter::getVerticalModes(vector<string> &dstVerticalModes, Value &srcVerticalModes) {
    if (!srcVerticalModes.IsArray()) {
        printf("[ERROR] This should be an array");
    }
    for (int i = 0; i < srcVerticalModes.Size(); ++i) {
        dstVerticalModes.emplace_back(srcVerticalModes[i].GetString());
    }
}
// ****************************************************************************************************************


// *** get lineNums ***********************************************************************************************
void DatabaseConverter::getLineNums(vector<int> &dstLineNums, Value &srcLineNums) {
    if (!srcLineNums.IsArray()) {
        printf("[ERROR] This should be an array");
    }
    for (int i = 0; i < srcLineNums.Size(); ++i) {
        dstLineNums.emplace_back(srcLineNums[i].GetInt());
    }
}
// ****************************************************************************************************************


// *** get measurements *******************************************************************************************
void DatabaseConverter::getMeasurements(vector<double> &dstMeasurements, Value &srcMeasurements) {
    if (!srcMeasurements.IsArray()) {
        printf("[ERROR] This should be an array");
    }
    for (int i = 0; i < srcMeasurements.Size(); ++i) {
        dstMeasurements.emplace_back(srcMeasurements[i].GetDouble());
    }
}
// ****************************************************************************************************************


void DatabaseConverter::getBaseField(BaseFieldInDatabase &dstBaseField, Value *srcBaseField) {
    /*
     * 获得lines, polylines等图元共有的基本字段的数据
     * 这些基本字段包括colors, coords_x, coords_y, criterions, handles
     * 如果某图元含有其它字段数据，则在其get函数中另做
     * BaseFieldInDatabase是lines, polylines等图元的基类
     */
    // colors
    if (srcBaseField->HasMember("colors")) getColors(dstBaseField.colors, (*srcBaseField)["colors"]);

    // coords_x
    if (srcBaseField->HasMember("coords_x")) getCoords(dstBaseField.coords_x, (*srcBaseField)["coords_x"]);

    // coords_y
    if (srcBaseField->HasMember("coords_y")) getCoords(dstBaseField.coords_y, (*srcBaseField)["coords_y"]);

    // criterions
    if (srcBaseField->HasMember("criterions")) getCriterions(dstBaseField.criterions, (*srcBaseField)["criterions"]);

    // handles
    if (srcBaseField->HasMember("handles")) getHandles(dstBaseField.handles, (*srcBaseField)["handles"]);
}


// getAlignedDimensions
void DatabaseConverter::getAlignedDimensions() {
    Value* alignedDimensions{}; if (docInputDatabase->HasMember("alignedDimensions")) alignedDimensions = &((*docInputDatabase)["alignedDimensions"]);
    if (!alignedDimensions->IsObject()) {
        printf("[ERROR] This should be an object");
    }
    getBaseField(inputDatabase.alignedDimensions, alignedDimensions);
    assert(alignedDimensions->HasMember("measurements")); getMeasurements(inputDatabase.alignedDimensions.measurements, (*alignedDimensions)["measurements"]);
}


// getArcDimensions
void DatabaseConverter::getArcDimensions() {
    Value* arcDimensions{}; if (docInputDatabase->HasMember("arcDimensions")) arcDimensions = &((*docInputDatabase)["arcDimensions"]);
    if (!arcDimensions->IsObject()) {
        printf("[ERROR] This should be an object");
    }
    getBaseField(inputDatabase.arcDimensions, arcDimensions);
}


// getArcs
void DatabaseConverter::getArcs() {
    Value* arcs{}; if (docInputDatabase->HasMember("arcs")) arcs = &((*docInputDatabase)["arcs"]);
    if (!arcs->IsObject()) {
        printf("[ERROR] This should be an object");
    }
    getBaseField(inputDatabase.arcs, arcs);
}


// getAttributes
void DatabaseConverter::getAttributes() {
    Value* attributes{}; if (docInputDatabase->HasMember("attributes")) attributes = &((*docInputDatabase)["attributes"]);
    if (!attributes->IsObject()) {
        printf("[ERROR] This should be an objcet");
    }
    getBaseField(inputDatabase.attributes, attributes);
    assert(attributes->HasMember("rota")); getRotas(inputDatabase.attributes.rota, (*attributes)["rota"]);
    assert(attributes->HasMember("info")); getInfos(inputDatabase.attributes.info, (*attributes)["info"]);
    assert(attributes->HasMember("widthFactor")); getWidthFactors(inputDatabase.attributes.widthFactor, (*attributes)["widthFactor"]);
    assert(attributes->HasMember("horizontalModes")); getHorizontalModes(inputDatabase.attributes.horizontalModes, (*attributes)["horizontalModes"]);
    assert(attributes->HasMember("verticalModes")); getVerticalModes(inputDatabase.attributes.verticalModes, (*attributes)["verticalModes"]);
}


// getCircles
void DatabaseConverter::getCircles() {
    Value* circles{}; if (docInputDatabase->HasMember("circles")) circles = &((*docInputDatabase)["circles"]);
    if (!circles->IsObject()) {
        printf("[ERROR] This should be an objcet");
    }
    getBaseField(inputDatabase.circles, circles);
}


// getDiametricDimensions
void DatabaseConverter::getDiametricDimensions() {
    Value* diametricDimensions{}; if (docInputDatabase->HasMember("diametricDimensions")) diametricDimensions = &((*docInputDatabase)["diametricDimensions"]);
    if (!diametricDimensions->IsObject()) {
        printf("[ERROR] This should be an objcet");
    }
    getBaseField(inputDatabase.diametricDimensions, diametricDimensions);
}


// getEllipses
void DatabaseConverter::getEllipses() {
    Value* ellipses{}; if (docInputDatabase->HasMember("ellipses")) ellipses = &((*docInputDatabase)["ellipses"]);
    if (!ellipses->IsObject()) {
        printf("[ERROR] This should be an objcet");
    }
    getBaseField(inputDatabase.ellipses, ellipses);
}


// getLineAngularDimensions
void DatabaseConverter::getLineAngularDimensions() {
    Value* lineAngularDimensions{}; if (docInputDatabase->HasMember("lineAngularDimensions")) lineAngularDimensions = &((*docInputDatabase)["lineAngularDimensions"]);
    if (!lineAngularDimensions->IsObject()) {
        printf("[ERROR] This should be an objcet");
    }
    getBaseField(inputDatabase.lineAngularDimensions, lineAngularDimensions);
}


// getLines
void DatabaseConverter::getLines() {
    Value* lines{}; if (docInputDatabase->HasMember("lines")) lines = &((*docInputDatabase)["lines"]);
    if (!lines->IsObject()) {
        printf("[ERROR] This should be an object");
    }
    getBaseField(inputDatabase.lines, lines);
}


// getMtexts
void DatabaseConverter::getMtexts() {
    Value* mtexts{}; if (docInputDatabase->HasMember("mtexts")) mtexts = &((*docInputDatabase)["mtexts"]);
    if (!mtexts->IsObject()) {
        printf("[ERROR] This should be an object");
    }
    getBaseField(inputDatabase.mtexts, mtexts);
    assert(mtexts->HasMember("rota")); getRotas(inputDatabase.mtexts.rota, (*mtexts)["rota"]);
    assert(mtexts->HasMember("info")); getInfos(inputDatabase.mtexts.info, (*mtexts)["info"]);
    assert(mtexts->HasMember("widthFactor")); getWidthFactors(inputDatabase.mtexts.widthFactor, (*mtexts)["widthFactor"]);
    assert(mtexts->HasMember("horizontalModes")); getHorizontalModes(inputDatabase.mtexts.horizontalModes, (*mtexts)["horizontalModes"]); //TODO: 目前.json文件中，关于mtexts，没有这两个字段，这里只是为了迎合python版本
    assert(mtexts->HasMember("verticalModes")); getVerticalModes(inputDatabase.mtexts.verticalModes, (*mtexts)["verticalModes"]);
    assert(mtexts->HasMember("lineNums")); getLineNums(inputDatabase.mtexts.lineNums, (*mtexts)["lineNums"]);
}


// getOrdinateDimensions
void DatabaseConverter::getOrdinateDimensions() {
    Value* ordinateDimensions{}; if (docInputDatabase->HasMember("ordinateDimensions")) ordinateDimensions = &((*docInputDatabase)["ordinateDimensions"]);
    if (!ordinateDimensions->IsObject()) {
        printf("[ERROR] This should be an object");
    }
    getBaseField(inputDatabase.ordinateDimensions, ordinateDimensions);
}


// getPoints
void DatabaseConverter::getPoints() {
    Value* points{}; if (docInputDatabase->HasMember("points")) points = &((*docInputDatabase)["points"]);
    if (!points->IsObject()) {
        printf("[ERROR] This should be an object");
    }
    getBaseField(inputDatabase.points, points);
}


// getPolylines
void DatabaseConverter::getPolylines() {
    Value* polylines{}; if (docInputDatabase->HasMember("polylines")) polylines = &((*docInputDatabase)["polylines"]);
    if (!polylines->IsObject()) {
        printf("[ERROR] This should be an object");
    }
    getBaseField(inputDatabase.polylines, polylines);
    if (polylines->HasMember("ori_coords_x")) getCoords(inputDatabase.polylines.ori_coords_x, (*polylines)["ori_coords_x"]);
    if (polylines->HasMember("ori_coords_y")) getCoords(inputDatabase.polylines.ori_coords_y, (*polylines)["ori_coords_y"]);
}


// getRadialDimensionLarges
void DatabaseConverter::getRadialDimensionLarges() {
    Value* radialDimensionLarges{}; if (docInputDatabase->HasMember("radialDimensionLarges")) radialDimensionLarges = &((*docInputDatabase)["radialDimensionLarges"]);
    if (!radialDimensionLarges->IsObject()) {
        printf("[ERROR] This should be an object");
    }
    getBaseField(inputDatabase.radialDimensionLarges, radialDimensionLarges);
}


// getRadialDimensions
void DatabaseConverter::getRadialDimensions() {
    Value* radialDimensions{}; if (docInputDatabase->HasMember("radialDimensions")) radialDimensions = &((*docInputDatabase)["radialDimensions"]);
    if (!radialDimensions->IsObject()) {
        printf("[ERROR] This should be an object");
    }
    getBaseField(inputDatabase.radialDimensions, radialDimensions);
}


// getRotatedDimensions
void DatabaseConverter::getRotatedDimensions() {
    Value* rotatedDimensions{}; if (docInputDatabase->HasMember("rotatedDimensions")) rotatedDimensions = &((*docInputDatabase)["rotatedDimensions"]);
    if (!rotatedDimensions->IsObject()) {
        printf("[ERROR] This should be an object");
    }
    getBaseField(inputDatabase.rotatedDimensions, rotatedDimensions);
}


// getSolids
void DatabaseConverter::getSolids() {
    Value* solids{}; if (docInputDatabase->HasMember("solids")) solids = &((*docInputDatabase)["solids"]);
    if (!solids->IsObject()) {
        printf("[ERROR] This should be an object");
    }
    getBaseField(inputDatabase.solids, solids);
}


// getSplines
void DatabaseConverter::getSplines() {
    Value* splines{}; if (docInputDatabase->HasMember("splines")) splines = &((*docInputDatabase)["splines"]);
    if (!splines->IsObject()) {
        printf("[ERROR] This should be an object");
    }
    getBaseField(inputDatabase.splines, splines);
}


// getTexts
void DatabaseConverter::getTexts() {
    Value* texts{}; if (docInputDatabase->HasMember("texts")) texts = &((*docInputDatabase)["texts"]);
    if (!texts->IsObject()) {
        printf("[ERROR] This should be an object");
    }
    getBaseField(inputDatabase.texts, texts);
    assert(texts->HasMember("rota")); getRotas(inputDatabase.texts.rota, (*texts)["rota"]);
    assert(texts->HasMember("info")); getInfos(inputDatabase.texts.info, (*texts)["info"]);
    assert(texts->HasMember("widthFactor")); getWidthFactors(inputDatabase.texts.widthFactor, (*texts)["widthFactor"]);
    assert(texts->HasMember("horizontalModes")); getHorizontalModes(inputDatabase.texts.horizontalModes, (*texts)["horizontalModes"]);
    assert(texts->HasMember("verticalModes")); getVerticalModes(inputDatabase.texts.verticalModes, (*texts)["verticalModes"]);
}


void DatabaseConverter::jsonstr2database(string& jsonstr) {
    // 统计时间
    // timer t_ogm;
    clock_t start = clock();

    // 打印提示信息 //TODO 应该做成装饰模式
    cout << "[INFO] jsonstr2database ..." << endl;

    // 实例化Document对象
    initDocInputDatabase();

    // 填充doc中的内容
    assert(!(docInputDatabase->Parse(jsonstr.c_str()).HasParseError())); //TODO: 这一句好像必须得有，因为有Parse

    // alignedDimensions
    getAlignedDimensions();

    // arcDimensions
    getArcDimensions();

    // arcs
    getArcs();

    // attributes
    getAttributes();

    // circles
    getCircles();

    // diametricDimensions
    getDiametricDimensions();

    // ellipses
    getEllipses();

    // lineAngularDimensions
    getLineAngularDimensions();

    // lines
    getLines();

    // mtexts
    getMtexts();

    // ordinateDimensions
    getOrdinateDimensions();

    // points
    getPoints();

    // polylines
    getPolylines();

    // radialDimensionLarges
    getRadialDimensionLarges();

    // radialDimensions
    getRadialDimensions();

    // rotatedDimensions
    getRotatedDimensions();

    // solids
    getSolids();

    // splines
    getSplines();

    // texts
    getTexts();

    // 统计时间
    // double time_cost = t_ogm.elapsed(); //秒
    double time_cost = (double)(clock() - start) / CLOCKS_PER_SEC; //秒
    cout << "[INFO] jsonstr2database done, " << time_cost << "s has elapsed" << endl; //TODO: 装饰器怎么写出来？
}
