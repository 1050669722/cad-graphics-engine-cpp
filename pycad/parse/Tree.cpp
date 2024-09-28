//
// Created by liuxingbo03 on 2021/10/22.
//

#include <unistd.h>
// #include <boost/timer.hpp>
#include <ctime>

#include "pycad/Utils.h"
#include "pycad/parse/Tree.h"
#include "pycad/tools/TreeTools.h"


// using namespace boost;
using namespace std;


Tree::Tree() = default;


Tree::Tree(
            Configuration* config,
            BLock* block,
            vector<string>* pedigree,
            TransSolution* transSolution,
            MatrixXd* affineMat,
            Database* database,
            vector<TransSolution>* solutionsPaths,
            int* referSeq,
            int totalNum
            )
{
    // 配置
    this->config = config;

    // 树根block
    this->block = block;

    // 谱系
    this->pedigree = pedigree;

    // 引用变换方案
    this->transSolution = transSolution;

    // 仿射变换矩阵
    this->affineMat = affineMat;

    // 绘图数据库
    this->database = database;

    // 引用变换 形成的 路径
    this->solutionsPaths = solutionsPaths;

    // 相对于根节点的第几个引用
    this->referSeq = referSeq;

    // 节点总数
    totalNum == 0 ? this->totalNum = (*this->block).countBlocks(0) : this->totalNum = totalNum;
}


void Tree::assembleSolution(BlockReferenceInfoDict &solution) {
    /*
     * 装配引用变换方案
     * solution 新增引用变换方案
     */
    // record
    transSolution->records.push_back(solution.record);

    // 缩放
    transSolution->scaleFactors["x"].push_back(solution.scaleFactor["x"]);
    transSolution->scaleFactors["y"].push_back(solution.scaleFactor["y"]);
    transSolution->scaleFactors["z"].push_back(solution.scaleFactor["z"]);

    // 旋转
    transSolution->rotations.push_back(solution.rotation);

    // 位移
    transSolution->positions["x"].push_back(solution.position["x"]);
    transSolution->positions["y"].push_back(solution.position["y"]);
    transSolution->positions["z"].push_back(solution.position["z"]);

    // 法向量
    transSolution->normals["z"].push_back(solution.normal["z"]);

    // 图层
    transSolution->layers.push_back(solution.layer);

    // 动态块
    transSolution->isDyns.push_back(solution.isDynBlock);

    // solution本身的Handle
    transSolution->BRHandles.push_back(solution.handle);

    // visible
    transSolution->visibles.push_back(solution.visible);

    // spatialFilter
    transSolution->spatialFilter.push_back(solution.spatialFilter);

    // isFromHatchs
    transSolution->isFromHatchs.push_back(solution.isFromHatch);

    // pedigree
    pedigree->push_back(block->handle);

    // referSeq
    (*referSeq)++;

    // 仿射变换矩阵
    TreeTools::generateAffineMat(
                                *affineMat,
                                transSolution->scaleFactors,
                                transSolution->rotations,
                                transSolution->positions,
                                transSolution->normals
                                );
}


void Tree::disassembleSolution() {
    /*
     * 拆解引用变换方案
     * transST 原引用变换方案
     */
    // 缩放
    transSolution->scaleFactors["x"].pop_back();
    transSolution->scaleFactors["y"].pop_back();
    transSolution->scaleFactors["z"].pop_back();

    // 旋转
    transSolution->rotations.pop_back();

    // 位移
    transSolution->positions["x"].pop_back();
    transSolution->positions["y"].pop_back();
    transSolution->positions["z"].pop_back();

    // 法向量
    transSolution->normals["z"].pop_back();

    // 图层
    transSolution->layers.pop_back();

    // 动态块
    transSolution->isDyns.pop_back();

    // solution本身的Handle
    transSolution->BRHandles.pop_back();

    // visible
    transSolution->visibles.pop_back();

    // spatialFilter
    transSolution->spatialFilter.pop_back();

    // isFromHatchs
    transSolution->isFromHatchs.pop_back();

    // pedigree
    pedigree->pop_back();

    // referSeq
    // 本来就是要计数，所以没有referSeq的逻辑

    // 仿射变换矩阵
    // TreeTools::generateAffineMat内部含有复位TreeTools::resetAffineMat，故这里不用写额外的逻辑
}


