//
// Created by liuxingbo03 on 2021/11/21.
//

/*
 * *** 过滤选项优先级：白名单 > 黑名单 > wanted ***
 */

// #include <boost/timer.hpp>
#include <ctime>

#include "pycad/filter/Filter.h"
#include "pycad/Utils.h"
#include "pycad/parse/Elements.h"


// using namespace boost;
using namespace std;


Filter::Filter() = default;


Filter::Filter(Configuration &config) {
    // 配置类实例
    this->config = &config;

    // 初始化 开放判据结果标识 //每一道审判 的 判定结果 存储在这里
    resetOpen();

    // 初始化 封闭判据结果标识 //
    resetClose();

    // 从Elements中获取基本判据属性 //用于归并判断结果
    baseAttrFromElements = BaseAttrCriterion::outputBaseAttrCriterion();
}


void Filter::resetOpen() {
    /*
     * 复位flags 默认值都是指向[不画]的
     */
    for (auto& judgement : config->judgements) {
        flagsOpen[judgement.first] = false;
    }
}


void Filter::resetClose() {
    /*
     * 复位flags 默认值都是指向[不画]的
     */
    flagsClose["visibleHistory"] = false;
    flagsClose["isDynHistory"] = true; //TODO: 目前isDynLast的策略都是不画，尚未进一步探究其中奥秘
}


void Filter::getOpenCriterionFromCriterion(Criterion& criterion) {
    /*
     * 根据Structs.h中提供的接口
     * typedef vector<string> LayerHistory; //layerHistory
     * typedef vector<string> BLockReferenceHandleHistory; //BRHandleHistory
     * typedef vector<string> PedigreeLogger; //pedigreeLogger
     * 或者说，根据Elements.h中提供的接口
     * pair<string, vector<string>>("coordAttr", vector<string>({"coord_x", "coord_y"})),
     * pair<string, vector<string>>("openCriAttr", vector<string>({"color", "layerHistory", "isDynHistory", "belongToWhichBlock", "BRHandleHistory", "pedigreeLogger"})), //"color", "layers", "isDyns", "belongToWhichBlocks", "BRHandles", "blockHandlesPedigree"
     * pair<string, vector<string>>("closeCriAttr", vector<string>({"visibleHistory", "isFromHatchHistory"})), //"visibles", "isFromHatchs"
     * pair<string, vector<string>>("extraAttr", vector<string>({"handle"}))
     * 获取OpenCriterion
     * OpenCriterion包含于openCriAttr，但并不等于；将openCriAttr留待扩展时参考；
     * 相当于OpenCriterion继承自openCriAttr；前者属于Filter，后者属于Elements；
     */
    openCriterion.clear(); //TODO: 这里应该写一个class OpenCriterion，因为可以提高效率，也可以让代码逻辑统一
    openCriterion.insert(pair<string, ElemOpenCriterion>("layerHistory", criterion.layerHistory));
    openCriterion.insert(pair<string, ElemOpenCriterion>("BRHandleHistory", criterion.BRHandleHistory));
    openCriterion.insert(pair<string, ElemOpenCriterion>("pedigreeLogger", criterion.pedigreeLogger));
}


void Filter::getCloseCriterionFromCriterion(Criterion &criterion) {
    /*
     * 根据Structs.h中提供的接口
     * 或者说，根据Elements.h中提供的接口
     * 获取CloseCriterion
     */
    closeCriterion.clear();
    closeCriterion.visibleHistory = &(criterion.visibleHistory);
    closeCriterion.isDynHistory = &(criterion.isDynHistory);
}


bool Filter::isInOneByOne(const char *ac, ElemOpenCriterion &elemOpenCriterion) {
    // TODO: 考虑any_of()替换;
    for (auto& eec : elemOpenCriterion) {
        if (eec.find(ac) != -1) {
            return true;
        }
    }
    return false;
}


void Filter::writeFlagsChainCoreOpen(
        const char *accusationKey,
        Accusation &accusation,
        ElemOpenCriterion &elemOpenCriterion
        ) {
    // wanted
    for (auto& ac : accusation["wanted"]) {
        if (!ac.empty() && isInOneByOne(ac.c_str(), elemOpenCriterion)) {
            flagsOpen[accusationKey] = true;
            break;
        }
        else if (ac.empty()) {
            if (config->judgementsInterOrUnion == "Inter") { //TODO: 适配大小写
                flagsOpen[accusationKey] = true;
                break;
            }
            else if (config->judgementsInterOrUnion == "Union") { //TODO: 适配大小写
                flagsOpen[accusationKey] = false;
                continue;
            }
            else {
                throw runtime_error("[ERROR] Ivalid Parameter \"judgementsInterOrUnion\"");
            }
        }
    }

    // blackList
    for (auto& ac : accusation["blackList"]) {
        if (!ac.empty() && isInOneByOne(ac.c_str(), elemOpenCriterion)) {
            flagsOpen[accusationKey] = false;
        }
    }
}


