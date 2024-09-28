//
// Created by liuxingbo03 on 2021/12/24.
//

#include "pycad/clip/SutherlandHodgman.h"


SutherlandHodgman::SutherlandHodgman() = default;


SutherlandHodgman::SutherlandHodgman(Configuration &config) {
    this->config = &config;
}


bool SutherlandHodgman::inside(point& pt, EDGE ClipBoundary)//判断点是否可见
{
    if (ClipBoundary.ex > ClipBoundary.bx)
    {
        if (pt.y <= ClipBoundary.by)//裁剪边为窗口上边沿
            return true;
    }
    else if (ClipBoundary.ex < ClipBoundary.bx)
    {
        if (pt.y >= ClipBoundary.by)//裁剪边为窗口下边沿
            return true;
    }
    else if (ClipBoundary.ey > ClipBoundary.by)//裁剪边为窗口右边沿
    {
        if (pt.x >= ClipBoundary.bx)
            return true;
    }
    else if (ClipBoundary.ey < ClipBoundary.by)//裁剪边为窗口左边沿
    {
        if (pt.x <= ClipBoundary.bx)
            return true;
    }
    return false;
}


//直线段SP和边界求交，返回交点
void SutherlandHodgman::intersect(point& s, point& p, EDGE ClipBoundary, point& intersect_pt)
{
    if (ClipBoundary.by == ClipBoundary.ey)//水平裁剪边界
    {
        intersect_pt.y = ClipBoundary.by;
        //x=起点的横坐标+dy/sp斜率
        intersect_pt.x = s.x + (ClipBoundary.by - s.y) * (p.x - s.x) / (p.y - s.y);
    }
    else//垂直裁剪边界
    {
        intersect_pt.x = ClipBoundary.bx;
        intersect_pt.y = s.y + (ClipBoundary.bx - s.x) * (p.y - s.y) / (p.x - s.x);
    }
}


// 用窗口的一条边裁剪多边形
void SutherlandHodgman::sutherlandHodgman(EDGE& ClipBoundary)
{
    point s, p, ip;
    output_vertice.clear();
    s = input_vertice[input_vertice.size() - 1]; //首尾

    for (int j = 0; j < input_vertice.size(); j++)
    {
        p = input_vertice[j];
        if (inside(p, ClipBoundary)) //p在内
        {
            if (inside(s, ClipBoundary)) //sp都在窗口内
            {
                output_vertice.push_back(p);
            }
            else //p在里面 s不在
            {
                intersect(s, p, ClipBoundary, ip);
                output_vertice.push_back(ip);
                output_vertice.push_back(p);
            }
        }
        else //p在外面
        {
            if (inside(s, ClipBoundary)) { //s在窗口内p在窗口外
                intersect(s, p, ClipBoundary, ip);
                output_vertice.push_back(ip);
            }
            //sp都在外面则无输出
        }
        s = p;
    }

    input_vertice = output_vertice; //这次的输出作为下一次的输入，input_vertice和output_vertice是全局变量
}


void SutherlandHodgman::sutherlandHodgman_reverse(EDGE &ClipBoundary) {
    point s, p, ip;
    output_vertice_reverse.clear();
    s = input_vertice_reverse[input_vertice_reverse.size() - 1];//首尾
    for (int j = 0; j < input_vertice_reverse.size(); j++)
    {
        p = input_vertice_reverse[j];
        if (!inside(p, ClipBoundary))//p在外
        {
            if (!inside(s, ClipBoundary))//sp都在窗口外
            {
                output_vertice_reverse.push_back(p);
            }
            else//p在外 s不在
            {
                intersect(s, p, ClipBoundary, ip);
                output_vertice_reverse.push_back(ip);
                output_vertice_reverse.push_back(p);
            }
        }
        else//p在内
        {
            if (!inside(s, ClipBoundary))//s在窗口外p在窗口内
            {
                intersect(s, p, ClipBoundary, ip);
                output_vertice_reverse.push_back(ip);
            }
            //sp都在内则无输出
        }
        s = p;
    }
    // input_vertice_reverse = output_vertice_reverse;//这次的输出作为下一次的输入，input_vertice和output_vertice是全局变量
}


