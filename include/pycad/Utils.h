//
// Created by liuxingbo03 on 2021/10/26.
//

#ifndef ATUOCADDATAANALYSISPYCADCPP_UTILS_H
#define ATUOCADDATAANALYSISPYCADCPP_UTILS_H


#include <sys/stat.h>
#include <sys/types.h>
#include <codecvt>
#include <string>

#include <Eigen/Dense>

#include "pycad/Structs.h"
#include "pycad/Database.h"


using namespace std;
using namespace Eigen;


class Utils {
public:
    // string 转换为 wstring
    static wstring s2ws(string& str);

    // wstring 转换为 string
    static string ws2s(wstring& w_str);

    // 获取utf-8字符个数（utf-8下，英文字符一个站位一个字节，中文字符一个站位3个字节）
    static int getUtf8LetterNumber(string& str);

    // 判断文件或目录是否存在
    static bool isExist(string& path);

    // 判断路径是否为文件
    static bool isFile(string& path);

    // 判断路径是否为目录
    static bool isDir(string& path);

    // 获取文件大小
    static int getFileSize(const wchar_t* filepath);
    static int getFileSize(string& filepathFromPython);

    // 拼接路径
    static string joinPath(string& path, string& name);

    // 创建目录
    static bool makeDir(string& path);

    // 判断两数是否相等
    static bool isEqual(const double &a, const double &b, const double& threshold);

    // 限制一个数的范围 in-place
    static void restrict(double& value, const double& low, const double& high);

    // linspace | start, stop, num
    static void linspace(vector<double>& container, const double& start, const double& stop, const int& num);
    static vector<double> linspace(const double& start, const double& stop, const int& num);

    // 计算两点之间的距离
    static double calculateDistanceBetweenTwoPoints(vector<double>& A, vector<double>& B);

    // 计算[任意]多边形的面积
    static double calculateAreaOfPolygon(Atom& atom_x, Atom& atom_y);

    // 构造缩放矩阵
    static void generateScaleMat(MatrixXd& mat, const double& scaleFactor_x, const double& scaleFactor_y);
    static MatrixXd generateScaleMat(const double& scaleFactor_x, const double& scaleFactor_y);

    // 构造旋转矩阵
    static void generateRotateMat(MatrixXd& mat, const double& rotation);
    static MatrixXd generateRotateMat(const double& rotation);

    // 构造位移矩阵
    static void generateDisplaceMat(MatrixXd& mat, const double& displacement_x, const double& displacement_y);
    static MatrixXd generateDisplaceMat(const double& displacement_x, const double& displacement_y);

    // 构造仿射变换矩阵
    static void generateAffineMat(
            MatrixXd& mat,
            const double& scaleFactor_x,
            const double& scaleFactor_y,
            const double& rotation,
            const double& displacement_x,
            const double& displacement_y
            );
    static MatrixXd generateAffineMat(
            const double& scaleFactor_x,
            const double& scaleFactor_y,
            const double& rotation,
            const double& displacement_x,
            const double& displacement_y
            );

    // 面向Atom的仿射变换
    static void atomOrientedAffine(Atom &atomX, Atom &atomY, MatrixXd &affineMat);

    // 对于database的某一类图元的各个属性，添加一组值
    static void wirteDatabaseForOneFieldInDatabaseOneStep(
            BaseFieldInDatabase& database_field,
            Coord& coord_x,
            Coord& coord_y,
            string& color,
            string& handle,
            Criterion& criterion
    );
};


#endif //ATUOCADDATAANALYSISPYCADCPP_UTILS_H
