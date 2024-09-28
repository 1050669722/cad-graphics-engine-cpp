//
// Created by liuxingbo03 on 2021/10/29.
//

#include <bits/stdc++.h>
// #include <io.h>
// #include <direct.h>
#include <string>
// #include <boost/timer.hpp>
#include <ctime>

#include "pycad/plot/Painter.h"
#include "pycad/tools/PainterTools.h"
#include "pycad/Utils.h"


// using namespace boost;
using namespace std;


Painter::Painter() = default;


Painter::Painter(Configuration &config, Database &database) {
    // 配置 数据结构 初始化环境
    this->config = &config;
    this->database = &database;
    setEnv();

    // // 用于搜集信息的数据结构
    // vector<AggData> aggData; //TODO
}


void Painter::adjustAppropriateScaleFactor(double cadxmin, double cadymin, double cadxmax, double cadymax) {
    // resotreResolution参与
    if (config->resotreResolution > 0.0) { //当config.yaml中这个字段空着时，根据ConfigConverter的规则，这里缺省为0.0
        config->restoreScaleFactor = config->resotreResolution / max(cadymax - cadymin, cadxmax - cadxmin);
    }

    // 绘图尺寸限制 比例反馈机制
    double currentResolutionHeight = config->restoreScaleFactor * (cadymax - cadymin);
    double limitHeight = 40000.0; //hard-coded
    if (currentResolutionHeight > limitHeight) {
        cout << "[WARNING] the window is too large, plotting may take a long time" << endl;
        config->restoreScaleFactor = limitHeight / (cadymax - cadymin);
        cout << "[WARNING] the \"painter.config.restoreScaleFactor\" has been modified to " << config->restoreScaleFactor << endl; //. or ->, 前者是给外部看的，后者是内部真正的
    }
}


vector<double> Painter::getMaxAreaLBRT() {
    return PainterTools::getMaxAreaLBRT(*database);
}


void Painter::setEnv() {
    // 根据图形 确定 绘图的返回 屏幕的尺寸
    vector<double> cadxmin_cadymin_cadxmax_cadymax = this->getMaxAreaLBRT();
    double cadxmin = cadxmin_cadymin_cadxmax_cadymax[0],
            cadymin = cadxmin_cadymin_cadxmax_cadymax[1],
            cadxmax = cadxmin_cadymin_cadxmax_cadymax[2],
            cadymax = cadxmin_cadymin_cadxmax_cadymax[3];
    adjustAppropriateScaleFactor(cadxmin, cadymin, cadxmax, cadymax);

    int opencvleft = (int)(cadxmin * config->restoreScaleFactor);
    int opencvtop = (int)(cadymin * config->restoreScaleFactor);
    int opencvright = (int)(cadxmax * config->restoreScaleFactor);
    int opencvbottom = (int)(cadymax * config->restoreScaleFactor);

    // 起始点
    auto originalPoint = Atom{cadxmin, cadymin};

    // 假·screen
    // Mat screenWithoutRedundant = Mat(opencvright - opencvleft + 0, opencvbottom - opencvtop + 0, CV_8UC3, config->colorMap["white"]);
    Mat screenWithoutRedundant = Mat(opencvbottom - opencvtop + 0, opencvright - opencvleft + 0, CV_8UC3, config->colorMap["white"]);
    MatrixXd restoreAffineWithoutRedundant = PainterTools::CAD2OpenCV(*config, screenWithoutRedundant, originalPoint);

    // 真·screen
    // screen = Mat(opencvright - opencvleft + config->redundantBorder, opencvbottom - opencvtop + config->redundantBorder, CV_8UC3, config->colorMap["white"]);
    screen = Mat(opencvbottom - opencvtop + config->redundantBorder, opencvright - opencvleft + config->redundantBorder, CV_8UC3, config->colorMap["white"]);
    restoreAffine = PainterTools::CAD2OpenCV(*config, screen, originalPoint);
}


void Painter::loadDataForOneFieldInDatabase(BaseFieldInDatabase& database_field) {
    // 坐标
    // 对于非polyline来说是这样的：[[()], [()], ...]
    // 对于polyline来说是这样的：[[(), (), ...], [(), (), ...], ...]
    painting_coords_x = &(database_field.coords_x);
    painting_coords_y = &(database_field.coords_y);
    // 颜色 //['c', 'c', ...]
    painting_colors = &(database_field.colors); //TODO
}


void Painter::shader(const char* eType) {
    /*
     * 加载数据
     * 当前正在画的[current painting]这类元素的 坐标、颜色
     */
    if (strcmp(eType, "lines") == 0) {
        loadDataForOneFieldInDatabase(database->lines);
    }
    else if (strcmp(eType, "polylines") == 0) {
        loadDataForOneFieldInDatabase(database->polylines);
    }
    else if (strcmp(eType, "circles") == 0) {
        loadDataForOneFieldInDatabase(database->circles);
    }
    else if (strcmp(eType, "arcs") == 0) {
        loadDataForOneFieldInDatabase(database->arcs);
    }
    else if (strcmp(eType, "ellipses") == 0) {
        loadDataForOneFieldInDatabase(database->ellipses);
    }
    else if (strcmp(eType, "points") == 0) {
        loadDataForOneFieldInDatabase(database->points);
    }
    else if (strcmp(eType, "splines") == 0) {
        loadDataForOneFieldInDatabase(database->splines);
    }
    else if (strcmp(eType, "texts") == 0) {
        loadDataForOneFieldInDatabase(database->texts);
    }
    else if (strcmp(eType, "mtexts") == 0) {
        loadDataForOneFieldInDatabase(database->mtexts);
    }
    else if (strcmp(eType, "attributes") == 0) {
        loadDataForOneFieldInDatabase(database->attributes);
    }
    else if (strcmp(eType, "alignedDimensions") == 0) {
        loadDataForOneFieldInDatabase(database->alignedDimensions);
    }
    // else if (strcmp(eType, "name") == 0) {
    //     // 坐标
    //     // 对于非polyline来说是这样的：[[()], [()], ...]
    //     // 对于polyline来说是这样的：[[(), (), ...], [(), (), ...], ...]
    //     painting_coords_x = database->name.coords_x;
    //     painting_coords_y = database->name.coords_y;
    //     // 颜色 //['c', 'c', ...]
    //     painting_colors = database->name.color;
    // }
    else {

    }
}