void SutherlandHodgman::keyboard(WINDOW& C) {
    /*
     * 筛选四条边 - inside
     */
    // 筛选框
    EDGE left(C.xmin, C.ymin, C.xmin, C.ymax);
    EDGE bottom(C.xmin, C.ymax, C.xmax, C.ymax);
    EDGE right(C.xmax, C.ymax, C.xmax, C.ymin);
    EDGE top(C.xmax, C.ymin, C.xmin, C.ymin);

    // 一次用窗口的一条边裁剪多边形
    sutherlandHodgman(left);
    sutherlandHodgman(bottom);
    sutherlandHodgman(right);
    sutherlandHodgman(top);
}


void SutherlandHodgman::keyboard_reverse(WINDOW &C, vector<vector<point>> &verticesList) {
    /*
     * 筛选四条边 - outside
     */
    // 筛选框
    EDGE left(C.xmin, C.ymin, C.xmin, C.ymax);
    EDGE bottom(C.xmin, C.ymax, C.xmax, C.ymax);
    EDGE right(C.xmax, C.ymax, C.xmax, C.ymin);
    EDGE top(C.xmax, C.ymin, C.xmin, C.ymin);

    verticesList.clear();
    // 一次用窗口的一条边裁剪多边形
    sutherlandHodgman_reverse(left);
    verticesList.push_back(output_vertice_reverse);
    sutherlandHodgman_reverse(bottom);
    verticesList.push_back(output_vertice_reverse);
    sutherlandHodgman_reverse(right);
    verticesList.push_back(output_vertice_reverse);
    sutherlandHodgman_reverse(top);
    verticesList.push_back(output_vertice_reverse);
}


void SutherlandHodgman::clipCoords(Coords& coords_x, Coords& coords_y) {
    for (int j = 0; j < coords_x.size(); j++) {
        Coord& coord_x = coords_x[j];
        Coord& coord_y = coords_y[j];

        //创建新的Coord，push_back筛选后的坐标，替换原来的Coord
        Coord coord_x_1;
        Coord coord_y_1;
        coord_x_1.clear();
        coord_y_1.clear();

        for (int m = 0; m < coord_x.size(); m++) {
            Atom& atom_x = coord_x[m];
            Atom& atom_y = coord_y[m];

            input_vertice.clear();
            for (int n = 0; n < atom_x.size(); n++) {
                point p(atom_x[n], atom_y[n]);
                input_vertice.push_back(p);
            }

            WINDOW C{};
            C.xmin = config->region[0];
            C.ymin = config->region[1];
            C.xmax = config->region[2];
            C.ymax = config->region[3];

            keyboard(C);

            // // for-test
            // if (input_vertice.size() == 2) {
            //     for (int q = 0; q < atom_x.size(); ++q) {
            //         cout << "line 205: " << "x: " << atom_x[q] << endl;
            //         cout << "line 205: " << "y: " << atom_y[q] << endl;
            //     }
            //     cout << "line 204: " << input_vertice.size() << endl;
            // }

            // if(input_vertice.empty()){
            if(input_vertice.size() <= 1){
                atom_x.clear();
                atom_y.clear();
                continue;
            }
            Atom atom_x_1;
            Atom atom_y_1;
            atom_x_1.clear();
            atom_y_1.clear();

            //算法筛选后对input_vertice的后处理
            //删除矩形框上的线段
            for (int k = 0; k < input_vertice.size()-1; k++) {
                if(k==0){
                    atom_x_1.push_back(input_vertice[k].x);
                    atom_y_1.push_back(input_vertice[k].y);
                    if(input_vertice[0].x==input_vertice[1].x&&input_vertice[0].y==input_vertice[1].y){
                        continue;
                    }
                }
                if((abs(input_vertice[k].x - C.xmin) < 0.001 && abs(input_vertice[k + 1].x - C.xmin) < 0.001) ||
                   (abs(input_vertice[k].x - C.xmax) < 0.001 && abs(input_vertice[k + 1].x - C.xmax) < 0.001) ||
                   (abs(input_vertice[k].y - C.ymin) < 0.001 && abs(input_vertice[k + 1].y - C.ymin) < 0.001) ||
                   (abs(input_vertice[k].y - C.ymax) < 0.001 && abs(input_vertice[k + 1].y - C.ymax) < 0.001)) {
                    if(atom_x_1.size() <= 1){
                        continue;
                    }

                    // // for-test
                    // if (atom_x_1.size() == 2) {
                    //     cout << "line 241: " << "input_vertice.size(): " << input_vertice.size() << endl;
                    // }

                    // // for-test
                    // if (atom_x_1.size() == 2) {
                    //     for (int q = 0; q < atom_x.size(); ++q) {
                    //         cout << "line 243: " << "x: " << atom_x[q] << endl;
                    //         cout << "line 244: " << "y: " << atom_y[q] << endl;
                    //     }
                    //     cout << endl;
                    // }

                    // if (atom_x_1.size() == 2) {
                    //     for (int q = 0; q < atom_x_1.size(); ++q) {
                    //         cout << "line 251: " << "x: " << atom_x_1[q] << endl;
                    //         cout << "line 252: " << "y: " << atom_y_1[q] << endl;
                    //     }
                    //     cout << endl;
                    // }

                    // if (atom_x_1.size() == 2) {
                    //     cout << "------------------------------------------------------" << endl;
                    // }

                    coord_x_1.push_back(atom_x_1);
                    coord_y_1.push_back(atom_y_1);
                    atom_x_1.clear();
                    atom_y_1.clear();
                }
                atom_x_1.push_back(input_vertice[k + 1].x);
                atom_y_1.push_back(input_vertice[k + 1].y);
            }
            if(atom_x_1.size()<=1){
                continue;
            }

            // // for-test
            // if (atom_x_1.size() == 2) {
            //     for (int q = 0; q < atom_x.size(); ++q) {
            //         cout << "line 263: " << "x: " << atom_x[q] << endl;
            //         cout << "line 264: " << "y: " << atom_y[q] << endl;
            //     }
            // }

            coord_x_1.push_back(atom_x_1);
            coord_y_1.push_back(atom_y_1);
        }
        coord_x.clear();
        coord_y.clear();
        coord_x = coord_x_1;
        coord_y = coord_y_1;
    }
}


