//
// Created by liuxingbo03 on 2021/11/21.
//

#ifndef ATUOCADDATAANALYSISPYCADCPP_FILTER_H
#define ATUOCADDATAANALYSISPYCADCPP_FILTER_H


#include "pycad/Structs.h"
#include "pycad/Configuration.h"
#include "pycad/Database.h"


/*
 * Elements采用的和提供的接口就是Criterion，这个Criterion来自于Structs.h，而这一接口又被Elements完全采用
 * 现在Filter需要针对这个接口做出适配，这种适配，需要结合Filter的方法、Filter需要实现的功能
 */

/*
 * 故可以认为，该类强烈依赖于Elements
 */


// 一个element的，某一项判据
typedef vector<string> ElemOpenCriterion; //这些判据，是用户可以在config文件中修改相应指控（Accusation）的，所以称之为“外部（Open）”

// OpenCriterion //一个element的，可以映射每一项的，判据，按照名称索引
typedef map<string, ElemOpenCriterion> OpenCriterion;

// CloseCriterion
class CloseCriterion {
public:
    // 成员属性
    VisibleHistory* visibleHistory = nullptr;
    IsDynHistory* isDynHistory = nullptr;
    // 成员方法
    CloseCriterion() = default;
    void clear() {
        // TODO: 可能的指针重置逻辑
        visibleHistory = nullptr;
        isDynHistory = nullptr;
    }
};

// 所有element的，可以映射所有项的，判据，按照名称索引
typedef vector<Criterion> Criterions;


class Filter {
private:
    // 成员属性
    Configuration* config = nullptr;
    map<string, bool> flagsOpen{};
    map<string, bool> flagsClose{};
    OpenCriterion openCriterion{}; //TODO: 考虑是否更名为OpenCriterionInheritFromElements
    CloseCriterion closeCriterion{};
    map<string, vector<string>> baseAttrFromElements;

    // 成员方法
    void resetOpen();
    void resetClose();

    void getOpenCriterionFromCriterion(Criterion &criterion);
    void getCloseCriterionFromCriterion(Criterion &criterion);

    static bool isInOneByOne(const char* ac, ElemOpenCriterion& ElemOpenCriterion);

    void writeFlagsChainCoreOpen(
            const char* accusationKey,
            Accusation& accusation,
            ElemOpenCriterion& elemcriterion
            );
    void writeFlagsChainCoreClose();

    void judgeOpen();
    void judgeClose();

    bool flagsResOpen();
    bool flagsResClose();

    bool pronounceOpen(Criterion& criterion); //开放判据
    bool pronounceClose(Criterion& criterion); //封闭判据
    bool pronounceWhitelistForType(const char* eType); //图元类型白名单判据
    static bool mergePronounce(
            bool& fResOpen,
            bool& fResClose,
            bool& fResWhitelistForType
            ); //合并各种判据宣判结果

    // 填写新数据库
    void writeNewDatabaseCore(
            BaseFieldInDatabase& newDatabase_field,
            BaseFieldInDatabase& database_field,
            const char* eType,
            Database& database
            );
    void writeNewDatabaseCore_lines(Database &database);
    void writeNewDatabaseCore_polylines(Database &database);
    void writeNewDatabaseCore_circles(Database &database);
    void writeNewDatabaseCore_arcs(Database &database);
    void writeNewDatabaseCore_ellipses(Database &database);
    void writeNewDatabaseCore_points(Database &database);
    void writeNewDatabaseCore_splines(Database &database);
    void writeNewDatabaseCore_texts(Database &database);
    void writeNewDatabaseCore_mtexts(Database &database);
    void writeNewDatabaseCore_attributes(Database &database);
    void writeNewDatabaseCore_alignedDimensions(Database &database);

public:
    // 成员属性
    Database newDatabase{}; //因为要保全原有的database，所以现在重写一个新的database；

    // 成员方法
    Filter();
    explicit Filter(Configuration& config);
    void filterDatabase(Database& database);
};


#endif //ATUOCADDATAANALYSISPYCADCPP_FILTER_H
