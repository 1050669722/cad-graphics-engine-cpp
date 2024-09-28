//
// Created by liuxingbo03 on 2021/11/3.
//

#ifndef ATUOCADDATAANALYSISPYCADCPP_PARSESTRUCTS_H
#define ATUOCADDATAANALYSISPYCADCPP_PARSESTRUCTS_H


#include <deque>

#include "pycad/Structs.h"


// parse组件中被复用的结构体 记录在这里


// TransSolution 字段 类型
typedef vector<string> Records;
typedef map<string, vector<double>> ScaleFactors; //typedef map<string, deque<double>> ScaleFactors; //
typedef vector<double> Rotations;
typedef map<string, vector<double>> Positions; //typedef map<string, deque<double>> Positions; //
typedef map<string, vector<double>> Normals; //typedef map<string, deque<double>> Normals; //
typedef vector<string> Layers;
typedef deque<bool> IsDyns; //从 bool isDyn 改为现在的样子
typedef vector<string> BLockReferenceHandles;
typedef deque<bool> Visibles;
typedef deque<bool> IsFromHatchs;

// TODO 搜集这些字段的类型 现在给出的类型只是可能的类型
typedef vector<vector<string>> Attributes;
typedef vector<SpatialFilterInfoDict> SpatialFilter;


// Tree 字段 类型
typedef vector<string> Pedigree;


#endif //ATUOCADDATAANALYSISPYCADCPP_PARSESTRUCTS_H