void Tree::collectCoordsValsAndCoordsPtrsForEIns(
                                                vector<double>& coords_vals_x,
                                                vector<double>& coords_vals_y,
                                                vector<double*>& coords_ptrs_x,
                                                vector<double*>& coords_ptrs_y,
                                                BaseIns& eIns
                                               ) {
    /*
     * 搜集坐标及其地址
     * TODO: 提炼出这个函数后，为什么感觉变慢了？
     */
    for (auto& atom_x : eIns.coord_x) {
        for (auto& val : atom_x) {
            coords_vals_x.push_back(val);
            coords_ptrs_x.push_back(&val);
        }
    }
    for (auto& atom_y : eIns.coord_y) {
        for (auto& val : atom_y) {
            coords_vals_y.push_back(val);
            coords_ptrs_y.push_back(&val);
        }
    }
}


void Tree::calCoords() {
    /*
     * blockOrientedAffine
     * 针对这个BLock中所有坐标值的批量仿射变换
     * in-place
     * Enrich Database Step 1.计算坐标属性
     */
    // 所有图元的坐标集合 以及 所有图元的坐标指针集合
    vector<double> coords_vals_x, coords_vals_y;
    vector<double*> coords_ptrs_x, coords_ptrs_y;

    // // 预分配空间 //为什么这里预分配空间之后，好像变慢了？ //TODO: 是因为里面的东西很小，分配却很频繁吗?
    // coords_vals_x.reserve(1000), coords_vals_y.reserve(1000);
    // coords_ptrs_x.reserve(1000), coords_ptrs_y.reserve(1000);

    // *** 搜集坐标值和地址 ****************************************************
    // block->lineInses
    for (auto& eIns : block->lineInses) {
        collectCoordsValsAndCoordsPtrsForEIns(coords_vals_x, coords_vals_y, coords_ptrs_x, coords_ptrs_y, eIns);
    }

    // block->polylineInses
    for (auto& eIns : block->polylineInses) {
        collectCoordsValsAndCoordsPtrsForEIns(coords_vals_x, coords_vals_y, coords_ptrs_x, coords_ptrs_y, eIns);
        for (auto& atom_x : eIns.ori_coord_x) {
            for (auto& val : atom_x) {
                coords_vals_x.push_back(val);
                coords_ptrs_x.push_back(&val);
            }
        }
        for (auto& atom_y : eIns.ori_coord_y) {
            for (auto& val : atom_y) {
                coords_vals_y.push_back(val);
                coords_ptrs_y.push_back(&val);
            }
        }
    }

    // block->circleInses
    for (auto& eIns : block->circleInses) {
        collectCoordsValsAndCoordsPtrsForEIns(coords_vals_x, coords_vals_y, coords_ptrs_x, coords_ptrs_y, eIns);
    }

    // block->arcInses //TODO: 将重复的逻辑写成函数
    for (auto& eIns : block->arcInses) {
        collectCoordsValsAndCoordsPtrsForEIns(coords_vals_x, coords_vals_y, coords_ptrs_x, coords_ptrs_y, eIns);
    }

    // block->ellipseInses
    for (auto& eIns : block->ellipseInses) {
        collectCoordsValsAndCoordsPtrsForEIns(coords_vals_x, coords_vals_y, coords_ptrs_x, coords_ptrs_y, eIns);
    }

    // block->pointInses
    for (auto& eIns : block->pointInses) {
        collectCoordsValsAndCoordsPtrsForEIns(coords_vals_x, coords_vals_y, coords_ptrs_x, coords_ptrs_y, eIns);
    }

    // block->splineInses
    for (auto& eIns : block->splineInses) {
        collectCoordsValsAndCoordsPtrsForEIns(coords_vals_x, coords_vals_y, coords_ptrs_x, coords_ptrs_y, eIns);
    }

    // block->textInses
    for (auto& eIns : block->textInses) {
        collectCoordsValsAndCoordsPtrsForEIns(coords_vals_x, coords_vals_y, coords_ptrs_x, coords_ptrs_y, eIns);
    }

    // block->mtextInses
    for (auto& eIns : block->mtextInses) {
        collectCoordsValsAndCoordsPtrsForEIns(coords_vals_x, coords_vals_y, coords_ptrs_x, coords_ptrs_y, eIns);
    }

    // block->attributeInses
    for (auto& eIns : block->attributeInses) {
        collectCoordsValsAndCoordsPtrsForEIns(coords_vals_x, coords_vals_y, coords_ptrs_x, coords_ptrs_y, eIns);
    }

    // block->alignedDimensionInses
    for (auto& eIns : block->alignedDimensionInses) {
        collectCoordsValsAndCoordsPtrsForEIns(coords_vals_x, coords_vals_y, coords_ptrs_x, coords_ptrs_y, eIns);
    }
    // **********************************************************************

    // 仅一次仿射变换 //TODO: 由于atomOrientedAffine内部需要坐标值的传递，所以这里不得不这样写，所以下面不得不出现值拷贝，有没有办法修改atomOrientedAffine呢 //这需要对eigen比较熟悉
    TreeTools::blockOrientedAffine(coords_vals_x, coords_vals_y, *affineMat);

    // *** 依地址找回坐标值 ****************************************************
    for (int idx = 0; idx < coords_ptrs_x.size(); ++idx) {
        *(coords_ptrs_x[idx]) = coords_vals_x[idx];
        *(coords_ptrs_y[idx]) = coords_vals_y[idx];
    }
    // **********************************************************************

}


