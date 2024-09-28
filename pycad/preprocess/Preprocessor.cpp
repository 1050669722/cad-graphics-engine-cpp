//
// Created by liuxingbo03 on 2021/11/29.
//

// #include <boost/timer.hpp>
#include <ctime>
#include "pycad/preprocess/Preprocessor.h"


// using namespace boost;
using namespace std;


Preprocessor::Preprocessor() = default;


Preprocessor::Preprocessor(Configuration &config, BLockTools &blockTools) {
    ptr_config = &config;
    ptr_blockTools = &blockTools;
}


void Preprocessor::freezeLayer() {
    for (auto& layerInfoDict : ptr_blockTools->infos.layerInfoDicts) {
        if (layerInfoDict.frozen) {
            ptr_config->judgements["layerHistory"]["blackList"].push_back(layerInfoDict.name);
        }
    }
}


void Preprocessor::turnOffLayer() {
    for (auto& layerInfoDict : ptr_blockTools->infos.layerInfoDicts) {
        if (layerInfoDict.on) {
            ptr_config->judgements["layerHistory"]["blackList"].push_back(layerInfoDict.name);
        }
    }
}


void Preprocessor::preprocess() {
    // 统计时间
    // timer t_ogm;
    clock_t start = clock();

    // // 打印提示信息
    cout << "[INFO] preprocess ..." << endl;

    // 预处理
    freezeLayer();
    turnOffLayer();

    // 统计时间
    // double time_cost = t_ogm.elapsed(); //秒
    double time_cost = (double)(clock() - start) / CLOCKS_PER_SEC; //秒
    cout << "[INFO] preprocess done, " << time_cost << "s has elapsed" << endl; //TODO: 装饰器怎么写出来？
}



