//
// Created by nan on 2021/11/17.
//

#include <sstream>
#include "pycad/clip/SutherlandHodgman.h"
#include "pycad/clip/WeilerAtherton.h"


WeilerAtherton::WeilerAtherton() = default;


WeilerAtherton::WeilerAtherton(Configuration& config) {
    this->config = &config;
    this->sutherlandHodgmanClip = SutherlandHodgman(config);
}


// 判断是否在多边形内
bool WeilerAtherton::isPointInsidePg(RPoint p, WeilerAtherton& py){
    int cnt = 0, size = py.pts.size();
    for (int i = 0; i < size; i++) {
        RPoint p1 = py.pts[i];
        RPoint p2 = py.pts[(i + 1) % size];
        if (p1.y == p2.y) continue;
        if (p.y < min(p1.y, p2.y)) continue;
        if (p.y >= max(p1.y, p2.y)) continue;
        double x = (double)(p.y - p1.y) * (double)(p2.x - p1.x) / (double)(p2.y - p1.y) + p1.x;
        if (x > p.x) cnt++;
    }
    return (cnt % 2 == 1);
}


int WeilerAtherton::cross(RPoint& p0, RPoint& p1, RPoint& p2){
    return ((p2.x - p0.x) * (p1.y - p0.y) - (p1.x - p0.x) * (p2.y - p0.y));
}


bool WeilerAtherton::onSegment(RPoint& p0, RPoint& p1, RPoint& p2){
    int minx = min(p0.x, p1.x), maxx = max(p0.x, p1.x);
    int miny = min(p0.y, p1.y), maxy = max(p0.y, p1.y);
    if (p2.x >= minx && p2.x <= maxx && p2.y >= miny && p2.y <= maxy) return true;
    return false;
}

//判断线段是否相交
bool WeilerAtherton::segmentsIntersect(RPoint& p1, RPoint& p2, RPoint& p3, RPoint& p4){
    int d1 = cross(p3, p4, p1);
    int d2 = cross(p3, p4, p2);
    int d3 = cross(p1, p2, p3);
    int d4 = cross(p1, p2, p4);
    if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) &&
        ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0)))
        return true;
    if (d1 == 0 && onSegment(p3, p4, p1)) return true;
    if (d2 == 0 && onSegment(p3, p4, p2)) return true;
    if (d3 == 0 && onSegment(p1, p2, p3)) return true;
    if (d4 == 0 && onSegment(p1, p2, p4)) return true;
    return false;
}

//获取交点坐标
RPoint WeilerAtherton::getintersectPoint(RPoint p1, RPoint p2, RPoint p3, RPoint p4){
    RPoint p;
    double d= (p2.y - p1.y) * (p4.x - p3.x) - (p1.x - p2.x) * (p3.y - p4.y);
    p.x = ((p2.x - p1.x) * (p4.x - p3.x) * (p3.y - p1.y) - p3.x * (p2.x - p1.x) * (p4.y - p3.y) + p1.x * (p2.y - p1.y) * (p4.x - p3.x)) /d;
    p.y = -((p2.y - p1.y) * (p4.y - p3.y) * (p3.x - p1.x) - p3.y * (p2.y - p1.y) * (p4.x - p3.x) + p1.y * (p2.x - p1.x) * (p4.y - p3.y)) /d;
    return p;
}

