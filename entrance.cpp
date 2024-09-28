//
// Created by liuxingbo03 on 2021/10/21.
//

// TODO: 检查每一个文件，凡是指针类型的，变量名都修改为以ptr_为开端

#include <iostream>
// #include <boost/timer.hpp>
#include <ctime>

#include "pycad/Structs.h"
#include "pycad/Configuration.h"
#include "pycad/tools/BLockTools.h"
#include "pycad/preprocess/Preprocessor.h"
#include "pycad/parse/Tree.h"
#include "pycad/filter/Filter.h"
#include "pycad/clip/Clipper.h"
#include "pycad/plot/Painter.h"
#include "pycad/Utils.h"

#include "pycad/interface/converters/databaseConverter/DatabaseConverter.h" //TODO: 删除这一行


using namespace std;
// using namespace boost;


int main() {
    // 统计时间
    // timer t_ogm;
    clock_t start = clock();

    // 配置文件对象以及指针
    Configuration config = Configuration();
    Configuration* ptr_config = &config;

    // 根 rootBlock
    // BLockTools blockTools = BLockTools(config.jsonPath);
    string strJsonPath = string(string(config.jsonPath));
    BLockTools blockTools = BLockTools(strJsonPath);
    BLock* ptr_rootBlock = blockTools.rootBlock;

    // 预处理
    Preprocessor preprocessor = Preprocessor(config, blockTools);
    preprocessor.preprocess();

    // database //出于语言特性和运行效率的考量，这里将Tree的实例封装在Tree::blossom里 //TODO: 这只是个示例，真正封装的时候，应该将Tree::bloom中的*(tree.database)取出来直接输入到Painter中去； //Tree的准备工作太多，如果在这里实例化，会比较累赘
    Database database = Database{};
    Database* ptr_database = &database;
    Tree::bloom(ptr_database, ptr_config, ptr_rootBlock); //TODO: 是否省去Tree的返回值Database而作为属性？

    // // for-test //TODO: 删除这五行
    // DatabaseConverter dbCvter = DatabaseConverter();
    // dbCvter.database2jsonstr(database);
    // dbCvter.jsonstr2database(dbCvter.outputDatabase);
    // Database restoreDatabase = dbCvter.inputDatabase;

    // 筛选器对象
    Filter filter = Filter(config);
    filter.filterDatabase(database);

    // 区域筛选器对象
    Clipper clipper = Clipper(config);
    clipper.clipDatabase(filter.newDatabase);

    // 绘画器对象
    // Painter painter = Painter(config, database);
    // Painter painter = Painter(config, filter.newDatabase);
    Painter painter = Painter(config, clipper.newDatabase);
    painter.paint(); //TODO: 这只是个示例，真正封装的时候，应该将painter.screen返回
    Mat& img = painter.screen;

    // 统计时间
    // double time_cost = t_ogm.elapsed(); //秒
    double time_cost = (double)(clock() - start) / CLOCKS_PER_SEC; //秒
    cout << "[INFO] main done, " << time_cost << "s has elapsed" << endl; //TODO: 装饰器怎么写出来？

    return 0;
}

