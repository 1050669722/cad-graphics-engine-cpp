//
// Created by liuxingbo03 on 2021/10/26.
//


#include <cmath>
#include <algorithm>
#include <stdexcept>

#include "pycad/Utils.h"


wstring Utils::s2ws(string &str) {
    if (str.empty()) {
        return L"";
    }
    unsigned len = str.size() + 1;
    setlocale(LC_CTYPE, "en_US.UTF-8");
    unique_ptr<wchar_t[]> p(new wchar_t[len]);
    mbstowcs(p.get(), str.c_str(), len);
    wstring w_str(p.get());
    return w_str;
}


string Utils::ws2s(wstring &w_str) {
    if (w_str.empty()) {
        return "";
    }
    unsigned len = w_str.size() * 4 + 1;
    setlocale(LC_CTYPE, "en_US.UTF-8");
    unique_ptr<char[]> p(new char[len]);
    wcstombs(p.get(), w_str.c_str(), len);
    string str(p.get());
    return str;
}


int Utils::getUtf8LetterNumber(string &str) {
    // 转换
    char* strc = new char[strlen(str.c_str()) + 1];
    strcpy(strc, str.c_str());

    // 统计
    int i = 0, j = 0;
    while (strc[i])
    {
        if ((strc[i] & 0xc0) != 0x80) j++;
        i++;
    }

    // 清除
    delete[] strc;

    return j;
}


bool Utils::isExist(string &path) {
    /*
     * 判断文件或目录是否存在
     */
    struct stat buffer{};
    return (stat(path.c_str(), &buffer) == 0);
}


bool Utils::isFile(string &path) {
    if (!isExist(path)) {
        // printf("[ERROR] %s: %d %s not exists\n", __FILE__, __LINE__, path.c_str());
        printf("[ERROR] %s not exists\n", path.c_str());
        return false;
    }
    struct stat buffer{};
    return (stat(path.c_str(), &buffer) == 0 && S_ISREG(buffer.st_mode));
}


bool Utils::isDir(string &path) {
    if (!isExist(path)) {
        printf("[ERROR] %s not exists\n", path.c_str());
        return false;
    }
    struct stat buffer{};
    return (stat(path.c_str(), &buffer) == 0 && S_ISDIR(buffer.st_mode));
}


int Utils::getFileSize(const wchar_t *filepath) {
    /*
     * 获取文件大小
     */
    struct stat statbuf{};
    int ret;
    ret = wstat(filepath, &statbuf);
    if (ret != 0) return -1;
    return statbuf.st_size; //返回文件大小
}


int Utils::getFileSize(string &filepathFromPython) {
    /*
     * 获取文件大小
     */
    struct stat statbuf{};
    int ret;
    ret = stat(filepathFromPython.c_str(), &statbuf); //调用stat函数
    if (ret != 0) return -1; //获取失败
    return statbuf.st_size; //返回文件大小
}


string Utils::joinPath(string &path, string &name) {
    int dirSize = (int)(path.size());

    int lastSprtIndex = (int)(path.find_last_of('/'));
    bool lastIsSprt = (lastSprtIndex == dirSize - 1);

    if (lastIsSprt) {
        return path + name;
    }
    else {
        return path + "/" + name;
    }
}


bool Utils::makeDir(string& path) {
    if (!isExist(path)) {
        #ifdef _WIN32
            int isCreate = mkdir(path.c_str()); //TODO: 如果在linux平台，是否需要添加权限参数？
        #else
            int isCreate = mkdir(path.c_str(), 00700);
        #endif
        if (isCreate != 0) {
            printf("[INFO] the dir: %s has not been created correctly\n", path.c_str());
            return false;
        }
        else {
            printf("[INFO] the dir: %s has been created\n", path.c_str());
            return true;
        }
    }
    else {
        printf("[INFO] the dir: %s has existed\n", path.c_str());
        return false;
    }
}


bool Utils::isEqual(const double &a, const double &b, const double &threshold) {
    /*
     * 判断两数是否相等
     */
    return abs(a - b) < threshold;
}


void Utils::restrict(double &value, const double &low, const double &high) {
    /*
     * 限制一个数的范围 in-place
     */
    value = min(max(value, low), high);
}


void Utils::linspace(vector<double> &container, const double &start, const double &stop, const int &num) {
    /*
     * linspace | start, stop, num
     */
    if (num < 1) {
        throw runtime_error("[ERROR] Invalid Parameter \"num\"");
    }
    else if ((isEqual(start, stop, 1e-8)) || (num == 1)) {
        container.push_back(start);
    }
    else {
        int intervalNum = num - 1;
        double interval = (stop - start) / (double)intervalNum;
        for (int i = 0; i < num; ++i) {
            double point = start + (double)i * interval;
            container.push_back(point);
        }
    }
}


vector<double> Utils::linspace(const double &start, const double &stop, const int &num) {
    vector<double> container;
    linspace(container, start, stop, num);
    return container;
}


double Utils::calculateDistanceBetweenTwoPoints(vector<double> &A, vector<double> &B) {
    return sqrt(pow(A[0] - B[0], 2) + pow(A[1] - B[1], 2));
}