void Tree::calCoords(BaseIns &eIns) {
    /*
     * 某一类型 的 某一个元素 计算并输出坐标
     * eIns可以修改eIns自身的属性，但是不能修改传入参数的属性，尤其是Tree的属性 //TODO: 能否用const限制？
     */
    // Coord 针对的就是 一个eIns
    eIns.affine(
                affineMat,
                vector<string>{"coord_x", "coord_y"}
                );
}


void Tree::calParams(BaseIns &eIns) {
    /*
     * 某一类型 的 某一个元素 统计并输出判据
     * eIns可以修改eIns自身的属性，但是不能修改传入参数的属性，尤其是Tree的属性 //TODO: 能否用const限制？
     */
    eIns.report(
                transSolution->layers,
                transSolution->isDyns,
                transSolution->BRHandles,
                (*pedigree),
                block->handle,
                transSolution->visibles,//
                transSolution->isFromHatchs
                );
}


void Tree::wirteDatabaseForOneFieldInDatabase(BaseFieldInDatabase &database_field, BaseIns &eIns) {
    Utils::wirteDatabaseForOneFieldInDatabaseOneStep(
            database_field,
            eIns.coord_x, //坐标
            eIns.coord_y,
            eIns.color, //其它
            eIns.handle,
            eIns.criterion //判据
            );
}


void Tree::writeDatabase(const char* eType, BaseIns& eIns) {
    if (strcmp(eType, "lines") == 0) {
        wirteDatabaseForOneFieldInDatabase(database->lines, eIns);
    }
    else if (strcmp(eType, "polylines") == 0) {
        wirteDatabaseForOneFieldInDatabase(database->polylines, eIns);
    }
    else if (strcmp(eType, "circles") == 0) {
        wirteDatabaseForOneFieldInDatabase(database->circles, eIns);
    }
    else if (strcmp(eType, "arcs") == 0) {
        wirteDatabaseForOneFieldInDatabase(database->arcs, eIns);
    }
    else if (strcmp(eType, "ellipses") == 0) {
        wirteDatabaseForOneFieldInDatabase(database->ellipses, eIns);
    }
    else if (strcmp(eType, "points") == 0) {
        wirteDatabaseForOneFieldInDatabase(database->points, eIns);
    }
    else if (strcmp(eType, "splines") == 0) {
        wirteDatabaseForOneFieldInDatabase(database->splines, eIns);
    }
    else if (strcmp(eType, "texts") == 0) {
        wirteDatabaseForOneFieldInDatabase(database->texts, eIns);
    }
    else if (strcmp(eType, "mtexts") == 0) {
        wirteDatabaseForOneFieldInDatabase(database->mtexts, eIns);
    }
    else if (strcmp(eType, "attributes") == 0) {
        wirteDatabaseForOneFieldInDatabase(database->attributes, eIns);
    }
    else if (strcmp(eType, "alignedDimensions") == 0) {
        wirteDatabaseForOneFieldInDatabase(database->alignedDimensions, eIns);
    }
}