//判断点是否在矩形框内
bool WeilerAtherton::isPointInsidePoly(RPoint P, const vector<RPoint>& polyVertices) {
    std::size_t vertCount = polyVertices.size();
    if (vertCount < 2)
        return false;
    RPoint tmp = P;
    for (int l = 0; l < 2; l++) {
        for (int r = 0; r < 2; r++) {
            P = tmp;
            if (l % 2) P.x += 0.001;
            else P.x -= 0.001;
            if (r % 2) P.y += 0.001;
            else P.y -= 0.001;
            bool inside = false;
            for (unsigned i = 1; i <= vertCount; ++i) {
                const RPoint &A = polyVertices[i - 1];
                const RPoint &B = polyVertices[i % vertCount];
                if ((B.y <= P.y && P.y < A.y) || (A.y <= P.y && P.y < B.y)) {
                    double t = (P.x - B.x) * (A.y - B.y) - (A.x - B.x) * (P.y - B.y);
                    if (A.y < B.y)
                        t = -t;
                    if (t < 0)
                        inside = !inside;
                }
            }
            if (inside) return inside;
        }
    }
    return false;
}


// 获得两组多边形的交点
void WeilerAtherton::generateIntersectPoints(WeilerAtherton& pyclip, WeilerAtherton& py, list<IP>& iplist){
    int clipSize = pyclip.pts.size(), pySize = py.pts.size();
    for (int i = 0; i < clipSize; i++) {
        RPoint p1 = pyclip.pts[i];
        RPoint p2 = pyclip.pts[(i + 1) % clipSize];
        for (int j = 0; j < pySize-1; j++) {
            RPoint p3 = py.pts[j];
            RPoint p4 = py.pts[(j + 1) % pySize];
            if (segmentsIntersect(p1, p2, p3, p4)) {
                IP ip;
                ip.index0 = j;
                ip.index1 = i;
                ip.p = getintersectPoint(p1, p2, p3, p4);
                iplist.push_back(ip);
            }
        }
    }
}

//两个点之间的距离
int WeilerAtherton::getDistance(RPoint& p1, RPoint& p2) {
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}


bool WeilerAtherton::distanceComparator(IP& ip1, IP& ip2) {
    return ip1.dis < ip2.dis;
}


//将两个多边形交点按照顺序添加到顶点产生新的list
void WeilerAtherton::generateList(WeilerAtherton& py, list<IP>& iplist, list<IP>& comlist, int index) {
    int size = py.pts.size(); // 原来的点数
    list<IP>::iterator it;

    for (int i = 0; i < size; i++) {
        RPoint p1 = py.pts[i];
        IP ip;
        ip.pointFlag = 0;
        ip.p = p1;
        comlist.push_back(ip);
        list<IP> oneSeg;
        //遍历iplist 交点
        for (it = iplist.begin(); it != iplist.end(); it++) {
            if ((index == 0 && i == it->index0) ||
                (index == 1 && i == it->index1)) {
                it->dis = getDistance(it->p, p1);
                it->pointFlag = 1;
                oneSeg.push_back(*it);
            }
        }
        oneSeg.sort(distanceComparator);// 按序排列，可能存在多个交点
        for (it = oneSeg.begin(); it != oneSeg.end(); it++)
            comlist.push_back(*it);
    }
}


// 为多边形Pglist交点赋上出入点属性
void WeilerAtherton::getPgPointInOut(list<IP>& Pglist, WeilerAtherton& pyclip) {
    bool inFlag;
    list<IP>::iterator it;
    for (it = Pglist.begin(); it != Pglist.end(); it++) {
        if (it->pointFlag == 0) { //
            if (isPointInsidePg(it->p, pyclip))
                inFlag = true;
            else inFlag = false;
        }
        else {
            inFlag = !inFlag;
            it->inFlag = inFlag;//
        }
    }
}
bool operator==(RPoint& p1, RPoint& p2) {
    return p1.x == p2.x && p1.y == p2.y;
}


// 为矩形框cliplist交点赋上出入点属性
void WeilerAtherton::getClipPointInOut(list<IP>& cliplist, list<IP>& Pglist) {
    list<IP>::iterator it, it1;
    list<IP> Pglist1 = Pglist;
    for (it = cliplist.begin(); it != cliplist.end(); it++) {
        if (it->pointFlag == 0) continue;
        for (it1 = Pglist1.begin(); it1 != Pglist1.end(); it1++) {
            if (it1->pointFlag == 0) continue; // 原顶点
            if (it->p == it1->p) {
                it->inFlag = it1->inFlag;
            }
        }
    }
}