bool SutherlandHodgman::isEqual_InputVertice_Atom(vector<point>& inputVertice, Atom &atom_x, Atom &atom_y) {
    // area_atom
    double area_atom = Utils::calculateAreaOfPolygon(atom_x, atom_y);

    // area_input_vertice
    Atom atom_x_inputVertice{}, atom_y_inputVertice{};
    for (auto& pt : inputVertice) {
        atom_x_inputVertice.push_back(pt.x);
        atom_y_inputVertice.push_back(pt.y);
    }
    double area_inputVertice = Utils::calculateAreaOfPolygon(atom_x_inputVertice, atom_y_inputVertice);

    // isEqual
    return Utils::isEqual(area_atom, area_inputVertice, 1e-3);
}


void SutherlandHodgman::clipCoords_reverse(Coords &coords_x, Coords &coords_y) {
    for (int j = 0; j < coords_x.size(); j++) {
        Coord& coord_x = coords_x[j];
        Coord& coord_y = coords_y[j];

        //创建新的Coord，push_back筛选后的坐标，替换原来的Coord
        Coord coord_x_1;
        Coord coord_y_1;
        coord_x_1.clear();
        coord_y_1.clear();

        for (int m = 0; m < coord_x.size(); m++) {
            Atom& atom_x = coord_x[m];
            Atom& atom_y = coord_y[m];

            input_vertice_reverse.clear();
            for (int n = 0; n < atom_x.size(); n++) {
                point p(atom_x[n], atom_y[n]);
                input_vertice_reverse.push_back(p);
            }

            WINDOW C{};
            C.xmin = config->region[0];
            C.ymin = config->region[1];
            C.xmax = config->region[2];
            C.ymax = config->region[3];

            vector<vector<point>> verticesList;
            keyboard_reverse(C, verticesList);

            if(verticesList.empty()){
                atom_x.clear();
                atom_y.clear();
                continue;
            }

            //算法筛选后对input_vertice的后处理
            //删除矩形框上的线段 //TODO: 存在重叠边
            for (int l = 0; l < verticesList.size(); l++) {
                if (verticesList[l].size() <= 1) {
                    continue;
                }

                Atom atom_x_1;
                Atom atom_y_1;
                atom_x_1.clear();
                atom_y_1.clear();

                for (int k = 0; k < verticesList[l].size() - 1; k++) {
                    if (k == 0) {
                        atom_x_1.push_back(verticesList[l][k].x);
                        atom_y_1.push_back(verticesList[l][k].y);
                        if (verticesList[l][0].x == verticesList[l][1].x && verticesList[l][0].y == verticesList[l][1].y) {
                            continue;
                        }
                    }

                    if ((abs(verticesList[l][k].x - C.xmin) < 0.001 && abs(verticesList[l][k + 1].x - C.xmin) < 0.001) ||
                        (abs(verticesList[l][k].x - C.xmax) < 0.001 && abs(verticesList[l][k + 1].x - C.xmax) < 0.001) ||
                        (abs(verticesList[l][k].y - C.ymin) < 0.001 && abs(verticesList[l][k + 1].y - C.ymin) < 0.001) ||
                        (abs(verticesList[l][k].y - C.ymax) < 0.001 && abs(verticesList[l][k + 1].y - C.ymax) < 0.001)) {
                        if(atom_x_1.size() <= 1){
                            continue;
                        }

                        coord_x_1.push_back(atom_x_1);
                        coord_y_1.push_back(atom_y_1);
                        atom_x_1.clear();
                        atom_y_1.clear();
                    }

                    atom_x_1.push_back(verticesList[l][k + 1].x);
                    atom_y_1.push_back(verticesList[l][k + 1].y);

                }

                if(atom_x_1.size() <= 1){
                    continue;
                }

                coord_x_1.push_back(atom_x_1);
                coord_y_1.push_back(atom_y_1);

            }

        }

        coord_x.clear();
        coord_y.clear();
        coord_x = coord_x_1;
        coord_y = coord_y_1;

    }
}


