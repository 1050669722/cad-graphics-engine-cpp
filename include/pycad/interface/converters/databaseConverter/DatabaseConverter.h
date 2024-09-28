//
// Created by liuxingbo03 on 2021/12/13.
//

#ifndef ATUOCADDATAANALYSISPYCADCPP_DATABASECONVERTER_H
#define ATUOCADDATAANALYSISPYCADCPP_DATABASECONVERTER_H


// #include <iostream>
// #include <iomanip>
// #include "nlohmann/json.hpp"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"

#include "pycad/Structs.h"
#include "pycad/Database.h"


using namespace std;
// using json = nlohmann::json;
using namespace rapidjson;


class DatabaseConverter {
private:
    // 成员属性
    Document* docOutputDatabase = nullptr;
    Document::AllocatorType* allocator = nullptr; //分配器指针，专门为docOutputDatabase设计

    Document* docInputDatabase = nullptr;

    // 成员方法
    // *** init docOutputDatabase ***
    void initDocOutputDatabase();
    // ******************************

    // *** send colors ***
    void sendColors(Value& dstObject, vector<string>& srcColors);
    // *******************

    // *** send coords ***
    void sendAtom(Value& dstArrayAtom, Atom& srcAtom);
    void sendCoord(Value& dstArrayCoord, Coord& srcCoord);
    void sendCoords(Value &dstObject, const char* fieldName, Coords& srcCoords);
    // *******************

    // *** send criterions ***
    void sendBRHandleHistory(Value& dstObject, BLockReferenceHandleHistory& srcBRHandleHistory);
    void sendBelongToWhichBlock(Value& dstObject, string& srcBelongToWhichBlock);
    void sendIsDynHistory(Value& dstObject, IsDynHistory& srcIsDynHistory);
    void sendIsFromHatchHistory(Value& dstObject, IsFromHatchHistory& srcIsFromHatchHistory);
    void sendLayerHistory(Value& dstObject, LayerHistory& srcLayerHistory);
    void sendPedigreeLogger(Value& dstObject, PedigreeLogger& srcPedigreeLogger);
    void sendVisibleHistory(Value& dstObject, VisibleHistory& srcVisibleHistory);
    void sendCriterion(Value& dstCriterion, Criterion& srcCriterion);
    void sendCriterions(Value &dstObject, vector<Criterion>& srcCriterions);
    // ***********************

    // *** send handles ***
    void sendHandles(Value& dstObject, vector<string>& srcHandles);
    // ********************

    // *** send infos *****
    void sendInfos(Value& dstObject, vector<string>& srcInfos);
    // ********************

    // *** send rotas *****
    void sendRotas(Value& dstObject, vector<double>& srcRotas);
    // ********************

    // *** send widthFactors ****
    void sendWidthFactors(Value& dstObject, vector<double>& srcWidthFactors);
    // **************************

    // *** send horizontalModes ****
    void sendHorizontalModes(Value& dstObject, vector<string>& srcHorizontalModes);
    // *****************************

    // *** send verticalModes ******
    void sendVerticalModes(Value& dstObject, vector<string>& srcVerticalModes);
    // *****************************

    // *** send lineNums **********
    void sendLineNums(Value& dstObject, vector<int>& srcLineNums);
    // ****************************

    // *** send measurements ******
    void sendMeasurements(Value& dstObject, vector<double>& srcMeasurements);
    // ****************************

    // sendBaseField
    void sendBaseField(Value& dstBaseField, BaseFieldInDatabase& srcBaseField);

    // sendAlignedDimensions
    void sendAlignedDimensions(Database& database);

    // sendArcDimensions
    void sendArcDimensions(Database& database);

    // sendArcs
    void sendArcs(Database& database);

    // sendAttributes
    void sendAttributes(Database& database);

    // sendCircles
    void sendCircles(Database& database);

    // sendDiametricDimensions
    void sendDiametricDimensions(Database& database);

    // sendEllipses
    void sendEllipses(Database& database);

    // sendLineAngularDimensions
    void sendLineAngularDimensions(Database& database);

    // sendLines
    void sendLines(Database& database);

    // sendMtexts
    void sendMtexts(Database& database);

    // sendOrdinateDimensions
    void sendOrdinateDimensions(Database& database);

    // sendPoints
    void sendPoints(Database& database);

    // sendPolylines
    void sendPolylines(Database& database);

    // sendRadialDimensionLarges
    void sendRadialDimensionLarges(Database& database);

    // sendRadialDimensions
    void sendRadialDimensions(Database& database);

