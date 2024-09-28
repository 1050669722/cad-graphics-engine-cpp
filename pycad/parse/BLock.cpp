//
// Created by liuxingbo03 on 2021/10/22.
//

#include <iostream>
//#include <boost/timer.hpp>

#include "pycad/parse/BLock.h"


using namespace std;
//using namespace boost;


BLock::BLock() = default;


BLock::BLock(BlockInfoDict &blockInfoDict) {
    // 属性赋值
    // timer t_ogm5;
    id = blockInfoDict.id;
    handle = blockInfoDict.handle;
    name = blockInfoDict.name;
    blockScaling = blockInfoDict.blockScaling;
    // origin = blockInfoDict.origin;
    ptr_origin = &(blockInfoDict.origin);
    // entityInfoDicts = blockInfoDict.entityInfoDicts;
    ptr_entityInfoDicts = &(blockInfoDict.entityInfoDicts);
    // lineInfoDicts = blockInfoDict.lineInfoDicts;
    ptr_lineInfoDicts = &(blockInfoDict.lineInfoDicts);
    // polylineInfoDicts = blockInfoDict.polylineInfoDicts;
    ptr_polylineInfoDicts = &(blockInfoDict.polylineInfoDicts);
    // arcInfoDicts = blockInfoDict.arcInfoDicts;
    ptr_arcInfoDicts = &(blockInfoDict.arcInfoDicts);
    // circleInfoDicts = blockInfoDict.circleInfoDicts;
    ptr_circleInfoDicts = &(blockInfoDict.circleInfoDicts);
    // ellipseInfoDicts = blockInfoDict.ellipseInfoDicts;
    ptr_ellipseInfoDicts = &(blockInfoDict.ellipseInfoDicts);
    // pointInfoDicts = blockInfoDict.pointInfoDicts;
    ptr_pointInfoDicts = &(blockInfoDict.pointInfoDicts);
    // splineInfoDicts = blockInfoDict.splineInfoDicts;
    ptr_splineInfoDicts = &(blockInfoDict.splineInfoDicts);
    // textInfoDicts = blockInfoDict.textInfoDicts;
    ptr_textInfoDicts = &(blockInfoDict.textInfoDicts);
    // mtextInfoDicts = blockInfoDict.mtextInfoDicts;
    ptr_mtextInfoDicts = &(blockInfoDict.mtextInfoDicts);
    // hatchInfoDicts = blockInfoDict.hatchInfoDicts;
    ptr_hatchInfoDicts = &(blockInfoDict.hatchInfoDicts);
    // blockReferenceInfoDicts = blockInfoDict.blockReferenceInfoDicts;
    ptr_blockReferenceInfoDicts = &(blockInfoDict.blockReferenceInfoDicts);
    // alignedDimensionInfoDicts = blockInfoDict.alignedDimensionInfoDicts;
    ptr_alignedDimensionInfoDicts = &(blockInfoDict.alignedDimensionInfoDicts);

    // TODO 搜集这些字段的数据结构
    // rotatedDimensionInfoDicts = blockInfoDict.rotatedDimensionInfoDicts;
    ptr_rotatedDimensionInfoDicts = &(blockInfoDict.rotatedDimensionInfoDicts);
    // lineAngularDimensionInfoDicts = blockInfoDict.lineAngularDimensionInfoDicts;
    ptr_lineAngularDimensionInfoDicts = &(blockInfoDict.lineAngularDimensionInfoDicts);
    // arcDimensionInfoDicts = blockInfoDict.arcDimensionInfoDicts;
    ptr_arcDimensionInfoDicts = &(blockInfoDict.arcDimensionInfoDicts);
    // radialDimensionInfoDicts = blockInfoDict.radialDimensionInfoDicts;
    ptr_radialDimensionInfoDicts = &(blockInfoDict.radialDimensionInfoDicts);
    // diametricDimensionInfoDicts = blockInfoDict.diametricDimensionInfoDicts;
    ptr_diametricDimensionInfoDicts = &(blockInfoDict.diametricDimensionInfoDicts);
    // radialDimensionLargeInfoDicts = blockInfoDict.radialDimensionLargeInfoDicts;
    ptr_radialDimensionLargeInfoDicts = &(blockInfoDict.radialDimensionLargeInfoDicts);
    // ordinateDimensionInfoDicts = blockInfoDict.ordinateDimensionInfoDicts;
    ptr_ordinateDimensionInfoDicts = &(blockInfoDict.ordinateDimensionInfoDicts);
    // mLeaderInfoDicts = blockInfoDict.mLeaderInfoDicts;
    ptr_mLeaderInfoDicts = &(blockInfoDict.mLeaderInfoDicts);
    // cout << "t_ogm5: " << t_ogm5.elapsed() << endl;

    // 搜集引用变换方案 TransSolution
    // timer t_ogm6;
    howToReference();
    // cout << "t_ogm6: " <<  t_ogm6.elapsed() << endl;

    // 实例化元素 //block自身包含的元素
    // timer t_ogm7;
    instantiateElements();
    // cout << "t_ogm7: " <<  t_ogm7.elapsed() << endl;
}