void Tree::enrichDataCore(const char* eType) {
    /*
     * ---富集数据---
     * 这里eIns的作用就是 丰富database的数据 | 完成之后就不再被使用了
     * 所以eIns被修改是可行的
     * 如果是这样，那么从一开始就建立一个包含了所有eIns的BLock有必要吗
     * TODO: 是否可以始终在内存中只保留一个eIns呢？ | 是否能提高时间效率呢？
     */
    // TODO: 如果没有动态特性，Tree::enrichDataCore甚至没有必要出现
    // TODO: assert eType in RenderingDatabase.elememtsTypes, "Expect eType: {} is one of elementsTypes: {}".format(eType, RenderingDatabase.elememtsTypes)
    if (strcmp(eType, "lines") == 0) {
        for (auto& eIns : block->lineInses) {
            // calCoords(eIns); // Enrich Database Step 1.计算坐标属性
            calParams(eIns); // Enrich Database Step 2.计算判据属性
            writeDatabase("lines", eIns); // Enrich Database Step 3.写数据库
        }
    }
    else if (strcmp(eType, "polylines") == 0) {
        for (auto& eIns : block->polylineInses) {
            // calCoords(eIns); // Enrich Database Step 1.计算坐标属性
            calParams(eIns); // Enrich Database Step 2.计算判据属性
            writeDatabase("polylines", eIns); // Enrich Database Step 3.写数据库
            database->polylines.ori_coords_x.push_back(eIns.ori_coord_x);
            database->polylines.ori_coords_y.push_back(eIns.ori_coord_y);
        }
    }
    else if (strcmp(eType, "circles") == 0) {
        for (auto& eIns : block->circleInses) {
            // calCoords(eIns); // Enrich Database Step 1.计算坐标属性
            calParams(eIns); // Enrich Database Step 2.计算判据属性
            writeDatabase("circles", eIns); // Enrich Database Step 3.写数据库
        }
    }
    else if (strcmp(eType, "arcs") == 0) {
        for (auto& eIns : block->arcInses) {
            // calCoords(eIns); // Enrich Database Step 1.计算坐标属性
            calParams(eIns); // Enrich Database Step 2.计算判据属性
            writeDatabase("arcs", eIns); // Enrich Database Step 3.写数据库
        }
    }
    else if (strcmp(eType, "ellipses") == 0) {
        for (auto& eIns : block->ellipseInses) {
            // calCoords(eIns); // Enrich Database Step 1.计算坐标属性
            calParams(eIns); // Enrich Database Step 2.计算判据属性
            writeDatabase("ellipses", eIns); // Enrich Database Step 3.写数据库
        }
    }
    else if (strcmp(eType, "points") == 0) {
        for (auto& eIns : block->pointInses) {
            // calCoords(eIns); // Enrich Database Step 1.计算坐标属性
            calParams(eIns); // Enrich Database Step 2.计算判据属性
            writeDatabase("points", eIns); // Enrich Database Step 3.写数据库
        }
    }
    else if (strcmp(eType, "splines") == 0) {
        for (auto& eIns : block->splineInses) {
            // calCoords(eIns); // Enrich Database Step 1.计算坐标属性
            calParams(eIns); // Enrich Database Step 2.计算判据属性
            writeDatabase("splines", eIns); // Enrich Database Step 3.写数据库
        }
    }
    else if (strcmp(eType, "texts") == 0) {
        for (auto& eIns : block->textInses) {
            // calCoords(eIns); // Enrich Database Step 1.计算坐标属性
            calParams(eIns); // Enrich Database Step 2.计算判据属性
            writeDatabase("texts", eIns); // Enrich Database Step 3.写数据库
            database->texts.rota.push_back(eIns.rotation + accumulate(transSolution->rotations.begin(), transSolution->rotations.end(), 0.0));
            database->texts.info.push_back(eIns.text);
            database->texts.widthFactor.push_back(eIns.widthFactor);
            database->texts.horizontalModes.push_back(eIns.horizontalMode);
            database->texts.verticalModes.push_back(eIns.verticalMode);
        }
    }
    else if (strcmp(eType, "mtexts") == 0) {
        for (auto& eIns : block->mtextInses) {
            // calCoords(eIns); // Enrich Database Step 1.计算坐标属性
            calParams(eIns); // Enrich Database Step 2.计算判据属性
            writeDatabase("mtexts", eIns); // Enrich Database Step 3.写数据库
            database->mtexts.rota.push_back(eIns.rotation + accumulate(transSolution->rotations.begin(), transSolution->rotations.end(), 0.0));
            database->mtexts.info.push_back(eIns.contents);
            database->mtexts.widthFactor.push_back(1.0);
            database->mtexts.lineNums.push_back(eIns.lineNum);
        }
    }
    else if (strcmp(eType, "attributes") == 0) {
        for (auto& eIns : block->attributeInses) {
            // calCoords(eIns); // Enrich Database Step 1.计算坐标属性
            calParams(eIns); // Enrich Database Step 2.计算判据属性
            writeDatabase("attributes", eIns); // Enrich Database Step 3.写数据库
            database->attributes.rota.push_back(eIns.rotation + accumulate(transSolution->rotations.begin(), transSolution->rotations.end(), 0.0));
            database->attributes.info.push_back(eIns.text);
            database->attributes.widthFactor.push_back(eIns.widthFactor);
            database->attributes.horizontalModes.push_back(eIns.horizontalMode);
            database->attributes.verticalModes.push_back(eIns.verticalMode);
        }
    }
    else if (strcmp(eType, "alignedDimensions") == 0) {
        for (auto& eIns : block->alignedDimensionInses) {
            // calCoords(eIns); // Enrich Database Step 1.计算坐标属性
            calParams(eIns); // Enrich Database Step 2.计算判据属性
            writeDatabase("alignedDimensions", eIns); // Enrich Database Step 3.写数据库
            database->alignedDimensions.measurements.push_back(eIns.DimensionIns::measurement);
        }
    }
    // TODO more else-if

}


