//
// Created by liuxingbo03 on 2021/10/25.
//

#include <iostream>
// #include <fstream>

#include "pycad/Database.h"
// #include "pycad/Utils.h"


// 搬运至DatabaseConverter.cpp
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
Database::Database() = default;
//
//
// void Database::dumps(string& jsonstr) {
//     json j;
//     j = json{
//             {"lines", this->lines},
//             {"polylines", this->polylines},
//             {"circles", this->circles},
//             {"arcs", this->arcs},
//             {"ellipses", this->ellipses},
//             {"points", this->points},
//             {"splines", this->splines},
//             {"solids", this->solids},
//             {"texts", this->texts},
//             {"mtexts", this->mtexts},
//             {"attributes", this->attributes},
//             {"alignedDimensions", this->alignedDimensions},
//             {"rotatedDimensions", this->rotatedDimensions},
//             {"lineAngularDimensions", this->lineAngularDimensions},
//             {"arcDimensions", this->arcDimensions},
//             {"radialDimensions", this->radialDimensions},
//             {"diametricDimensions", this->diametricDimensions},
//             {"ordinateDimensions", this->ordinateDimensions},
//             {"radialDimensionLarges", this->radialDimensionLarges}
//     };
//     jsonstr = j.dump();
//     // string s = j.dump(); //TODO: 考虑删除这三行
//     // char* mOutputDatabase = (char*)malloc(sizeof(char) * (s.length() + 1));
//     // strcpy(mOutputDatabase, s.c_str());
// }
//
//
// void Database::dump(string& filepath) {
//     // 序列化 到 jsonstr
//     string jsonstr;
//     this->dumps(jsonstr);
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
// // void Database::dumpCache(string& cache, string& name) {
// //     Utils::makeDir(cache);
// //     string cachePath = Utils::joinPath(cache, name);
// //     this->dump(cachePath);
// // }
