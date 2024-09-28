//
// Created by nan on 2021/12/17.
//

#ifndef ATUOCADDATAANALYSISPYCADCPP_CLIPSTRUCTS_H
#define ATUOCADDATAANALYSISPYCADCPP_CLIPSTRUCTS_H


#define MAX_NUM 10000


struct Key
{
    int d3;
    int d2;
    int d1;
    int d0;
};


struct Vertex
{
    double x, y;
    int table;
    Key code;
};


struct Graph
{
    Vertex ver[MAX_NUM + 1];
    int vexnum;
};//存储的是图形边界点（封闭凸图形）


struct WINDOW
{
    double xmin;
    double xmax;
    double ymin;
    double ymax;
};


struct point
{
    double x, y;
    point() = default;
    point(double xx, double yy)
            :x(xx), y(yy) {}

    bool operator < (const point& a)const //运算符重载
    {
        return x < a.x;
    }
};


struct EDGE//Edge边
{
    double bx, by, ex, ey;
    EDGE() = default;
    EDGE(double bxx, double byy, double exx, double eyy)
            :bx(bxx), by(byy), ex(exx), ey(eyy) {}
};


//窗口
struct Window {
public:
    // TODO: 暂且定义为double，原先是int
    double x, y;
    double width;
    double height;
    double l, r, t, b; //边界：左右上下
    Window(double x, double y, double width, double height) {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
        l = x;
        r = x + width;
        t = y + height;
        b = y;
    }
};


struct RPoint
{
    double x, y;
    RPoint() = default;
    RPoint(double x1, double y1) : x(x1), y(y1){}
};


typedef struct IntersectionPoint
{
    int pointFlag; //0表示被剪裁多边形，1表示交点
    int index0, index1;//属于被剪裁哪个线段的交点
    RPoint p;
    bool inFlag;//1表示入，0表示出
    double dis;//交点于启点的距离
}IP;


#endif //ATUOCADDATAANALYSISPYCADCPP_CLIPSTRUCTS_H
