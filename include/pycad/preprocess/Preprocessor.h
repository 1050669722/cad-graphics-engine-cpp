//
// Created by liuxingbo03 on 2021/11/29.
//

#ifndef ATUOCADDATAANALYSISPYCADCPP_PREPROCESSOR_H
#define ATUOCADDATAANALYSISPYCADCPP_PREPROCESSOR_H


#include "pycad/Configuration.h"
#include "pycad/tools/BLockTools.h"

/*
 * 结合 配置文件 和 输入json文件 的
 * 预处理步骤
 */

class Preprocessor {
private:
    // 成员属性
    Configuration* ptr_config{};
    BLockTools* ptr_blockTools{};
    vector<string> sleptLayerNames{"frozen", "on"};

    // 成员方法
    void freezeLayer();
    void turnOffLayer();

public:
    // 成员属性

    // 成员方法
    Preprocessor();
    explicit Preprocessor(Configuration& config, BLockTools& blockTools);
    void preprocess();
};


#endif //ATUOCADDATAANALYSISPYCADCPP_PREPROCESSOR_H