// 获取相交区域，输出筛选后的线段
void WeilerAtherton::generateClipArea(list<IP>& Pglist, list<IP>& cliplist , vector<vector<RPoint>>& output) {
//    double intersection_area = 0.0;
    list<IP>::iterator it, it1,end;
    WeilerAtherton py;
    int num = Pglist.size();

    for (it = Pglist.begin(); it != Pglist.end(); it++) {
        if (it->pointFlag == 1 && it->inFlag) break; // 找到第一个inFlag=true的交点
    }
    py.pts.clear();
    auto it_begin = it;
    while (true) {
        //py.pts.push_back(it->p);
        for (; ; it++) {
            if(it==Pglist.end()) it = Pglist.begin();
            if (it->pointFlag == 1 && !it->inFlag) break;
            py.pts.push_back(it->p);
        }
        for (it1 = cliplist.begin(); it1 != cliplist.end(); it1++){
            if(it1==cliplist.end()) it1 = cliplist.begin();
            if (it1->p == it->p) break; // it1追到it
        }
        for (; ; it1++) {
            if(it1==cliplist.end()) it1 = cliplist.begin();
            if (it1->pointFlag == 1 && it1->inFlag) break;
            py.pts.push_back(it1->p);
        }

        if (py.pts[0]==it1->p) {
            //intersection_area += calculate_ploygon_area(py.pts);
            output.push_back(py.pts);
            py.pts.clear();
            for (; it != Pglist.end(); it++)
                if (it->pointFlag == 1 && it->inFlag) break; //寻找下一个环

            if (it == it_begin||it==Pglist.end()) break;// 退出条件
            else continue;
        }else{
            for (; ; it++)
            {
                if(it==Pglist.end()) it = Pglist.begin();
                if (it->p == it1->p) break; // it 追到it1
            }
        }
    }
//    std::cout<<"the intersection area is: " <<intersection_area<<std::endl;
}


void WeilerAtherton::generateClipArea_reverse(list<IP> &Pglist, list<IP> &cliplist, vector<vector<RPoint>> &output) {
//    double intersection_area = 0.0;
    list<IP>::iterator it, it1,end;
    WeilerAtherton py;

    reverse(cliplist.begin(),cliplist.end());

    int num = Pglist.size();

    for (it = Pglist.begin(); it != Pglist.end(); it++) {
        if (it->pointFlag == 1 && it->inFlag) break; // 找到第一个inFlag=true的交点
    }
    py.pts.clear();
    auto it_begin = it;
    while (true) {
        //py.pts.push_back(it->p);
        for (; ; it++) {
            if(it==Pglist.end()) it = Pglist.begin();
            if (it->pointFlag == 1 && !it->inFlag) break;
            py.pts.push_back(it->p);
        }
        for (it1 = cliplist.begin(); it1 != cliplist.end(); it1++){
            if(it1==cliplist.end()) it1 = cliplist.begin();
            if (it1->p == it->p) break; // it1追到it
        }
        for (; ; it1++) {
            if(it1==cliplist.end()) it1 = cliplist.begin();
            if (it1->pointFlag == 1 && it1->inFlag) break;
            py.pts.push_back(it1->p);
        }

        if (py.pts[0]==it1->p) {
            //intersection_area += calculate_ploygon_area(py.pts);
            output.push_back(py.pts);
            py.pts.clear();
            for (; it != Pglist.end(); it++)
                if (it->pointFlag == 1 && it->inFlag) break; //寻找下一个环

            if (it == it_begin||it==Pglist.end()) break;// 退出条件
            else continue;
        }else{
            for (; ; it++)
            {
                if(it==Pglist.end()) it = Pglist.begin();
                if (it->p == it1->p) break; // it 追到it1
            }
        }
    }
//    std::cout<<"the intersection area is: " <<intersection_area<<std::endl;
}