void Filter::judgeOpen() {
    /*
     * 筛选链
     * 依据每一道审判 为 标志链flags 的 每一个键 赋值
     * criterion 一个element的，可以映射所有项的，判据，按照名称索引
     * elemcriterion 一个element的，某一项判据
     */
    for (auto& judgement : config->judgements) {
        // 判据 ElemOpenCriterion
        ElemOpenCriterion elemOpenCriterion = openCriterion[judgement.first];

        // 指控 accusation
        Accusation accusation = judgement.second;

        // 对于 每一个 accusationKey = judgement.first
        writeFlagsChainCoreOpen(
                judgement.first.c_str(),
                accusation,
                elemOpenCriterion
                );
    }
}


void Filter::judgeClose() {
    /*
     * 筛选链
     */
    writeFlagsChainCoreClose();
}


bool Filter::flagsResOpen() {
    /*
     * 根据标志链，判定标志链结果
     * 交集：仅当 标志链 中的值全为True的时候，才写入数据库
     * 并集：只要 标志链 中的值至少有一个为True的时候，就写入数据库
     */
    /*
     * 最后判定结果的归并，需要根据Elements中规定的开放属性、非开放属性等来进行
     * 这里的“开放属性（Open Attribute）”是包含了“外部判据（External Criterion）”的
     */
    // 开放属性判据结果容器，非开放属性判据结果容器
    deque<bool> openCriAttrRes; //不能使用vector<bool>？
    bool open_cri_attr_res;

    // 对于 标识链 中的每一个 判据名 和 判据结果 //开放
    for (auto& flag : flagsOpen) {
        string attrCriName = flag.first;
        bool attrCriRes = flag.second;
        if (find(baseAttrFromElements["openCriAttr"].begin(), baseAttrFromElements["openCriAttr"].end(), attrCriName) != baseAttrFromElements["openCriAttr"].end()) {
            openCriAttrRes.push_back(attrCriRes);
        }
        else {
            cout << "[WARNING] The \"attrCriName\" must be open" << endl;
        }
    }

    // 开放判据的综合结果 依据 config.judgementsInterOrUnion 来判断
    if (config->judgementsInterOrUnion == "Inter") {
        open_cri_attr_res = find(openCriAttrRes.begin(), openCriAttrRes.end(), false) == openCriAttrRes.end();
    }
    else if (config->judgementsInterOrUnion == "Union") {
        open_cri_attr_res = find(openCriAttrRes.begin(), openCriAttrRes.end(), true) != openCriAttrRes.end();
    }

    // 返回
    return open_cri_attr_res;
}


bool Filter::flagsResClose() {
    /*
     * 根据标志链，判定标志链结果
     * 交集：仅当 标志链 中的值全为True的时候，才写入数据库
     * 并集：只要 标志链 中的值至少有一个为True的时候，就写入数据库
     */
    /*
     * 最后判定结果的归并，需要根据Elements中规定的开放属性、非开放属性等来进行
     * 这里的“开放属性（Open Attribute）”是包含了“外部判据（External Criterion）”的
     */
    // 开放属性判据结果容器，非开放属性判据结果容器
    deque<bool> closeCriAttrRes; //不能使用vector<bool>？
    bool close_attr_cri_res;

    // 对于 标识链 中的每一个 判据名 和 判据结果 //开放
    for (auto& flag : flagsClose) {
        string attrCriName = flag.first;
        bool attrCriRes = flag.second;
        if (find(baseAttrFromElements["closeCriAttr"].begin(), baseAttrFromElements["closeCriAttr"].end(), attrCriName) != baseAttrFromElements["closeCriAttr"].end()) {
            closeCriAttrRes.push_back(attrCriRes);
        }
        else {
            cout << "[WARNING] The \"attrCriName\" must be close" << endl;
        }
    }

    // 非开放标志链的综合结果 一定是依据 且 得出
    close_attr_cri_res = find(closeCriAttrRes.begin(), closeCriAttrRes.end(), false) == closeCriAttrRes.end();

    // 返回
    return close_attr_cri_res;
}