void SutherlandHodgman::clipCoords_textAnalogue(Coords &coords_x, Coords &coords_y) {
    for (int j = 0; j < coords_x.size(); j++) {
        Coord& coord_x = coords_x[j];
        Coord& coord_y = coords_y[j];

        //创建新的Coord，push_back筛选后的坐标，替换原来的Coord
        Coord coord_x_1;
        Coord coord_y_1;
        coord_x_1.clear();
        coord_y_1.clear();

        for (int m = 0; m < coord_x.size(); m++) {
            Atom& atom_x = coord_x[m];
            Atom& atom_y = coord_y[m];

            input_vertice.clear();
            for (int n = 0; n < atom_x.size(); n++) {
                point p(atom_x[n], atom_y[n]);
                input_vertice.push_back(p);
            }

            WINDOW C{};
            C.xmin = config->region[0];
            C.ymin = config->region[1];
            C.xmax = config->region[2];
            C.ymax = config->region[3];

            keyboard(C);

            // if(input_vertice.empty()){
            if(input_vertice.size() <= 1){
                atom_x.clear();
                atom_y.clear();
                continue;
            }
            Atom atom_x_1;
            Atom atom_y_1;
            atom_x_1.clear();
            atom_y_1.clear();

            // 切到了文本框
            if (!isEqual_InputVertice_Atom(input_vertice, atom_x, atom_y)) {
                continue;
            }

            // 算法筛选后对input_vertice的后处理
            // 删除矩形框上的线段
            // for (int k = 0; k < input_vertice.size()-1; k++) {
                // if(k==0){
                //     atom_x_1.push_back(input_vertice[k].x);
                //     atom_y_1.push_back(input_vertice[k].y);
                //     if(input_vertice[0].x==input_vertice[1].x&&input_vertice[0].y==input_vertice[1].y){
                //         continue;
                //     }
                // }
                // if((abs(input_vertice[k].x - C.xmin) < 0.001 && abs(input_vertice[k + 1].x - C.xmin) < 0.001) ||
                //    (abs(input_vertice[k].x - C.xmax) < 0.001 && abs(input_vertice[k + 1].x - C.xmax) < 0.001) ||
                //    (abs(input_vertice[k].y - C.ymin) < 0.001 && abs(input_vertice[k + 1].y - C.ymin) < 0.001) ||
                //    (abs(input_vertice[k].y - C.ymax) < 0.001 && abs(input_vertice[k + 1].y - C.ymax) < 0.001)) {
                //
                //     // for-test
                //     cout << "----------------- line 471 -----------------" << endl;
                //
                //     if(atom_x_1.size() <= 1){
                //         continue;
                //     }
                //
                //     coord_x_1.push_back(atom_x_1);
                //     coord_y_1.push_back(atom_y_1);
                //     atom_x_1.clear();
                //     atom_y_1.clear();
                // }
                // atom_x_1.push_back(input_vertice[k + 1].x);
                // atom_y_1.push_back(input_vertice[k + 1].y);
            // }

            // 新的后处理 --- 不删除筛选框上的边
            for (int k = 0; k < input_vertice.size(); k++) {
                atom_x_1.push_back(input_vertice[k].x);
                atom_y_1.push_back(input_vertice[k].y);
            }

            if(atom_x_1.size()<=1){
                continue;
            }

            coord_x_1.push_back(atom_x_1);
            coord_y_1.push_back(atom_y_1);
        }
        coord_x.clear();
        coord_y.clear();
        coord_x = coord_x_1;
        coord_y = coord_y_1;
    }
}

