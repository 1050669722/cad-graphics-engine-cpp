//
// Created by liuxingbo03 on 2021/10/11.
//

#ifndef ATUOCADDATAANALYSISPYCADCPP_CONFIGURATION_H
#define ATUOCADDATAANALYSISPYCADCPP_CONFIGURATION_H


#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <opencv2/opencv.hpp>


using namespace std;
using namespace cv;


typedef map<string, vector<string>> Accusation;
typedef map<string, Accusation> Judgements;


class Configuration {
private:
    // 成员属性


    // 成员方法


public:
    // 成员属性
    const wchar_t* jsonPath; //可以写中文
    string jsonPathFromPython; //为python层做的，可以写中文
    string cache; //不写中文 //python层可以写中文
    double restoreScaleFactor;
    double resotreResolution;
    int redundantBorder;
    map<string, Scalar> colorMap;
    bool isSaveFig;
    string savePath; //不写中文 //python层可以写中文
    string saveFileName; //不写中文 //python层可以写中文
    bool verbose;
    string fontfile;
    int fontsize;
    vector<string> vacantTextInfo;
    string judgementsInterOrUnion;
    Judgements judgements;
    vector<string> whitelistForType;
    vector<double> region; //TODO: 原先是regions
    string insideOrOutside;

    // 成员方法
    Configuration();
};


#endif //ATUOCADDATAANALYSISPYCADCPP_CONFIGURATION_H
