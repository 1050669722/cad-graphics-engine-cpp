//
// Created by liuxingbo03 on 2021/10/11.
//

#include <opencv2/opencv.hpp>

#include "pycad/Configuration.h"


using namespace std;
using namespace cv;


Configuration::Configuration() {
    jsonPath = L"../json/15.集团版YJ160-11(T2-17X)_t3.json"; //L""
    // jsonPath = L"../json/15.YJ160-11(T2-17X)_t3.json";
    // jsonPath = L"../json/15.YJ160-11(T2-17X)_t3_1229.json"; //L""
    // jsonPath = L"../json/15.集团版YJ160-11(T2-17X)_t3 - forMtext.json"; //L""
    // jsonPath = L"../json/15.集团版YJ160-11(T2-17X)_t3 - 副本.json"; //L""
    // jsonPath = L"../json/qwerty.json";
    // jsonPath = L"../json/3-20.json";
    // jsonPath = L"../json/01_20211025.json"; //for-test hatch
    // jsonPath = L"D:\\项目-代码和数据仅在本地\\CAD\\解析dxf\\桌面上的CAD文件夹和文件\\CAD相关_桌面临时\\贾振威\\1208\\01_WSP-2429A1.json"; //for-test hatch
    // jsonPath = L"../json/01_WSP-2429A1.json";
    // jsonPath = L"D:/项目-代码和数据仅在本地/CAD/解析dxf/桌面上的CAD文件夹和文件/CAD相关_桌面临时/王婷/1228/茂名碧桂园天钻2#楼.json";
    // jsonPath = L"../json/门窗新图元模板参考.json";
    // jsonPath = L"../json/门窗新图元模板参考_画了一个矩形框.json";
    // jsonPath = L"../json/20200245-01D001-1#楼-建筑_门窗大样及表t3.json";
    // jsonPath = L"../json/Drawing2.json";

    cache = "../cache";

    colorMap = map<string, Scalar>{ //这里是BGR
            pair<string, Scalar>("r", Scalar(0, 0, 255)), //255, 0, 0
            pair<string, Scalar>("g", Scalar(0, 255, 0)), //0, 255, 0
            pair<string, Scalar>("b", Scalar(255, 0, 0)), //0, 0, 255
            pair<string, Scalar>("m", Scalar(255, 0, 255)), //255, 0, 255
            pair<string, Scalar>("white", Scalar(255, 255, 255)), //255, 255, 255
            pair<string, Scalar>("yellow", Scalar(0, 255, 255)), //255, 255, 0
            pair<string, Scalar>("orange", Scalar(0, 128, 255)), //255, 128, 0
            pair<string, Scalar>("pink", Scalar(203, 192, 255)), //255, 192, 203
            pair<string, Scalar>("violet", Scalar(226, 43, 138)), //138, 43, 226
            pair<string, Scalar>("black", Scalar(0, 0, 0)), //0, 0, 0
            pair<string, Scalar>("SlateBlue", Scalar(205, 90, 106)), //106, 90, 205
            pair<string, Scalar>("SaddleBrown", Scalar(19, 69, 139)), //139, 69, 19
            pair<string, Scalar>("grassgreen", Scalar(35, 142, 107)), //107, 142, 35
            pair<string, Scalar>("chocolate", Scalar(30, 105, 210)), //210, 105, 30
            pair<string, Scalar>("mazarine", Scalar(112, 25, 25)), //25, 25, 112
            pair<string, Scalar>("TurquoiseBlue", Scalar(140, 199, 0)), //0, 199, 140
    };

    restoreScaleFactor = 0.1; // 1.0; //0.5; // //TODO: 需要寻找到一个合适的最大尺寸（参考pygame），因为opencv好像不会报错却画得时间特别长，而且画出来空白
    resotreResolution = 0.0; //3000.0; //

    redundantBorder = 15;

    isSaveFig = true;

    savePath = "../figs"; //TODO: 不要带有中文 //这个文件仅用于调试

    time_t rawtime;
    struct tm* timeinfo;
    char buffer [128];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H-%M-%S", timeinfo);
    saveFileName = (string)buffer + ".png"; //TODO: 不要带有中文 //这个文件仅用于调试

    verbose = false; // true; //

    fontfile = "";

    fontsize = 100;

    vacantTextInfo = vector<string>{"''", "\"\"", "'", "\""};

    judgementsInterOrUnion = "Inter"; //"Union";

    judgements["pedigreeLogger"]["wanted"] = vector<string>{""};
    judgements["pedigreeLogger"]["blackList"] = vector<string>{""};
    judgements["layerHistory"]["wanted"] = vector<string>{""};
    judgements["layerHistory"]["blackList"] = vector<string>{""}; //G-建筑-规划建筑外墙
    judgements["BRHandleHistory"]["wanted"] = vector<string>{""};
    judgements["BRHandleHistory"]["blackList"] = vector<string>{""};

    whitelistForType = vector<string>{""}; //优先级顺序：whitelistForType > blackList > wanted

    region = vector<double>{}; //495486, 4632, 504886, 29262 //460960.587, 89527.829, 463633.59, 96084.393 //xmin, ymin, xmax, ymax
    insideOrOutside = "Inside"; //"Outside"; //
}