bool Filter::pronounceOpen(Criterion &criterion) {
    /*
     * 宣判
     * criterion 一个element的，可以映射所有项的，判据，按照名称索引
     */
    // 获取OpenCriterion
    getOpenCriterionFromCriterion(criterion);

    // 筛选链 给出 标志链
    // 修改flagsOpen
    judgeOpen();

    // 根据 标志链 判定 标志链结果 //记录这一个元素的审判标志链结果
    // 根据flagsOpen给出判定结果
    bool fResOpen = flagsResOpen();

    // 标志链复位
    resetOpen();

    return fResOpen;
}


bool Filter::pronounceClose(Criterion &criterion) {
    /*
     * 宣判
     */
    // 获取CloseCriterion
    getCloseCriterionFromCriterion(criterion);

    // 筛选链 给出 标志链
    // 修改flagsClose
    judgeClose();

    // 根据 标志链 判定 标志链结果 //记录这一个元素的审判标志链结果
    // 根据flagsClose给出判定结果
    bool fResClose = flagsResClose();

    // 标志链复位
    resetClose();

    return fResClose;
}


bool Filter::pronounceWhitelistForType(const char* eType) {
    return isInOneByOne(eType, config->whitelistForType);
}


bool Filter::mergePronounce(
        bool& fResOpen,
        bool& fResClose,
        bool& fResWhitelistForType
        ) {
    return (fResOpen && fResClose) || fResWhitelistForType;
}


void Filter::writeNewDatabaseCore(
        BaseFieldInDatabase &newDatabase_field,
        BaseFieldInDatabase &database_field,
        const char *eType,
        Database &database
        ) {
    for (int idx = 0; idx < database_field.criterions.size(); ++idx) {
        /*
         * 当前图元类型的第idx个图元
         */
        // 开放判据结果 //第idx个图元的 判据 //一个element的，可以映射每一项的，判据，按照名称索引 //判据经过Filter的审判
        bool fResOpen = pronounceOpen(database_field.criterions[idx]);
        // 封闭判据结果
        bool fResClose = pronounceClose(database_field.criterions[idx]);
        // 图元类型白名单判据结果
        bool fResWhitelistForType = pronounceWhitelistForType(eType);
        // 合并各种判据宣判结果
        bool fRes = mergePronounce(fResOpen, fResClose, fResWhitelistForType);

        // 非特殊情况
        if (fRes) {
            Utils::wirteDatabaseForOneFieldInDatabaseOneStep(
                    newDatabase_field,
                    database_field.coords_x[idx],
                    database_field.coords_y[idx],
                    database_field.colors[idx],
                    database_field.handles[idx],
                    database_field.criterions[idx]
                    );
        }

        // 特殊情况
        if (fRes && (strcmp(eType, "texts") == 0)) {
            newDatabase.texts.rota.push_back(database.texts.rota[idx]);
            newDatabase.texts.info.push_back(database.texts.info[idx]);
            newDatabase.texts.widthFactor.push_back(database.texts.widthFactor[idx]);
            newDatabase.texts.horizontalModes.push_back(database.texts.horizontalModes[idx]);
            newDatabase.texts.verticalModes.push_back(database.texts.verticalModes[idx]);
        }
        if (fRes && (strcmp(eType, "mtexts") == 0)) {
            newDatabase.mtexts.rota.push_back(database.mtexts.rota[idx]);
            newDatabase.mtexts.info.push_back(database.mtexts.info[idx]);
            newDatabase.mtexts.widthFactor.push_back(database.mtexts.widthFactor[idx]);
            newDatabase.mtexts.lineNums.push_back(database.mtexts.lineNums[idx]);
        }
        if (fRes && (strcmp(eType, "attributes") == 0)) {
            newDatabase.attributes.rota.push_back(database.attributes.rota[idx]);
            newDatabase.attributes.info.push_back(database.attributes.info[idx]);
            newDatabase.attributes.widthFactor.push_back(database.attributes.widthFactor[idx]);
            newDatabase.attributes.horizontalModes.push_back(database.attributes.horizontalModes[idx]);
            newDatabase.attributes.verticalModes.push_back(database.attributes.verticalModes[idx]);
        }

        if (fRes && (strcmp(eType, "alignedDimensions") == 0)) {
            newDatabase.alignedDimensions.measurements.push_back(database.alignedDimensions.measurements[idx]);
        }
    }
}