vector<vector<RPoint>> WeilerAtherton::weilerAtherton(WeilerAtherton& pyclip, WeilerAtherton& py) {
    vector<vector<RPoint>> output;
    list<IP> iplist, Pglist, cliplist;
    generateIntersectPoints(pyclip, py, iplist);// 获取两个多边形的相交点
    if(iplist.empty()){
        //如果两个多边形不相交，判断一个多边形是否在另一个内部
        if(isPointInsidePoly(py.pts[0],pyclip.pts)){
            output.push_back(py.pts);
            return output;
        } else{
            return {};
        }
    }
    generateList(py, iplist, Pglist, 0);
    generateList(pyclip, iplist, cliplist, 1);
    getPgPointInOut(Pglist, pyclip); //出入点属性付给Pglist
    getClipPointInOut(cliplist, Pglist); // 将出入点属性赋给cliplist
    generateClipArea(Pglist, cliplist,output);
    return output;
}


vector<vector<RPoint>> WeilerAtherton::weilerAtherton_reverse(WeilerAtherton &pyclip, WeilerAtherton &py) {
    vector<vector<RPoint>> output;
    list<IP> iplist, Pglist, cliplist;
    generateIntersectPoints(pyclip, py, iplist);// 获取两个多边形的相交点
    if(iplist.empty()){
        //如果两个多边形不相交，判断一个多边形是否在另一个内部
        if(!isPointInsidePoly(py.pts[0],pyclip.pts)){
            output.push_back(py.pts);
            return output;
        } else{
            return {};
        }
    }
    generateList(py, iplist, Pglist, 0);
    generateList(pyclip, iplist, cliplist, 1);
    getPgPointInOut(Pglist, pyclip); //出入点属性付给Pglist
    getClipPointInOut(cliplist, Pglist); // 将出入点属性赋给cliplist
    generateClipArea_reverse(Pglist, cliplist,output);
    return output;
}


//计算多边形的面积
double WeilerAtherton::calculateAreaOfPolygon(Atom& atom_x, Atom& atom_y) {
    /*
     * 计算凸多边形的面积
     * TODO: 计算任意多边形面积，不论凸凹
     */
    double area = 0.0;
    if (atom_x.size() < 3) {
        return area;
    }

    // // 只允许凸多边形，欠缺兼容性
    // Atom p0 = Atom{atom_x[0], atom_y[0]};
    // for (int idx = 1; idx < atom_x.size() - 2; ++idx) {
    //     Atom p1 = Atom{atom_x[idx], atom_y[idx]};
    //     Atom p2 = Atom{atom_x[idx + 1], atom_y[idx + 1]};
    //     area += calculateAreaOfTriangle(p0, p1, p2);
    // }
    // return area;

    // Green公式，不论凹凸，正确的，但是要求输入的坐标按照顺时针或者逆时针排列，TODO: 暂时还没有在此方法外部引入判断凹凸性的函数
    int idx = 0;
    for (; idx < atom_x.size() - 1; ++idx) {
        area += (atom_x[idx] + atom_x[idx + 1]) * (atom_y[idx + 1] - atom_y[idx]);
    }
    area += (atom_x[idx] + atom_x[0]) * (atom_y[0] - atom_y[idx]);
    return 0.5 * area;
}


