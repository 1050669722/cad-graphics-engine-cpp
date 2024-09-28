# -*- coding: utf-8 -*-
"""
---------------------------------------
File Name:  Polyline
Description:
Author:     liuxingbo03
Date:       2022/01/04
---------------------------------------
"""

import json
# import time

import numpy as np

from pycad.interface.Utils import *
from pycad.interface.converters.configConverter.Configuration import Configuration
from pycad.interface.converters.databaseConverter.Formalization import Formalization
from pycad.interface.launchers.parserLauncher.Parser import Parser
from pycad.interface.launchers.filterLauncher.Filter import Filter


class Polyline(object):
    def __init__(self, polylineInfoDict):
        # depoly
        copyToDeploy() #TODO: 调试专用，实装注释

        # polylineInfoDict对象 转换为 jsonstr
        self.polylineInfoDictJsonstr = json.dumps(polylineInfoDict) #, ensure_ascii=False

        # 实例化
        self._instantiate()


    def _instantiate(self):
        # 获取 库文件对象
        libc = getLibc()

        # 获取 编码后的jsonstrs
        polylineInfoDictJsonstr = encodeJsonstr(self.polylineInfoDictJsonstr)

        # 设置 输入类型 和 返回类型
        libc.polylineEntrance.argtype = c_char_p
        libc.polylineEntrance.restype = c_char_p

        # 调用 获取elementIns #TODO: 真正的elementIns的实例不会从这里以jsonstr的形式被loads出来，这里loads出来的还是字典
        elementIns = json.loads(libc.polylineEntrance(polylineInfoDictJsonstr))

        # 清除 堆内存 #注意，libc的返回值用完之后，才能执行这一行
        libc.freeHeapMemoryGeneratedByPolyline()

        # 重整属性 #TODO: 所以在这里将elementIns中的信息重整为python中Polyline类的属性
        self.coord_x = elementIns["coord_x"]
        self.coord_y = elementIns["coord_y"]




if __name__ == '__main__':
    # jsonPath
    jsonPath = "../../../../json/15.集团版YJ160-11(T2-17X)_t3.json"

    # get infoDict
    try:
        with open(jsonPath, mode='r') as f: #, encoding="utf-8"
            infos = json.load(f)
    except:
        try:
            with open(jsonPath, mode="rb") as f:
                infos = json.load(f)
        except: #windows下打不开json的问题，在ubuntu中没有遇到，可能是哪里的编码方式设置得不对，可能是系统的，也可能是PyCharm的
            f = open(jsonPath, encoding="utf-8")
            infos = json.load(f)
            f.close()
    polylineInfoDict = infos["blocks"][0]["polylines"][0]

    # get coord
    polyline = Polyline(polylineInfoDict)
    # print(polyline.coord_x)
    # print(polyline.coord_y)
    pass