void BLock::howToReference() {
    for (auto& blockReferenceInfoDict : (*ptr_blockReferenceInfoDicts)) {
        referenceTransformSolutions.emplace_back(blockReferenceInfoDict.blockTableRecord, blockReferenceInfoDict);
    }
}


void BLock::instantiateElements() {
    // TODO 工厂模式 //如果工厂模式可以实现，应该以工厂模式实现
    // TODO: 这里似乎可以多线程，每个线程负责一种图元类型的实例化

    // 实例化所有的lines //lines -> linesIns //line -> lineIns
    for (auto& lineInfoDict : (*ptr_lineInfoDicts)) {
        LineIns lineIns = LineIns(lineInfoDict, handle.c_str(), (*ptr_origin));
        lineInses.push_back(lineIns);
    }

    // 实例化所有的polylines //polylines -> polylinesIns //polyline -> polyline_ELE
    for (auto& polylineInfoDict : (*ptr_polylineInfoDicts)) {
        PolylineIns polylineIns = PolylineIns(polylineInfoDict, handle.c_str(), (*ptr_origin));
        polylineInses.push_back(polylineIns);
    }

    // 实例化所有的circles //circles -> circlesIns //circle -> circleIns
    for (auto& circleInfoDict : (*ptr_circleInfoDicts)) {
        CircleIns circleIns = CircleIns(circleInfoDict, handle.c_str(), (*ptr_origin));
        circleInses.push_back(circleIns);
    }

    // 实例化所有的arcs //arcs -> arcsIns //arc -> arcIns
    for (auto& arcInfoDict : (*ptr_arcInfoDicts)) {
        ArcIns arcIns = ArcIns(arcInfoDict, handle.c_str(), (*ptr_origin));
        arcInses.push_back(arcIns);
    }

    // 实例化所有的ellipses //ellipses -> ellipsesIns //ellipse -> ellipseIns
    for (auto& ellipseInfoDict : (*ptr_ellipseInfoDicts)) {
        EllipseIns ellipseIns = EllipseIns(ellipseInfoDict, handle.c_str(), (*ptr_origin));
        ellipseInses.push_back(ellipseIns);
    }

    // 实例化所有的points //points -> pointsIns //point -> pointIns
    for (auto& pointInfoDict : (*ptr_pointInfoDicts)) {
        PointIns pointIns = PointIns(pointInfoDict, handle.c_str(), (*ptr_origin));
        pointInses.push_back(pointIns);
    }

    // 实例化所有的splines //splines -> splinesIns //spline -> splineIns
    for (auto& splineInfoDict : (*ptr_splineInfoDicts)) {
        SplineIns splineIns = SplineIns(splineInfoDict, handle.c_str(), (*ptr_origin));
        splineInses.push_back(splineIns);
    }

    // 实例化所有的texts //texts -> textsIns //text -> textIns
    for (auto& textInfoDict : (*ptr_textInfoDicts)) {
        TextIns textIns = TextIns(textInfoDict, handle.c_str(), (*ptr_origin));
        textInses.push_back(textIns);
    }

    // 实例化所有的mtexts //mtexts -> mtextsIns //mtext -> mtextIns
    for (auto& mtextInfoDict: (*ptr_mtextInfoDicts)) {
        MtextIns mtextIns = MtextIns(mtextInfoDict, handle.c_str(), (*ptr_origin));
        mtextInses.push_back(mtextIns);
    }

    // 实例化所有的attributes //attributes -> attributesIns //attribute -> attributeIns
    // attribute位于blockReference中
    for (auto& blockReference : (*ptr_blockReferenceInfoDicts)) {
        for (auto& attributeInfoDict : blockReference.attributes) {
            AttributeIns attributeIns = AttributeIns(attributeInfoDict, handle.c_str(), (*ptr_origin));
            attributeInses.push_back(attributeIns);
        }
    }

    // 实例化所有的alignedDimensions //alignedDimensions -> alignedDimensionsIns //alignedDimension -> alignedDimensionIns
    for (auto& alignedDimensionInfoDict : (*ptr_alignedDimensionInfoDicts)) {
        AlignedDimensionIns alignedDimensionIns = AlignedDimensionIns(alignedDimensionInfoDict, handle.c_str(), (*ptr_origin));
        alignedDimensionInses.push_back(alignedDimensionIns);
    }
}


void BLock::appendReferenceBlock(BLock& block) {
    references.push_back(block);
}


int BLock::countBlocks(int num) {
    // 自己这一节点
    num++;

    // 递归统计
    for (auto& reference : references) {
        num = reference.countBlocks(num);
    }

    // 返回
    return num;
}


// int main() {
//
//     BLock block = BLock();
//
//     return 0;
// }