void Painter::paintCore(const char* eType) {
    // TODO: 如果没有动态特性，Painter::paintCore甚至没有必要出现
    // TODO: 多线程画？如果是分辨率很大的图像，多线程画是收益较大的
    // 加载数据
    shader(eType);

    // 遍历 //遍历格式统一到原子级别
    for (int i = 0; i < painting_coords_x->size(); ++i) {
        // 坐标
        Coord& coord_x = (*painting_coords_x)[i];
        Coord& coord_y = (*painting_coords_y)[i];
        // 颜色
        string& color = (*painting_colors)[i];

        for (int j = 0; j < coord_x.size(); ++j) {
            // 每一个atom
            Atom& atom_x = coord_x[j];
            Atom& atom_y = coord_y[j];

            // 空间变换
            PainterTools::atomOrientedAffine(atom_x, atom_y, restoreAffine);

            // opencv中的Point类
            cv::Point points[atom_x.size()];
            for (int m = 0; m < atom_x.size(); ++m) {
                points[m] = cv::Point(atom_x[m], atom_y[m]);
                // for-test
                if (atom_x[m] < -10000000) {
                    cout << "x: " << atom_x[m] << endl;
                }
            }
            const cv::Point* pts[] = {points};
            int npts[]{(int)atom_x.size()};

            // 画
            polylines(screen, pts, npts,1,false,config->colorMap[color],1,LINE_AA,0);
            if (strcmp(eType, "polylines") == 0) fillPoly(screen, pts, npts, 1, config->colorMap[color]); //如果图元类型是polylines，则还需要再填充一下 //TODO: 这两个命令究竟应该怎样使用？ //TODO: 有些polyline的线度为0，是不是不用填充，可以节省时间
            if (strcmp(eType, "points") == 0) {
                double pointSize = 0.0015 * (screen.size[0] + screen.size[1]) / 2.0; //1e-2能看见，1e-3看不见 //0.0015能看见，0.00125看不见
                circle(screen, (*pts)[0], (int)pointSize, config->colorMap[color], (int)(pointSize * 2), LINE_AA, 0); //为了确保圆画成实心的，所以这里thickness乘了2
            }
        }

    }

}


void Painter::paintC_lines() {
    paintCore("lines");
}


void Painter::paintC_polylines() {
    paintCore("polylines");
}


void Painter::paintC_circles() {
    paintCore("circles");
}


void Painter::paintC_arcs() {
    paintCore("arcs");
}


void Painter::paintC_ellipses() {
    paintCore("ellipses");
}


void Painter::paintC_points() {
    paintCore("points");
}


void Painter::paintC_splines() {
    paintCore("splines");
}


void Painter::paintC_texts() {
    paintCore("texts");
}


void Painter::paintC_mtexts() {
    paintCore("mtexts");
}


void Painter::paintC_attributes() {
    paintCore("attributes");
}


void Painter::paintC_alignedDimensions() {
    paintCore("alignedDimensions");
}


void Painter::paintC() {
    // 统计时间
    // timer t_ogm;
    clock_t start = clock();

    // 打印提示信息 //TODO 应该做成装饰模式
    cout << "[INFO] paintC ..." << endl;

    // 分 图元类型 画
    paintC_lines(); //TODO: 利用函数精简
    paintC_polylines();
    paintC_circles();
    paintC_arcs();
    paintC_ellipses();
    paintC_points();
    paintC_splines();
    paintC_texts();
    paintC_mtexts();
    paintC_attributes();
    paintC_alignedDimensions();

    // 统计时间
    // double time_cost = t_ogm.elapsed(); //秒
    double time_cost = (double)(clock() - start) / CLOCKS_PER_SEC; //秒
    cout << "[INFO] paintC done, " << time_cost << "s has elapsed" << endl; //TODO: 装饰器怎么写出来？
}


void Painter::paint() {
    /*
     * 逐 图元类型 画
     */
    // 对当前 这一类型的图元 的 所有实例 的坐标 画
    paintC();

    // 保存图片
    if (config->isSaveFig) {
        // char folderName[]{};
        // strncpy(folderName, config->savePath.c_str(), config->savePath.length() + 1);
        // if (_access(folderName, 0) == -1) {
        //     _mkdir(folderName);
        // }
        // imwrite(config->savePath + "/" + config->saveFileName, screen);

        Utils::makeDir(config->savePath);
        imwrite(Utils::joinPath(config->savePath, config->saveFileName), screen);

        // if (config->verbose) {
            cout << "[INFO] The picture has been saved to " << config->savePath << endl;
        // }
    }

    // 图像转换 //这里本来就是BGR的
    //

    // return screen;
}
