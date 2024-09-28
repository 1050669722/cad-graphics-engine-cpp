//
// Created by liuxingbo03 on 2021/12/24.
//

#ifndef ATUOCADDATAANALYSISPYCADCPP_SUTHERLANDHODGMAN_H
#define ATUOCADDATAANALYSISPYCADCPP_SUTHERLANDHODGMAN_H


#include "pycad/Structs.h"
#include "pycad/Configuration.h"
#include "pycad/clip/ClipStructs.h"
#include "pycad/Utils.h"


class SutherlandHodgman {
private:
    // 成员属性
    Configuration* config{};

    // 成员方法

public:
    // 成员属性
    vector<point> input_vertice;  //输入 //TODO: 考虑改成指针；
    vector<point> output_vertice;
    vector<point> input_vertice_reverse;
    vector<point> output_vertice_reverse;

    // 成员方法
    SutherlandHodgman();
    explicit SutherlandHodgman(Configuration& config);
    static bool inside(point& pt, EDGE ClipBoundary);//判断点是否可见
    static void intersect(point& s, point& p, EDGE ClipBoundary, point& intersect_pt);//直线段SP和边界求交，返回交点
    void sutherlandHodgman(EDGE& ClipBoundary);
    void sutherlandHodgman_reverse(EDGE& ClipBoundary);
    void keyboard(WINDOW& C);
    void keyboard_reverse(WINDOW& C, vector<vector<point>>& verticesList);

    void clipCoords(Coords& coords_x, Coords& coords_y);
    void clipCoords_reverse(Coords& coords_x, Coords& coords_y);

    bool isEqual_InputVertice_Atom(vector<point>& inputVertice, Atom& atom_x, Atom& atom_y);
    void clipCoords_textAnalogue(Coords& coords_x, Coords& coords_y);
    void clipCoords_textAnalogue_reverse(Coords& coords_x, Coords& coords_y);
};


#endif //ATUOCADDATAANALYSISPYCADCPP_SUTHERLANDHODGMAN_H
