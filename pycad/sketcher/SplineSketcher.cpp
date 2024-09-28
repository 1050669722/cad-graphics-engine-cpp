//
// Created by liuxingbo03 on 2021/11/11.
//

#include <cassert>
#include <cmath>

#include "pycad/Utils.h"
#include "pycad/sketcher/SplineSketcher.h"


SplineSketcher::SplineSketcher() = default;


void SplineSketcher::convertAndAssignControlPoints(vector<map<string, double>>& controlPoints) {
    for (auto& controlPoint : controlPoints) {
        this->ctrlpts.push_back(vector<double>{controlPoint["x"], controlPoint["y"]});
    }
}


SplineSketcher::SplineSketcher(int& degree, const double& delta, vector<map<string, double>>& controlPoints) {//, int precision
    assert(degree > 0); //, "degree must bigger than 0"
    assert((delta > 0.0) && (delta < 1.0)); //, "evaluation delta should be between 0.0 and 1.0"
    this->degree = degree;
    this->sample_size = (int)(floor((1.0 / delta) + 0.5)); //四舍五入取整
    // this->precision = precision;
    convertAndAssignControlPoints(controlPoints);
    evaluate();
    getAtom();
}


SplineSketcher::~SplineSketcher() {
    degree = -1;
    sample_size = -1;
    precision = -1;
}


void SplineSketcher::kv_normalize() {
    assert(!knotvector.empty()); //, "Input knot vector cannot be empty"
    double first_knot = knotvector[0];
    double last_knot = knotvector[knotvector.size() - 1];
    double denominator = last_knot - first_knot;
    for (auto& knot : knotvector) {
        // 标准化
        knot = (knot - first_knot) / denominator; //每一个 与 首个 的差距 相对于 总体差距的尺寸
    }
}


int SplineSketcher::find_span_linear(int &ctrlpts_size, double &knot) {
    int span = degree + 1;
    while ((span < ctrlpts_size) && (knotvector[span] <= knot)) {
        span += 1;
    }
    return span - 1;
}


void SplineSketcher::basis_functions(vector<double> &knots) {
    auto s_ptr = spans.begin();
    auto k_ptr = knots.begin();

    for (
            ;
            (s_ptr != spans.end()) || (k_ptr != knots.end());
            s_ptr++, k_ptr++
            ) {
        vector<double> left(degree + 1, 0.0);
        vector<double> right(degree + 1, 0.0);
        vector<double> N(degree + 1, 1.0);

        for (int j = 1; j < degree + 1; j++) {
            left[j] = (*k_ptr) - knotvector[(*s_ptr) + 1 - j];
            right[j] = knotvector[(*s_ptr) + j] - (*k_ptr);
            double saved = 0.0;

            for (int r = 0; r < j; r++) {
                double temp = N[r] / (right[r + 1] + left[j - r]);
                N[r] = saved + right[r + 1] * temp;
                saved = left[j - r] * temp;
            }
            N[j] = saved;
        }

        basis.push_back(N);
    }

}


void SplineSketcher::evaluate() {
    // 配置
    // int ctrlpts_size = ctrlpts.size();
    // int dimension = ctrlpts[0].size();
    int ctrlpts_size = 0;
    int dimension = 0;
    for (auto& ctrlpt : ctrlpts) { //TODO: 如何优雅地获取一个vector的长度
        ctrlpts_size++;
    }
    for (auto& _ : ctrlpts[0]) {
        dimension++;
    }

    // 检查维度
    assert(ctrlpts_size >= 0); //, "the size of input points is zeros."
    for (auto ctrlpts_ptr = ctrlpts.begin(); ctrlpts_ptr != ctrlpts.end(); ctrlpts_ptr++) {
        assert(ctrlpts_ptr->size() == dimension); //, "The dimension of input points is not the same."
    }

    // 设置knots
    int num_segments = ctrlpts_size - (degree + 1);
    for (int i = 0; i < degree; i++) {
        knotvector.push_back(0.0);
    }
    vector<double> tmp_knotvector = Utils::linspace(0.0, 1.0, num_segments + 2);
    knotvector.insert(knotvector.end(), tmp_knotvector.begin(), tmp_knotvector.end());
    for (int i = 0; i < degree; i++) {
        knotvector.push_back(1.0);
    }
    kv_normalize();
    assert(knotvector.size() == degree + ctrlpts_size + 1); //, "Input is not a valid knot vector"

    // 计算basis
    double start = knotvector[degree];
    double stop = knotvector[knotvector.size() - (degree + 1)]; //double stop = knotvector[knotvector.size() - 12];
    vector<double> knots = Utils::linspace(start, stop, sample_size);
    for (double & knot : knots) {
        spans.push_back(find_span_linear(ctrlpts_size, knot));
    }
    basis_functions(knots);

    // 计算样条点
    for (int idx = 0; idx < knots.size(); idx++) {
        vector<double> crvpt(dimension, 0.0);
        for (int i = 0; i < degree + 1; i++) {
            vector<double> tmp_ctrlpt = ctrlpts[spans[idx] - degree + i];
            auto crv_p = crvpt.begin();
            auto ctl_p = tmp_ctrlpt.begin();
            for (; (crv_p != crvpt.end()) || (ctl_p != tmp_ctrlpt.end()); crv_p++, ctl_p++) {
                (*crv_p) = (*crv_p) + (basis[idx][i] * (*ctl_p));
            }
        }
        eval_points.push_back(crvpt);
    }

}


void SplineSketcher::getAtom() {
    for (auto& eval_point : eval_points) {
        atom_x.push_back(eval_point[0]);
        atom_y.push_back(eval_point[1]);
    }
}