double Utils::calculateAreaOfPolygon(Atom &atom_x, Atom &atom_y) {
    /*
 * 计算[任意]多边形的面积
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
    return 0.5 * abs(area);
}


void Utils::generateScaleMat(MatrixXd &mat, const double &scaleFactor_x, const double &scaleFactor_y) {
    mat(0, 0) = scaleFactor_x;
    mat(1, 1) = scaleFactor_y;
}


MatrixXd Utils::generateScaleMat(const double &scaleFactor_x, const double &scaleFactor_y) {
    MatrixXd mat = MatrixXd::Identity(3, 3);
    generateScaleMat(mat, scaleFactor_x, scaleFactor_y);
    return mat;
}


void Utils::generateRotateMat(MatrixXd &mat, const double &rotation) {
    mat(0, 0) = cos(rotation);
    mat(0, 1) = -1.0 * sin(rotation);
    mat(1, 0) = sin(rotation);
    mat(1, 1) = cos(rotation);
}


MatrixXd Utils::generateRotateMat(const double &rotation) {
    MatrixXd mat = MatrixXd::Identity(3, 3);
    generateRotateMat(mat, rotation);
    return mat;
}


void Utils::generateDisplaceMat(MatrixXd &mat, const double &displacement_x, const double &displacement_y) {
    mat(0, 2) = displacement_x;
    mat(1, 2) = displacement_y;
}


MatrixXd Utils::generateDisplaceMat(const double &displacement_x, const double &displacement_y) {
    MatrixXd mat = MatrixXd::Identity(3, 3);
    generateDisplaceMat(mat, displacement_x, displacement_y);
    return mat;
}


void Utils::generateAffineMat(
        MatrixXd &mat,
        const double &scaleFactor_x,
        const double &scaleFactor_y,
        const double &rotation,
        const double &displacement_x,
        const double &displacement_y) {
    /*
     * normalZ的可能作用，请在调用本函数时，自动乘在scaleFactor_x和rotation这两个参数上
     */
    // 缩放 矩阵
    MatrixXd scaleMat = MatrixXd::Identity(3, 3);
    generateScaleMat(scaleMat, scaleFactor_x, scaleFactor_y);

    // 旋转 矩阵
    MatrixXd rotateMat = MatrixXd::Identity(3, 3);
    generateRotateMat(rotateMat, rotation);

    // 位移 矩阵
    MatrixXd displaceMat = MatrixXd::Identity(3, 3);
    generateDisplaceMat(displaceMat, displacement_x, displacement_y);

    // 仿射变换 矩阵
    mat = displaceMat * rotateMat * scaleMat; //TODO: 本项目中的仿射变换，均采用矩阵左乘列向量的模式
}


MatrixXd Utils::generateAffineMat(
        const double &scaleFactor_x,
        const double &scaleFactor_y,
        const double &rotation,
        const double &displacement_x,
        const double &displacement_y) {
    /*
     * normalZ的可能作用，请在调用本函数时，自动乘在scaleFactor_x和rotation这两个参数上
     */
    MatrixXd affineMat = MatrixXd::Identity(3, 3);
    generateAffineMat(
            affineMat,
            scaleFactor_x,
            scaleFactor_y,
            rotation,
            displacement_x,
            displacement_y
            );
    return affineMat;
}


void Utils::atomOrientedAffine(Atom &atomX, Atom &atomY, MatrixXd &affineMat) {
    // 坐标矩阵 //每列表示一个点的坐标
    auto atom_size = atomX.size();
    VectorXd vec_x = Map<VectorXd>(atomX.data(), (Index)atom_size);
    VectorXd vec_y = Map<VectorXd>(atomY.data(), (Index)atom_size);
    VectorXd vec_ones = VectorXd::Ones((Index)atom_size);
    MatrixXd coordMat = MatrixXd::Zero((Index)atom_size, 3);
    coordMat << vec_x, vec_y, vec_ones;
    coordMat.transposeInPlace();

    // 进行仿射变换
    coordMat = affineMat * coordMat;

    // 转换回来
    coordMat.transposeInPlace();

    // 修改Atom
    atomX = Atom(coordMat.col(0).data(), coordMat.col(0).data() + coordMat.col(0).size());
    atomY = Atom(coordMat.col(1).data(), coordMat.col(1).data() + coordMat.col(1).size());
}


void Utils::wirteDatabaseForOneFieldInDatabaseOneStep(
        BaseFieldInDatabase &database_field,
        Coord &coord_x,
        Coord &coord_y,
        string &color,
        string &handle,
        Criterion &criterion
        ) {
    // 坐标
    database_field.coords_x.push_back(coord_x); //TODO: database应该保存值，还是应该保存指针？
    database_field.coords_y.push_back(coord_y); //TODO: 在调用层，Tree的实例会不会被保留，或者会不会被修改？
    // 其它
    database_field.colors.push_back(color);
    database_field.handles.push_back(handle);
    // 判据
    database_field.criterions.push_back(criterion);
}
