//
// Created by liuxingbo03 on 2021/11/11.
//

#ifndef ATUOCADDATAANALYSISPYCADCPP_SPLINESKETCHER_H
#define ATUOCADDATAANALYSISPYCADCPP_SPLINESKETCHER_H


#include "pycad/Structs.h"


class SplineSketcher {
private:
    // 成员属性
    int degree{}, sample_size{}, precision{};
    vector<double> knotvector;
    vector<int> spans;
    vector<vector<double>> basis;
    vector<vector<double>> ctrlpts;
    vector<vector<double>> eval_points;

    // 成员方法
    void kv_normalize();
    int find_span_linear(
                            int& ctrlpts_size,
                            double& knot
                        );
    void basis_functions(vector<double>& knots);
    void evaluate();
    void getAtom();

public:
    // 成员属性
    Atom atom_x;
    Atom atom_y;

    // 成员方法
    SplineSketcher();
    void convertAndAssignControlPoints(vector<map<string, double>>& controlPoints);
    explicit SplineSketcher(
                            int& degree, //=3,
                            const double& delta, //=0.01//,
                            // int precision=18
                            vector<map<string, double>>& controlPoints
                            );
    ~SplineSketcher();
};


#endif //ATUOCADDATAANALYSISPYCADCPP_SPLINESKETCHER_H