void SutherlandHodgman::clipCoords_textAnalogue_reverse(Coords &coords_x, Coords &coords_y) {
    for (int j = 0; j < coords_x.size(); j++) {
        Coord& coord_x = coords_x[j];
        Coord& coord_y = coords_y[j];

        //创建新的Coord，push_back筛选后的坐标，替换原来的Coord
        Coord coord_x_1;
        Coord coord_y_1;
        coord_x_1.clear();
        coord_y_1.clear();

        for (int m = 0; m < coord_x.size(); m++) {
            Atom& atom_x = coord_x[m];
            Atom& atom_y = coord_y[m];

            input_vertice_reverse.clear();
            for (int n = 0; n < atom_x.size(); n++) {
                point p(atom_x[n], atom_y[n]);
                input_vertice_reverse.push_back(p);
            }

            WINDOW C{};
            C.xmin = config->region[0];
            C.ymin = config->region[1];
            C.xmax = config->region[2];
            C.ymax = config->region[3];

            vector<vector<point>> verticesList;
            keyboard_reverse(C, verticesList);

            if(verticesList.empty()){
                atom_x.clear();
                atom_y.clear();
                continue;
            }

            //算法筛选后对input_vertice的后处理
            //删除矩形框上的线段
            for (int l = 0; l < verticesList.size(); l++) {
                if (verticesList[l].size() <= 1) {
                    continue;
                }

                // 切到文本框
                if (!isEqual_InputVertice_Atom(verticesList[l], atom_x, atom_y)) {
                    continue;
                }

                Atom atom_x_1;
                Atom atom_y_1;
                atom_x_1.clear();
                atom_y_1.clear();

                // // 算法筛选后对verticesList的后处理
                // // 删除矩形框上的线段
                // for (int k = 0; k < verticesList[l].size() - 1; k++) {
                //     if (k == 0) {
                //         atom_x_1.push_back(verticesList[l][k].x);
                //         atom_y_1.push_back(verticesList[l][k].y);
                //         if (verticesList[l][0].x == verticesList[l][1].x && verticesList[l][0].y == verticesList[l][1].y) {
                //             continue;
                //         }
                //     }
                //
                //     if ((abs(verticesList[l][k].x - C.xmin) < 0.001 && abs(verticesList[l][k + 1].x - C.xmin) < 0.001) ||
                //         (abs(verticesList[l][k].x - C.xmax) < 0.001 && abs(verticesList[l][k + 1].x - C.xmax) < 0.001) ||
                //         (abs(verticesList[l][k].y - C.ymin) < 0.001 && abs(verticesList[l][k + 1].y - C.ymin) < 0.001) ||
                //         (abs(verticesList[l][k].y - C.ymax) < 0.001 && abs(verticesList[l][k + 1].y - C.ymax) < 0.001)) {
                //
                //         // for-test
                //         cout << "----------------- line 567 -----------------" << endl;
                //
                //         if(atom_x_1.size() <= 1){
                //             continue;
                //         }
                //
                //         coord_x_1.push_back(atom_x_1);
                //         coord_y_1.push_back(atom_y_1);
                //         atom_x_1.clear();
                //         atom_y_1.clear();
                //     }
                //
                //     atom_x_1.push_back(verticesList[l][k + 1].x);
                //     atom_y_1.push_back(verticesList[l][k + 1].y);
                // }

                // 新的后处理 --- 不删除筛选框上的边
                for (int k = 0; k < verticesList[l].size(); k++) {
                    atom_x_1.push_back(verticesList[l][k].x);
                    atom_y_1.push_back(verticesList[l][k].y);
                }

                if(atom_x_1.size() <= 1){
                    continue;
                }

                coord_x_1.push_back(atom_x_1);
                coord_y_1.push_back(atom_y_1);

            }

        }

        coord_x.clear();
        coord_y.clear();
        coord_x = coord_x_1;
        coord_y = coord_y_1;

    }
}