void WeilerAtherton::clipCoords(Coords& coords_x, Coords& coords_y) {
    // this->coords_x = coords_x;
    // this->coords_y = coords_y;
    for (int i = 0; i < coords_x.size(); i++) {
        Coord &coord_x = coords_x[i];
        Coord &coord_y = coords_y[i];

        Coord coord_x_1;
        Coord coord_y_1;
        coord_x_1.clear();
        coord_y_1.clear();

        WINDOW C{};
        C.xmin = config->region[0];
        C.ymin = config->region[1];
        C.xmax = config->region[2];
        C.ymax = config->region[3];

        for (int j = 0; j < coord_x.size(); j++) {
            Atom &atom_x = coord_x[j];
            Atom &atom_y = coord_y[j];
            double area = calculateAreaOfPolygon(atom_x,atom_y);
            if(abs(area - 0) <= 0.001){
                //判断获取的多边形面积是否为零，如果面积为零，则使用Sutherland_Hodgman算法进行筛选
                // vector<point> input_vertice;
                vector<point>& input_vertice = sutherlandHodgmanClip.input_vertice;
                input_vertice.clear();

                for (int n = 0; n < atom_x.size(); n++) {
                    point p(atom_x[n], atom_y[n]);
                    input_vertice.push_back(p);
                }

                sutherlandHodgmanClip.keyboard(C);
                if(input_vertice.empty()){
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
                    if((abs(input_vertice[k].x-C.xmin)<=0.001 && abs(input_vertice[k+1].x-C.xmin)<=0.001)||
                       (abs(input_vertice[k].x-C.xmax)<=0.001 && abs(input_vertice[k+1].x-C.xmax)<=0.001)||
                       (abs(input_vertice[k].y-C.ymin)<=0.001 && abs(input_vertice[k+1].y-C.ymin)<=0.001)||
                       (abs(input_vertice[k].y-C.ymax)<=0.001 && abs(input_vertice[k+1].y-C.ymax)<=0.001)){

                        if(atom_x_1.size()<=1){
                            continue;
                        }

                        // 反转拼接
                        vector<double> atom_x_Rever;
                        vector<double> atom_y_Rever;
                        atom_x_Rever = atom_x_1;
                        atom_y_Rever = atom_y_1;
                        reverse(atom_x_Rever.begin(), atom_x_Rever.end());
                        reverse(atom_y_Rever.begin(), atom_y_Rever.end());
                        atom_x_1.insert(atom_x_1.end(), atom_x_Rever.begin(), atom_x_Rever.end());
                        atom_y_1.insert(atom_y_1.end(), atom_y_Rever.begin(), atom_y_Rever.end());

                        coord_x_1.push_back(atom_x_1);
                        coord_y_1.push_back(atom_y_1);
                        atom_x_1.clear();
                        atom_y_1.clear();
                    }
                    atom_x_1.push_back(input_vertice[k+1].x);
                    atom_y_1.push_back(input_vertice[k+1].y);
                }

                // 去除只剩下一个点或者没有点的情况
                if (atom_x_1.size() <= 1){
                    continue;
                }

                // 反转拼接
                vector<double> atom_x_Rever;
                vector<double> atom_y_Rever;
                atom_x_Rever = atom_x_1;
                atom_y_Rever = atom_y_1;
                reverse(atom_x_Rever.begin(), atom_x_Rever.end());
                reverse(atom_y_Rever.begin(), atom_y_Rever.end());
                atom_x_1.insert(atom_x_1.end(), atom_x_Rever.begin(), atom_x_Rever.end());
                atom_y_1.insert(atom_y_1.end(), atom_y_Rever.begin(), atom_y_Rever.end());

                coord_x_1.push_back(atom_x_1);
                coord_y_1.push_back(atom_y_1);
                continue;
            }

            if((area - 0) < -0.001) {
                //多边形面积不为零，使用weiler_Atherton算法
                //判断获取的多边形是否为顺时针，如果为逆时针则将vector内部数据倒序

                // vector<double> atom_x_Rever;
                // vector<double> atom_y_Rever;
                // atom_x_Rever.reserve(atom_x_Rever.size());
                // atom_y_Rever.reserve(atom_y_Rever.size());
                // //vector反转
                // //使用vector自带的反转迭代器reverse_iterator,rbegin(),rend()
                // vector<double>::reverse_iterator x_it;
                // for (x_it=atom_x.rbegin();x_it!=atom_x.rend();x_it++)
                // {
                //     atom_x_Rever.push_back(*x_it);
                // }
                // vector<double>::reverse_iterator y_it;
                // for (y_it=atom_y.rbegin();y_it!=atom_y.rend();y_it++)
                // {
                //     atom_y_Rever.push_back(*y_it);
                // }
                // atom_x.clear();
                // atom_y.clear();
                // atom_x = atom_x_Rever;
                // atom_y = atom_y_Rever;

                reverse(atom_x.begin(), atom_x.end());
                reverse(atom_y.begin(), atom_y.end());
            }

            WeilerAtherton pyclip, window;
            pyclip.pts.clear();
            window.pts.clear();
            for (int n = 0; n < atom_x.size(); n++) {
                RPoint p(atom_x[n], atom_y[n]);
                pyclip.pts.push_back(p);
            }

            RPoint w1(C.xmin, C.ymin);
            window.pts.push_back(w1);
            RPoint w2(C.xmax, C.ymin);
            window.pts.push_back(w2);
            RPoint w3(C.xmax, C.ymax);
            window.pts.push_back(w3);
            RPoint w4(C.xmin, C.ymax);
            window.pts.push_back(w4);
            coord_clip = weilerAtherton(window,pyclip);
            if(coord_clip.empty()){
                continue;
            }
            for(int m = 0; m < coord_clip.size(); m++){
                Atom atom_x_1;
                Atom atom_y_1;
                atom_x_1.clear();
                atom_y_1.clear();
                for (int k = 0; k < coord_clip[m].size(); k++) {
                    atom_x_1.push_back(coord_clip[m][k].x);
                    atom_y_1.push_back(coord_clip[m][k].y);
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


void WeilerAtherton::clipCoords_reverse(Coords &coords_x, Coords &coords_y) {
    // this->coords_x = coords_x;
    // this->coords_y = coords_y;
    for (int i = 0; i < coords_x.size(); i++) {
        Coord &coord_x = coords_x[i];
        Coord &coord_y = coords_y[i];

        Coord coord_x_1;
        Coord coord_y_1;
        coord_x_1.clear();
        coord_y_1.clear();

        WINDOW C{};
        C.xmin = config->region[0];
        C.ymin = config->region[1];
        C.xmax = config->region[2];
        C.ymax = config->region[3];

        for (int j = 0; j < coord_x.size(); j++) {
            Atom &atom_x = coord_x[j];
            Atom &atom_y = coord_y[j];
            double area = calculateAreaOfPolygon(atom_x,atom_y);
            if(abs(area - 0) <= 0.001) {
                //判断获取的多边形面积是否为零，如果面积为零，则使用Sutherland_Hodgman算法进行筛选
                // vector<point> input_vertice;
                vector<point>& input_vertice_reverse = sutherlandHodgmanClip.input_vertice_reverse;
                input_vertice_reverse.clear();

                for (int n = 0; n < atom_x.size(); n++) {
                    point p(atom_x[n], atom_y[n]);
                    input_vertice_reverse.push_back(p);
                }

                vector<vector<point>> verticesList;
                sutherlandHodgmanClip.keyboard_reverse(C, verticesList);

                if(input_vertice_reverse.empty()){
                    atom_x.clear();
                    atom_y.clear();
                    continue;
                }

                //算法筛选后对input_vertice的后处理
                //删除矩形框上的线段
                for (int l = 0; l < verticesList.size(); l++){
                    if(verticesList[l].size()==0){
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

                            // 反转拼接
                            vector<double> atom_x_Rever;
                            vector<double> atom_y_Rever;
                            atom_x_Rever = atom_x_1;
                            atom_y_Rever = atom_y_1;
                            reverse(atom_x_Rever.begin(), atom_x_Rever.end());
                            reverse(atom_y_Rever.begin(), atom_y_Rever.end());
                            atom_x_1.insert(atom_x_1.end(), atom_x_Rever.begin(), atom_x_Rever.end());
                            atom_y_1.insert(atom_y_1.end(), atom_y_Rever.begin(), atom_y_Rever.end());

                            coord_x_1.push_back(atom_x_1);
                            coord_y_1.push_back(atom_y_1);
                            atom_x_1.clear();
                            atom_y_1.clear();
                        }

                        atom_x_1.push_back(verticesList[l][k + 1].x);
                        atom_y_1.push_back(verticesList[l][k + 1].y);

                    }

                    // 去除只剩下一个点或者没有点的情况
                    if(atom_x_1.size() <= 1){
                        continue;
                    }

                    // 反转拼接
                    vector<double> atom_x_Rever;
                    vector<double> atom_y_Rever;
                    atom_x_Rever = atom_x_1;
                    atom_y_Rever = atom_y_1;
                    reverse(atom_x_Rever.begin(), atom_x_Rever.end());
                    reverse(atom_y_Rever.begin(), atom_y_Rever.end());
                    atom_x_1.insert(atom_x_1.end(), atom_x_Rever.begin(), atom_x_Rever.end());
                    atom_y_1.insert(atom_y_1.end(), atom_y_Rever.begin(), atom_y_Rever.end());

                    coord_x_1.push_back(atom_x_1);
                    coord_y_1.push_back(atom_y_1);

                }

                continue;
            }

            if((area - 0) < -0.001) {
                //多边形面积不为零，使用weiler_Atherton算法
                //判断获取的多边形是否为顺时针，如果为逆时针则将vector内部数据倒序

                // vector<double> atom_x_Rever;
                // vector<double> atom_y_Rever;
                // atom_x_Rever.reserve(atom_x_Rever.size());
                // atom_y_Rever.reserve(atom_y_Rever.size());
                // //vector反转
                // //使用vector自带的反转迭代器reverse_iterator,rbegin(),rend()
                // vector<double>::reverse_iterator x_it;
                // for (x_it=atom_x.rbegin();x_it!=atom_x.rend();x_it++)
                // {
                //     atom_x_Rever.push_back(*x_it);
                // }
                // vector<double>::reverse_iterator y_it;
                // for (y_it=atom_y.rbegin();y_it!=atom_y.rend();y_it++)
                // {
                //     atom_y_Rever.push_back(*y_it);
                // }
                // atom_x.clear();
                // atom_y.clear();
                // atom_x = atom_x_Rever;
                // atom_y = atom_y_Rever;

                reverse(atom_x.begin(), atom_x.end());
                reverse(atom_y.begin(), atom_y.end());
            }

            WeilerAtherton pyclip, window;
            pyclip.pts.clear();
            window.pts.clear();
            for (int n = 0; n < atom_x.size(); n++) {
                RPoint p(atom_x[n], atom_y[n]);
                pyclip.pts.push_back(p);
            }

            RPoint w1(C.xmin, C.ymin);
            window.pts.push_back(w1);
            RPoint w2(C.xmax, C.ymin);
            window.pts.push_back(w2);
            RPoint w3(C.xmax, C.ymax);
            window.pts.push_back(w3);
            RPoint w4(C.xmin, C.ymax);
            window.pts.push_back(w4);

            coord_clip = weilerAtherton_reverse(window,pyclip);

            if(coord_clip.empty()){
                continue;
            }

            for(int m = 0; m < coord_clip.size(); m++){
                Atom atom_x_1;
                Atom atom_y_1;
                atom_x_1.clear();
                atom_y_1.clear();
                for (int k = 0; k < coord_clip[m].size(); k++) {
                    atom_x_1.push_back(coord_clip[m][k].x);
                    atom_y_1.push_back(coord_clip[m][k].y);
                }
                if (atom_x_1.size() <= 1) {
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