void Tree::enrichDC_LineInses() {
    enrichDataCore("lines");
}


void Tree::enrichDC_PolylineInses() {
    enrichDataCore("polylines");
}


void Tree::enrichDC_CircleInses() {
    enrichDataCore("circles");
}


void Tree::enrichDC_ArcInses() {
    enrichDataCore("arcs");
}


void Tree::enrichDC_EllipseInses() {
    enrichDataCore("ellipses");
}


void Tree::enrichDC_PointInses() {
    enrichDataCore("points");
}


void Tree::enrichDC_SplineInses() {
    enrichDataCore("splines");
}


void Tree::enrichDC_TextInses() {
    enrichDataCore("texts");
}


void Tree::enrichDC_MtextInses() {
    enrichDataCore("mtexts");
}


void Tree::enrichDC_AttributeInses() {
    enrichDataCore("attributes");
}


void Tree::enrichDC_AlignedDimensionInses() {
    enrichDataCore("alignedDimensions");
}


void Tree::displayProcess() const {
    if ((*referSeq) == 1) {
        // cout << "[INFO] Tree Processing ..." << endl;
        cout << "[INFO] Tree Sprouting ..." << endl;
    }

    if (config->verbose) {
        // fflush(stdout); //好像没有起到作用 //TODO 试验，python调用时，再添加这一句，是否有用
        string formatStr = "[INFO] refer_block_seq: %04d, total: %04d, PROGRESS: %6.2f%%";
        formatStr = "\r" + formatStr; //确实能够回到行首
        (*referSeq) != totalNum ? formatStr += "" : formatStr += "\n";
        printf(formatStr.c_str(), (*referSeq), totalNum, (float)(*referSeq) / (float)totalNum * 100); //sleep(1);
    }

    if ((*referSeq) == totalNum) {
        cout << "[INFO] Tree Sprout Done" << endl;
    }
}


void Tree::enrichDC() {
    enrichDC_LineInses();
    enrichDC_PolylineInses();
    enrichDC_CircleInses();
    enrichDC_ArcInses();
    enrichDC_EllipseInses();
    enrichDC_PointInses();
    enrichDC_SplineInses();
    enrichDC_TextInses();
    enrichDC_MtextInses();
    enrichDC_AttributeInses();
    enrichDC_AlignedDimensionInses();
}


