//
// Created by liuxingbo03 on 2022/1/6.
//

#include "pycad/interface/converters/elementsConverter/PolylineInsConverter.h"


PolylineInsConverter::PolylineInsConverter() = default;


PolylineInsConverter::~PolylineInsConverter() = default;


void PolylineInsConverter::polylineIns2Jsonstr(PolylineIns &polylineIns) {
    // // 统计时间
    // // timer t_ogm;
    // clock_t start = clock();

    // // 打印提示信息 //TODO 应该做成装饰模式
    // cout << "[INFO] polylineIns2Jsonstr ..." << endl;

    // baseIns2Jsonstr
    baseIns2Jsonstr(polylineIns);

    // // 统计时间
    // // double time_cost = t_ogm.elapsed(); //秒
    // double time_cost = (double)(clock() - start) / CLOCKS_PER_SEC; //秒
    // cout << "[INFO] polylineIns2Jsonstr done, " << time_cost << "s has elapsed" << endl; //TODO: 装饰器怎么写出来？
}