void Filter::writeFlagsChainCoreClose() {
    flagsClose["visibleHistory"] = find(closeCriterion.visibleHistory->begin(), closeCriterion.visibleHistory->end(), false) == closeCriterion.visibleHistory->end();
    flagsClose["isDynHistory"] = true; //动态块的可见性已经描述在visible中了，所以这里的动态块判定都通过就可以了 //find(closeCriterion.isDynHistory->begin(), closeCriterion.isDynHistory->end(), true) == closeCriterion.isDynHistory->end(); //!(closeCriterion.isDynHistory->back()); //注释掉的这个做法应该不合适
}


void Filter::writeNewDatabaseCore_lines(Database &database) {
    writeNewDatabaseCore(newDatabase.lines, database.lines, "lines", database);
}


void Filter::writeNewDatabaseCore_polylines(Database &database) {
    writeNewDatabaseCore(newDatabase.polylines, database.polylines, "polylines", database);

    // 特殊图元类型 的 特殊字段
    for (int idx = 0; idx < database.polylines.criterions.size(); ++idx) {
        newDatabase.polylines.ori_coords_x.push_back(database.polylines.ori_coords_x[idx]);
        newDatabase.polylines.ori_coords_y.push_back(database.polylines.ori_coords_y[idx]);
    }
}


void Filter::writeNewDatabaseCore_circles(Database &database) {
    writeNewDatabaseCore(newDatabase.circles, database.circles, "circles", database);
}


void Filter::writeNewDatabaseCore_arcs(Database &database) {
    writeNewDatabaseCore(newDatabase.arcs, database.arcs, "arcs", database);
}


void Filter::writeNewDatabaseCore_ellipses(Database &database) {
    writeNewDatabaseCore(newDatabase.ellipses, database.ellipses, "ellipses", database);
}


void Filter::writeNewDatabaseCore_points(Database &database) {
    writeNewDatabaseCore(newDatabase.points, database.points, "points", database);
}


void Filter::writeNewDatabaseCore_splines(Database &database) {
    writeNewDatabaseCore(newDatabase.splines, database.splines, "splines", database);
}


void Filter::writeNewDatabaseCore_texts(Database &database) {
    writeNewDatabaseCore(newDatabase.texts, database.texts, "texts", database);
}


void Filter::writeNewDatabaseCore_mtexts(Database &database) {
    writeNewDatabaseCore(newDatabase.mtexts, database.mtexts, "mtexts", database);
}


void Filter::writeNewDatabaseCore_attributes(Database &database) {
    writeNewDatabaseCore(newDatabase.attributes, database.attributes, "attributes", database);
}


void Filter::writeNewDatabaseCore_alignedDimensions(Database &database) {
    writeNewDatabaseCore(newDatabase.alignedDimensions, database.alignedDimensions, "alignedDimensions", database);
}


void Filter::filterDatabase(Database &database) {
    /*
     * 针对数据集的筛选
     * database 解析后的 全量数据集
     * 返回 筛选后的 newDatabase
     * 类内的 新建数据集 属性
     */
    // 统计时间
    // timer t_ogm;
    clock_t start = clock();

    // 打印提示信息 //TODO 应该做成装饰模式
    cout << "[INFO] filterDatabase ..." << endl;

    // 遍历 每一个图元类型 //TODO: 用函数封装一些逻辑
    // lines
    writeNewDatabaseCore_lines(database);
    // polylines
    writeNewDatabaseCore_polylines(database);
    // circles
    writeNewDatabaseCore_circles(database);
    // arcs
    writeNewDatabaseCore_arcs(database);
    // ellipses
    writeNewDatabaseCore_ellipses(database);
    // points
    writeNewDatabaseCore_points(database);
    // splines
    writeNewDatabaseCore_splines(database);
    // texts
    writeNewDatabaseCore_texts(database);
    // mtexts
    writeNewDatabaseCore_mtexts(database);
    // attributes
    writeNewDatabaseCore_attributes(database);
    // alignedDimensions
    writeNewDatabaseCore_alignedDimensions(database);

    // 统计时间
    // double time_cost = t_ogm.elapsed(); //秒
    double time_cost = (double)(clock() - start) / CLOCKS_PER_SEC; //秒
    cout << "[INFO] filterDatabase done, " << time_cost << "s has elapsed" << endl; //TODO: 装饰器怎么写出来？
}
