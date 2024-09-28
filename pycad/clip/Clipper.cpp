//
// Created by nan on 2021/11/17.
//

// #include <boost/timer.hpp>
#include <ctime>
#include "pycad/clip/Clipper.h"


// using namespace boost;
using namespace std;


Clipper::Clipper() = default;


Clipper::Clipper(Configuration &config) { //, Database &database) {
    this->config = &config;

    // 实例化SutherlandHodgmanClip
    sutherlandHodgman = SutherlandHodgman(config);
    weilerAtherton = WeilerAtherton(config);
}


void Clipper::generateNewDatabase(Database& database) {
    newDatabase = database;
}


void Clipper::clipGeneratedNewDatabaseCore_WeilerAtherton(BaseFieldInDatabase &newDatabase_field) {
    // 多段线使用Weiler_Atherton算法，使用weiler_Atherton可以获得封闭的矩形区域
    if (config->insideOrOutside == "Inside") {
        weilerAtherton.clipCoords(newDatabase_field.coords_x, newDatabase_field.coords_y);
    }
    else if (config->insideOrOutside == "Outside") {
        weilerAtherton.clipCoords_reverse(newDatabase_field.coords_x, newDatabase_field.coords_y);
    }
    else {
        printf(R"([WARNING] the filed "insideOrOutside" of config must be "Inside" or "Outside", here got "%s".)", config->insideOrOutside.c_str());
    }
}


void Clipper::clipGeneratedNewDatabaseCore_SutherlandHodgman(BaseFieldInDatabase &newDatabase_field) {
    // 除多段线外使用Sutherland_Hodgman算法
    if (config->insideOrOutside == "Inside") {
        sutherlandHodgman.clipCoords(newDatabase_field.coords_x, newDatabase_field.coords_y);
    }
    else if (config->insideOrOutside == "Outside") {
        sutherlandHodgman.clipCoords_reverse(newDatabase_field.coords_x, newDatabase_field.coords_y);
    }
    else {
        printf(R"([WARNING] the filed "insideOrOutside" of config must be "Inside" or "Outside", here got "%s".)", config->insideOrOutside.c_str());
    }
}


void Clipper::clipGeneratedNewDatabaseCore_textAnalogue_SutherlandHodgman(BaseFieldInDatabase &newDatabase_field) {
    // 除多段线外使用Sutherland_Hodgman算法 //特别处理文本类图元
    if (config->insideOrOutside == "Inside") {
        sutherlandHodgman.clipCoords_textAnalogue(newDatabase_field.coords_x, newDatabase_field.coords_y);
    }
    else if (config->insideOrOutside == "Outside") {
        sutherlandHodgman.clipCoords_textAnalogue_reverse(newDatabase_field.coords_x, newDatabase_field.coords_y);
    }
    else {
        printf(R"([WARNING] the filed "insideOrOutside" of config must be "Inside" or "Outside", here got "%s".)", config->insideOrOutside.c_str());
    }
}


void Clipper::clipDatabase(Database &database) { //原名：clip::readgraphics
    // 统计时间
    // timer t_ogm;
    clock_t start = clock();

    // 打印提示信息 //TODO 应该做成装饰模式
    cout << "[INFO] clipDatabase ..." << endl;

    // 获取一个备份 | 因为要保全原有的database，所以现在重写一个新的database；
    generateNewDatabase(database);

    // 若region为空，不筛选
    if ((config->region).empty()) {
        return;
    }

    // 若region不为空，则其长度必为4 | xmin, ymin, xmax, ymax
    assert((config->region).size() == 4);

    // 多段线使用Weiler_Atherton算法，使用weiler_Atherton可以获得封闭的矩形区域
    clipGeneratedNewDatabaseCore_WeilerAtherton(newDatabase.polylines);

    // 除多段线外使用Sutherland_Hodgman算法
    clipGeneratedNewDatabaseCore_SutherlandHodgman(newDatabase.lines);
    clipGeneratedNewDatabaseCore_SutherlandHodgman(newDatabase.circles);
    clipGeneratedNewDatabaseCore_SutherlandHodgman(newDatabase.arcs);
    clipGeneratedNewDatabaseCore_SutherlandHodgman(newDatabase.ellipses);
    clipGeneratedNewDatabaseCore_SutherlandHodgman(newDatabase.points);
    clipGeneratedNewDatabaseCore_SutherlandHodgman(newDatabase.splines);
    clipGeneratedNewDatabaseCore_SutherlandHodgman(newDatabase.solids);
    clipGeneratedNewDatabaseCore_SutherlandHodgman(newDatabase.alignedDimensions);
    clipGeneratedNewDatabaseCore_SutherlandHodgman(newDatabase.rotatedDimensions);
    clipGeneratedNewDatabaseCore_SutherlandHodgman(newDatabase.lineAngularDimensions);
    clipGeneratedNewDatabaseCore_SutherlandHodgman(newDatabase.arcDimensions);
    clipGeneratedNewDatabaseCore_SutherlandHodgman(newDatabase.radialDimensions);
    clipGeneratedNewDatabaseCore_SutherlandHodgman(newDatabase.diametricDimensions);
    clipGeneratedNewDatabaseCore_SutherlandHodgman(newDatabase.ordinateDimensions);
    clipGeneratedNewDatabaseCore_SutherlandHodgman(newDatabase.radialDimensionLarges);

    clipGeneratedNewDatabaseCore_textAnalogue_SutherlandHodgman(newDatabase.texts); //TODO: 需要特殊处理，划到就显示，或者划到就不显示
    clipGeneratedNewDatabaseCore_textAnalogue_SutherlandHodgman(newDatabase.mtexts); //TODO: 需要特殊处理，划到就显示，或者划到就不显示
    clipGeneratedNewDatabaseCore_textAnalogue_SutherlandHodgman(newDatabase.attributes); //TODO: 需要特殊处理，划到就显示，或者划到就不显示

    // 统计时间
    // double time_cost = t_ogm.elapsed(); //秒
    double time_cost = (double)(clock() - start) / CLOCKS_PER_SEC; //秒
    cout << "[INFO] clipDatabase done, " << time_cost << "s has elapsed" << endl; //TODO: 装饰器怎么写出来？
}