    // sendRotatedDimensions
    void sendRotatedDimensions(Database& database);

    // sendSolids
    void sendSolids(Database& database);

    // sendSplines
    void sendSplines(Database& database);

    // sendTexts
    void sendTexts(Database& database);

    // ****************************************************************************************************************
    // ****************************************************************************************************************

    // *** init docInputDatabase ***
    void initDocInputDatabase();
    // *****************************

    // *** get colors ***
    void getColors(vector<string>& dstColors, Value& srcColors);
    // ******************

    // *** get coords ***
    void getAtom(Atom& dstAtom, Value& srcAtom);
    void getCoord(Coord& dstCoord, Value& srcCoord);
    void getCoords(Coords& dstCoords, Value& srcCoords);
    // ******************

    // *** get criterions ***
    void getBRHandleHistory(BLockReferenceHandleHistory& dstBRHandleHistory, Value& srcBRHandleHistory);
    void getBelongToWhichBlock(string& dstBelongToWhichBlock, Value& srcBelongToWhichBlock);
    void getIsDynHistory(IsDynHistory& dstIsDynHistory, Value& srcIsDynHistory);
    void getIsFromHatchHistory(IsFromHatchHistory& dstIsFromHatchHistory, Value& srcIsFromHatchHistory);
    void getLayerHistory(LayerHistory& dstLayerHistory, Value& srcLayerHistory);
    void getPedigreeLogger(PedigreeLogger& dstPedigreeLogger, Value& srcPedigreeLogger);
    void getVisibleHistory(VisibleHistory& dstVisibleHistory, Value& srcVisibleHistory);
    void getCriterion(Criterion& dstCriterion, Value& srcCriterion);
    void getCriterions(vector<Criterion>& dstCriterions, Value& srcCriterions);
    // ***********************

    // *** get handles ***
    void getHandles(vector<string>& dstHandles, Value& srcHandles);
    // *******************

    // *** get infos *****
    void getInfos(vector<string>& dstInfos, Value& srcInfos);
    // *******************

    // *** get rotas *****
    void getRotas(vector<double>& dstRotas, Value& srcRotas);
    // *******************

    // *** get widthFactors ****
    void getWidthFactors(vector<double>& dstWidthFactors, Value& srcWidthFactors);
    // *************************

    // *** get horizontalModes ****
    void getHorizontalModes(vector<string>& dstHorizontalModes, Value& srcHorizontalModes);
    // ****************************

    // *** get verticalModes ******
    void getVerticalModes(vector<string>& dstVerticalModes, Value& srcVerticalModes);
    // ****************************

    // *** get lineNums ***********
    void getLineNums(vector<int>& dstLineNums, Value& srcLineNums);
    // ****************************

    // *** get measurements *******
    void getMeasurements(vector<double>& dstMeasurements, Value& srcMeasurements);
    // ****************************

    // getBaseField
    void getBaseField(BaseFieldInDatabase& dstBaseField, Value* srcBaseField);

    // getAlignedDimensions
    void getAlignedDimensions();

    // getArcDimensions
    void getArcDimensions();

    // getArcs
    void getArcs();

    // getAttributes
    void getAttributes();

    // getCircles
    void getCircles();

    // getDiametricDimensions
    void getDiametricDimensions();

    // getEllipses
    void getEllipses();

    // getLineAngularDimensions
    void getLineAngularDimensions();

    // getLines
    void getLines();

    // getMtexts
    void getMtexts();

    // getOrdinateDimensions
    void getOrdinateDimensions();

    // getPoints
    void getPoints();

    // getPolylines
    void getPolylines();

    // getRadialDimensionLarges
    void getRadialDimensionLarges();

    // getRadialDimensions
    void getRadialDimensions();

    // getRotatedDimensions
    void getRotatedDimensions();

    // getSolids
    void getSolids();

    // getSplines
    void getSplines();

    // getTexts
    void getTexts();

public:
    // 成员属性 //const char* 出去之后就不对了？
    string outputDatabase{}; //database2jsonstr //cpp层的database传输到外部成为jsonstr
    Database inputDatabase{}; //jsonstr2database //从外部传入的jsonstr转换为cpp层的database

    // 成员方法
    DatabaseConverter();
    ~DatabaseConverter();
    void database2jsonstr(Database &database);
    void jsonstr2database(string& jsonstr); //const char* jsonstr
};


#endif //ATUOCADDATAANALYSISPYCADCPP_DATABASECONVERTER_H
