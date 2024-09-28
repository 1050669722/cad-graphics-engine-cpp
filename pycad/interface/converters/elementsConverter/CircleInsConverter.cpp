//
// Created by liuxingbo03 on 2022/1/7.
//

#include "pycad/interface/converters/elementsConverter/CircleInsConverter.h"


CircleInsConverter::CircleInsConverter() = default;


CircleInsConverter::~CircleInsConverter() = default;


void CircleInsConverter::circleIns2Jsonstr(CircleIns &circleIns) {
    // // 统计时间
    // // timer t_ogm;
    // clock_t start = clock();

    // // 打印提示信息 //TODO 应该做成装饰模式
    // cout << "[INFO] circleIns2Jsonstr ..." << endl;

    // baseIns2Jsonstr
    baseIns2Jsonstr(circleIns);

    // // 统计时间
    // // double time_cost = t_ogm.elapsed(); //秒
    // double time_cost = (double)(clock() - start) / CLOCKS_PER_SEC; //秒
    // cout << "[INFO] circleIns2Jsonstr done, " << time_cost << "s has elapsed" << endl; //TODO: 装饰器怎么写出来？
}
