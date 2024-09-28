# 设置CMAKE最低版本
cmake_minimum_required(VERSION 3.19) #除非部署，暂定3.19版本
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_USE_CXX11_ABI=0") //TODO: 这个命令是什么意思？

# 设置编译版本
set(CMAKE_CXX_STANDARD 14)

# 项目名称
set(PROJECT_NAME AtuoCADDataAnalysisPyCADCpp)

# 建立项目
project(${PROJECT_NAME})

# 设置头文件路径
set(INC_DIR ./include)
set(EIGEN_DIR ./include/eigen-3.4.0/eigen-3.4.0)
set(OpenCV_DIR D:/Programs/OpenCV/3.4.15/opencv/build_20240929_mingw64)
#set(OpenCV_DIR /home/liu/Programs/opencv-3.4.15/build) #sudo make install之后，相关的东西就在这里了：/usr，编译器会在这里寻找

# 寻找OpenCV库
find_package(OpenCV REQUIRED)

# 设置库文件路径
set(LINK_DIR ./lib)

# 引入头文件
include_directories(
        ${INC_DIR}
        ${EIGEN_DIR}
        ${OpenCV_INCLUDE_DIRS}
)

# 引入库文件
link_directories(${LINK_DIR})

## 产生动态链接库文件
#file(
#        GLOB
#        src
#        ./*.cpp pycad/*.cpp pycad/*/*.cpp pycad/*/*/*.cpp pycad/*/*/*/*.cpp #TODO: 随着目录改变而改变，有没有更通用的办法？
##        OpenCV_DIR/*
##        INC_DIR/boost/*
##        INC_DIR/eigen-3.4.0/*
##        INC_DIR/nlohmann/*.hpp
##        INC_DIR/pycad/*.h INC_DIR/pycad/*/*.h INC_DIR/pycad/*/*/*.h INC_DIR/*.h
##        INC_DIR/rapidjson/*.h INC_DIR/rapidjson/*/*.h
#)
#set(all_srcs ${src})
#add_library(${PROJECT_NAME} SHARED ${all_srcs})
##install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${LINK_DIR}) #TODO: 这个命令是什么意思？

# 产生可执行文件
add_executable(
        ${PROJECT_NAME}

        pycad/Configuration.cpp
        pycad/Database.cpp
        pycad/Utils.cpp

        pycad/preprocess/Preprocessor.cpp

        pycad/sketcher/ArcSketcher.cpp
        pycad/sketcher/PolylineSketcher.cpp
        pycad/sketcher/EllipseSketcher.cpp
        pycad/sketcher/SplineSketcher.cpp
        pycad/sketcher/AlignedDimensionSketcher.cpp

        pycad/tools/JsonTools.cpp
        pycad/tools/SketcherTools.cpp
        pycad/tools/ElementTools.cpp
        pycad/tools/BLockTools.cpp
        pycad/tools/TreeTools.cpp
        pycad/tools/PainterTools.cpp

        pycad/parse/Elements.cpp
        pycad/parse/BLock.cpp
        pycad/parse/Tree.cpp

        pycad/filter/Filter.cpp

        pycad/clip/Clipper.cpp
        pycad/clip/SutherlandHodgman.cpp
        pycad/clip/WeilerAtherton.cpp

        pycad/plot/Painter.cpp

        pycad/interface/converters/configConverter/ConfigConverter.cpp
        pycad/interface/converters/databaseConverter/DatabaseConverter.cpp
        pycad/interface/converters/coordConverter/CoordConverter.cpp

        pycad/interface/converters/infoDictsConverter/BaseFieldsInJsonInfoDictAndElementInsesConverter.cpp
        pycad/interface/converters/infoDictsConverter/PolylineInfoDictConverter.cpp
        pycad/interface/converters/infoDictsConverter/SplineInfoDictConverter.cpp
        pycad/interface/converters/infoDictsConverter/ArcInfoDictConverter.cpp
        pycad/interface/converters/infoDictsConverter/CircleInfoDictConverter.cpp
        pycad/interface/converters/infoDictsConverter/EllipseInfoDictConverter.cpp

        pycad/interface/converters/elementsConverter/BaseInsConverter.cpp
        pycad/interface/converters/elementsConverter/PolylineInsConverter.cpp
        pycad/interface/converters/elementsConverter/SplineInsConverter.cpp
        pycad/interface/converters/elementsConverter/ArcInsConverter.cpp
        pycad/interface/converters/elementsConverter/CircleInsConverter.cpp
        pycad/interface/converters/elementsConverter/EllipseInsConverter.cpp

        pycad/interface/launchers/parserLauncher/ParserEntrance.cpp
        pycad/interface/launchers/filterLauncher/FilterEntrance.cpp
        pycad/interface/launchers/clipperLauncher/ClipperEntrance.cpp
        pycad/interface/launchers/painterLauncher/PainterEntrance.cpp
        pycad/interface/launchers/elementsLauncher/PolylineEntrance.cpp
        pycad/interface/launchers/elementsLauncher/SplineEntrance.cpp
        pycad/interface/launchers/elementsLauncher/ArcEntrance.cpp
        pycad/interface/launchers/elementsLauncher/CircleEntrance.cpp
        pycad/interface/launchers/elementsLauncher/EllipseEntrance.cpp

        entrance.cpp
)

# 添加链接库 #sudo make install之后，相关的东西就在这里了：/usr，编译器会在这里寻找
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
