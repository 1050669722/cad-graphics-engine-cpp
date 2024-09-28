//
// Created by liuxingbo03 on 2021/10/21.
//

// TODO: ���ÿһ���ļ�������ָ�����͵ģ����������޸�Ϊ��ptr_Ϊ����

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

#include "pycad/interface/converters/databaseConverter/DatabaseConverter.h" //TODO: ɾ����һ��


using namespace std;
// using namespace boost;


int main() {
    // ͳ��ʱ��
    // timer t_ogm;
    clock_t start = clock();

    // �����ļ������Լ�ָ��
    Configuration config = Configuration();
    Configuration* ptr_config = &config;

    // �� rootBlock
    // BLockTools blockTools = BLockTools(config.jsonPath);
    string strJsonPath = string(string(config.jsonPath));
    BLockTools blockTools = BLockTools(strJsonPath);
    BLock* ptr_rootBlock = blockTools.rootBlock;

    // Ԥ����
    Preprocessor preprocessor = Preprocessor(config, blockTools);
    preprocessor.preprocess();

    // database //�����������Ժ�����Ч�ʵĿ��������ｫTree��ʵ����װ��Tree::blossom�� //TODO: ��ֻ�Ǹ�ʾ����������װ��ʱ��Ӧ�ý�Tree::bloom�е�*(tree.database)ȡ����ֱ�����뵽Painter��ȥ�� //Tree��׼������̫�࣬���������ʵ��������Ƚ���׸
    Database database = Database{};
    Database* ptr_database = &database;
    Tree::bloom(ptr_database, ptr_config, ptr_rootBlock); //TODO: �Ƿ�ʡȥTree�ķ���ֵDatabase����Ϊ���ԣ�

    // // for-test //TODO: ɾ��������
    // DatabaseConverter dbCvter = DatabaseConverter();
    // dbCvter.database2jsonstr(database);
    // dbCvter.jsonstr2database(dbCvter.outputDatabase);
    // Database restoreDatabase = dbCvter.inputDatabase;

    // ɸѡ������
    Filter filter = Filter(config);
    filter.filterDatabase(database);

    // ����ɸѡ������
    Clipper clipper = Clipper(config);
    clipper.clipDatabase(filter.newDatabase);

    // �滭������
    // Painter painter = Painter(config, database);
    // Painter painter = Painter(config, filter.newDatabase);
    Painter painter = Painter(config, clipper.newDatabase);
    painter.paint(); //TODO: ��ֻ�Ǹ�ʾ����������װ��ʱ��Ӧ�ý�painter.screen����
    Mat& img = painter.screen;

    // ͳ��ʱ��
    // double time_cost = t_ogm.elapsed(); //��
    double time_cost = (double)(clock() - start) / CLOCKS_PER_SEC; //��
    cout << "[INFO] main done, " << time_cost << "s has elapsed" << endl; //TODO: װ������ôд������

    return 0;
}

