//
// Created by liuxingbo03 on 2022/1/7.
//

#include "pycad/interface/converters/elementsConverter/ArcInsConverter.h"


ArcInsConverter::ArcInsConverter() = default;


ArcInsConverter::~ArcInsConverter() = default;


void ArcInsConverter::arcIns2Jsonstr(ArcIns &arcIns) {
    // // 统计时间
    // // timer t_ogm;
    // clock_t start = clock();

    // // 打印提示信息 //TODO 应该做成装饰模式
    // cout << "[INFO] arcIns2Jsonstr ..." << endl;

    // baseIns2Jsonstr
    baseIns2Jsonstr(arcIns);

    // // 统计时间
    // // double time_cost = t_ogm.elapsed(); //秒
    // double time_cost = (double)(clock() - start) / CLOCKS_PER_SEC; //秒
    // cout << "[INFO] arcIns2Jsonstr done, " << time_cost << "s has elapsed" << endl; //TODO: 装饰器怎么写出来？
}
