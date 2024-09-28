//
// Created by nan on 2021/11/17.
//

#ifndef ATUOCADDATAANALYSISPYCADCPP_CLIPPER_H
#define ATUOCADDATAANALYSISPYCADCPP_CLIPPER_H


#include "pycad/Configuration.h"
#include "pycad/Database.h"
#include "pycad/clip/ClipStructs.h"
#include "pycad/clip/SutherlandHodgman.h"
#include "pycad/clip/WeilerAtherton.h"

using namespace std;


class Clipper {
private:
    // 成员属性
    Configuration* config{};
    SutherlandHodgman sutherlandHodgman{};
    WeilerAtherton weilerAtherton{};

    // 成员方法
    void generateNewDatabase(Database& database);
    void clipGeneratedNewDatabaseCore_WeilerAtherton(BaseFieldInDatabase &newDatabase_field);
    void clipGeneratedNewDatabaseCore_SutherlandHodgman(BaseFieldInDatabase &newDatabase_field);
    void clipGeneratedNewDatabaseCore_textAnalogue_SutherlandHodgman(BaseFieldInDatabase &newDatabase_field);

public:
    // 成员属性
    Database newDatabase{}; //因为要保全原有的database，所以现在重写一个新的database；

    // 成员方法
    Clipper();
    explicit Clipper(Configuration &config); //, Database &database);
    void clipDatabase(Database &database);
};


#endif //ATUOCADDATAANALYSISPYCADCPP_CLIPPER_H
