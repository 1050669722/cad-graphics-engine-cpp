# -*- coding: utf-8 -*-
"""
---------------------------------------
File Name:  Painter
Description:
Author:     liuxingbo03
Date:       2021/12/01
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


class Painter(object):
    def __init__(self, config):
        # depoly
        copyToDeploy() #TODO: 调试专用，实装注释

        # config对象 转换为 jsonstr
        self.configJsonstrForPaths, self.configJsonstr = config.cvtToJsonstr()


    def paint(self, database): #TODO: 注意，开发模块中的类似物Painter.paint()是无参的，database是放在Painter的实例化函数中的
        # 形式匹配
        formalization = Formalization(database)
        formalization.formalizePython2Cpp() #inplace modify

        # 获取 库文件对象
        libc = getLibc()

        # 获取 编码后的jsonstrs
        configJsonstrForPaths, configJsonstr, databaseJsonstr = encodeJsonstr(self.configJsonstrForPaths, self.configJsonstr, json.dumps(database))

        # 设置 输入类型 和 返回类型
        libc.painterEntrance.argtypes = [c_char_p, c_char_p, c_char_p]
        libc.painterEntrance.restype = POINTER(c_ubyte)
        libc.getOutputImgHeight.restype = c_int
        libc.getOutputImgWidth.restype = c_int

        # 调用
        img = libc.painterEntrance(configJsonstrForPaths, configJsonstr, databaseJsonstr)
        height = libc.getOutputImgHeight()
        width = libc.getOutputImgWidth()

        # 图像形状整理 #TODO: 这一步好像比较慢？图像高宽：4200x5940，时间：8.677576299999998s，这一函数占据时间最多：np.fromiter()
        img = np.array(np.fromiter(img, np.uint8, count=height * width * 3)).reshape((height, width, 3))

        # 清除 堆内存 #注意，libc的返回值用完之后，才能执行这一行
        libc.freeHeapMemoryGeneratedByPainter()

        # 形式匹配（恢复）
        formalization.database = database
        formalization.formalizeCpp2Python() #inplace modify

        return img




if __name__ == '__main__':
    yamlPath = "../../../../yaml/config.yaml"
    config = Configuration(yamlPath)

    parser = Parser(config)
    database = parser.parse()

    filter = Filter(config)
    newDatabase = filter.filter(database)

    painter = Painter(config)
    img = painter.paint(newDatabase)

    # cvShow(img)

    pass