void Tree::sprout() { //in-place地改变了rootBlock
    // 计数 //进度显示 //采用displayProcess函数封装这些逻辑，能让sprout的职责更加单一
    displayProcess();

    // 坐标 // 修改这个block中的所有图元类型的所有eIns的坐标 //in-place
    calCoords();

    // 判据 和 写入数据集 // 逐 图元类型 富集 database {自己的元素}
    enrichDC();

    // solutionsPaths
    if (block->references.empty()) {
        solutionsPaths->push_back((*transSolution)); //TODO 怎样塞到assembleSolution中
    }

    // 递归 富集 database {引用的元素} //{引用图元导致的更新}
    for (int idx = 0; idx < block->references.size(); ++idx) {
        // 引用 和 引用方案
        BLock* reference = &(block->references[idx]);
        pair<string, BlockReferenceInfoDict>& record_solution = block->referenceTransformSolutions[idx];
        BlockReferenceInfoDict& solution = record_solution.second;
        string& record = record_solution.first;
        assert(reference->handle == record);

        // [积累] 引用变换方案
        assembleSolution(solution);

        // 对于本轮reference
        Tree(
            config,
            reference,
            pedigree,
            transSolution,
            affineMat,
            database,
            solutionsPaths,
            referSeq,
            totalNum
            ).sprout();

        // [恢复] 引用变换参数
        disassembleSolution();
    }

}


void Tree::bloom(Database* ptr_database, Configuration* ptr_config, BLock* ptr_rootBlock) {
    // 统计时间
    // timer t_ogm;
    clock_t start = clock();

    // 打印提示信息 //TODO 应该做成装饰模式
    cout << "[INFO] bloom ..." << endl;

    // *** 准备参数 *****************************************************************************
    // pedigree
    vector<string> _pedigree = vector<string>{};
    _pedigree.reserve(100);
    vector<string>* ptr_pedigree = &_pedigree;

    // transSolution
    TransSolution _transSolution = TransSolution{};
    _transSolution.scaleFactors = map<string, vector<double>>{
            pair<string, vector<double>>{"x", vector<double>{1.0}},
            pair<string, vector<double>>{"y", vector<double>{1.0}},
            pair<string, vector<double>>{"z", vector<double>{1.0}}
    };
    _transSolution.rotations = vector<double>{0.0};
    _transSolution.positions = map<string, vector<double>>{
            pair<string, vector<double>>{"x", vector<double>{0.0}},
            pair<string, vector<double>>{"y", vector<double>{0.0}},
            pair<string, vector<double>>{"z", vector<double>{0.0}}
    };
    _transSolution.normals = map<string, vector<double>>{
            pair<string, vector<double>>{"x", vector<double>{0.0}},
            pair<string, vector<double>>{"y", vector<double>{0.0}},
            pair<string, vector<double>>{"z", vector<double>{1.0}}
    };
    TransSolution* ptr_transSolution = &_transSolution;

    // affineMat
    MatrixXd _affineMat = MatrixXd::Identity(3, 3); //这里是[单位阵]很重要
    MatrixXd* ptr_affineMat = &_affineMat;

    // database
    // Database _database = Database{};
    // Database* ptr_database = &_database;

    // solutionsPaths
    vector<TransSolution> _solutionsPaths = vector<TransSolution>{};
    _solutionsPaths.reserve(100);
    vector<TransSolution>* ptr_solutionsPaths = &_solutionsPaths;

    // referSeq
    int referSeq = 1;
    int* ptr_referSeq = &referSeq;
    // ****************************************************************************************

    // *** 实例化 ******************************************************************************
    Tree tree = Tree(
                        ptr_config,
                        ptr_rootBlock,
                        ptr_pedigree,
                        ptr_transSolution,
                        ptr_affineMat,
                        ptr_database,
                        ptr_solutionsPaths,
                        ptr_referSeq
                    );
    // ****************************************************************************************

    // *** 递归填充database *********************************************************************
    tree.sprout();
    // ****************************************************************************************

    // 统计时间
    // double time_cost = t_ogm.elapsed(); //秒
    double time_cost = (double)(clock() - start) / CLOCKS_PER_SEC; //秒
    cout << "[INFO] bloom done, " << time_cost << "s has elapsed" << endl; //TODO: 装饰器怎么写出来？

    // return _database;
}
