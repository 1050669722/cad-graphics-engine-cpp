//
// Created by liuxingbo03 on 2022/1/6.
//

#include "pycad/interface/converters/elementsConverter/BaseInsConverter.h"


template class BaseInsConverter<PolylineIns>;
template class BaseInsConverter<SplineIns>;
template class BaseInsConverter<ArcIns>;
template class BaseInsConverter<CircleIns>;
template class BaseInsConverter<EllipseIns>;


template<class T>
BaseInsConverter<T>::BaseInsConverter() = default;


template<class T>
BaseInsConverter<T>::~BaseInsConverter() {
    delete docOutput;
}


template<class T>
void BaseInsConverter<T>::initDocOutput() {
    // 实例化指针所指向的对象
    docOutput = new Document();
    // 设置docOutput为一个空的Object
    docOutput->SetObject();
    // 分配器指针
    allocator = &(docOutput->GetAllocator());
}


template<class T>
void BaseInsConverter<T>::sendAtom(Value &dstArrayAtom, Atom &srcAtom) {
    for (double& v : srcAtom) {
        dstArrayAtom.PushBack(v, *allocator);
    }
}


template<class T>
void BaseInsConverter<T>::sendCoord(Value &dstArrayCoord, Coord &srcCoord) {
    for (Atom& atom : srcCoord) {
        Value arrayAtom(kArrayType);
        sendAtom(arrayAtom, atom);
        dstArrayCoord.PushBack(arrayAtom, *allocator);
    }
}


template<class T>
void BaseInsConverter<T>::mntCoord(const char *fieldName, Coord &srcCoord) {
    /*
     * 为 elementIns 安装 coord字段 和 coord值
     */
    Value arrayCoord(kArrayType);
    sendCoord(arrayCoord, srcCoord);
    Value str;
    str.SetString(fieldName, *allocator);
    docOutput->AddMember(str, arrayCoord, *allocator);
}


template<class T>
void BaseInsConverter<T>::baseIns2Jsonstr(T &elementIns) {
    // 实例化Document对象
    initDocOutput();

    // coord_x
    mntCoord("coord_x", elementIns.coord_x);

    // coord_y
    mntCoord("coord_y", elementIns.coord_y);

    // 形成jsonstr
    StringBuffer s;
    Writer<StringBuffer> writer(s);
    docOutput->Accept(writer);
    outputElementIns = string(s.GetString());
}
