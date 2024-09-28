//
// Created by nan on 2021/11/17.
//

#ifndef ATUOCADDATAANALYSISPYCADCPP_WEILERATHERTON_H
#define ATUOCADDATAANALYSISPYCADCPP_WEILERATHERTON_H

#include <list>

#include "pycad/clip/ClipStructs.h"
#include "pycad/Configuration.h"


using namespace std;


class WeilerAtherton {
private:
    // 成员属性
    // Coords coords_x;
    // Coords coords_y;
    Configuration* config{};
    SutherlandHodgman sutherlandHodgmanClip{};
    vector<vector<RPoint>> coord_clip;

    // 成员方法

public:
    // 成员属性
    vector<RPoint> pts;
    vector<vector<RPoint>> weilerAtherton(WeilerAtherton& pyclip, WeilerAtherton& py);
    vector<vector<RPoint>> weilerAtherton_reverse(WeilerAtherton& pyclip, WeilerAtherton& py);

    // 成员方法
    WeilerAtherton();
    explicit WeilerAtherton(Configuration& config);
    bool isPointInsidePg(RPoint p, WeilerAtherton& py);
    int cross(RPoint& p0, RPoint& p1, RPoint& p2);
    bool onSegment(RPoint& p0, RPoint& p1, RPoint& p2);
    bool segmentsIntersect(RPoint& p1, RPoint& p2, RPoint& p3, RPoint& p4);
    RPoint getintersectPoint(RPoint p1, RPoint p2, RPoint p3, RPoint p4);
    void generateIntersectPoints(WeilerAtherton& pyclip, WeilerAtherton& py, list<IP>& iplist);
    int getDistance(RPoint& p1, RPoint& p2);
    static bool distanceComparator(IP& ip1, IP& ip2);
    void generateList(WeilerAtherton& py, list<IP>& iplist, list<IP>& comlist, int index);
    void getPgPointInOut(list<IP>& Pglist, WeilerAtherton& pyclip);
    double calculateAreaOfPolygon(Atom& atom_x, Atom& atom_y);
    bool isPointInsidePoly(RPoint P,const vector<RPoint>& polyVertices);
    void getClipPointInOut(list<IP>& cliplist, list<IP>& Pglist);
    void generateClipArea(list<IP>& Pglist, list<IP>& cliplist ,vector<vector<RPoint>>& output);
    void generateClipArea_reverse(list<IP>& Pglist, list<IP>& cliplist ,vector<vector<RPoint>>& output);
    void clipCoords(Coords& coords_x, Coords& coords_y);
    void clipCoords_reverse(Coords& coords_x, Coords& coords_y);
};


#endif //ATUOCADDATAANALYSISPYCADCPP_WEILERATHERTON_H
