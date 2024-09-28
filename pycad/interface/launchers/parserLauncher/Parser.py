# -*- coding: utf-8 -*-
"""
---------------------------------------
File Name:  Parser
Description:
Author:     liuxingbo03
Date:       2021/12/01
---------------------------------------
"""

import json
# import chardet

from pycad.interface.Utils import *
from pycad.interface.converters.configConverter.Configuration import Configuration
from pycad.interface.converters.databaseConverter.Formalization import Formalization
# from pycad.interface.launchers.painterLauncher.Painter import Painter
from pycad.interface.launchers.painterLauncher.PainterFromPython import PainterFromPython


class Parser(object):
    def __init__(self, config):
        # deploy
        copyToDeploy() #TODO: 调试专用，实装注释

        # config
        self.config = config

        # config对象 转换为 jsonstr
        self.configJsonstrForPaths, self.configJsonstr = config.cvtToJsonstr()

        # pklName
        self.getPklName()


    # TODO: 接收database的函数，或可在parse中实现


    def getPklName(self):
        # 获取.json文件名
        _, jsonName, _ = getPathNameExtFromPath(self.config.jsonPath)

        # 获取.pkl文件名
        self.pklName = os.path.join(self.config.cache, "database_" + jsonName)


    def pklExisted(self):
        # # 创建缓存目录
        # os.makedirs(self.config.cache, exist_ok=True)

        if not os.path.exists(self.config.cache):
            return False

        return self.pklName in [os.path.join(self.config.cache, os.path.splitext(filename)[0])
                                    for filename in os.listdir(self.config.cache)]


    @timeCounter
    def saveCache(self, database):
        # 创建缓存目录
        os.makedirs(self.config.cache, exist_ok=True)

        # 产生缓存文件
        savePkl(database, self.pklName)


    @timeCounter
    def loadCache(self):
        # 加载缓存文件
        return loadPkl(self.pklName)


    @timeCounter
    def parse(self):
        # for-test #TODO: 调试专用，实装注释
        while os.path.exists(self.pklName + ".pkl"):
            os.remove(self.pklName + ".pkl")

        # 如果没有缓存文件
        if not self.pklExisted():
            # 获取 库文件对象
            libc = getLibc()

            # 获取 编码后的jsonstrs
            configJsonstrForPaths, configJsonstr = encodeJsonstr(self.configJsonstrForPaths, self.configJsonstr)

            # for-test
            # print(chardet.detect(configJsonstr))
            # with open("./self_configJsonstr.txt", "w") as f:
            #     f.write(self.configJsonstr)
            # with open("./configJsonstr.txt", "w") as f:
            #     f.write(configJsonstr)
            # print(configJsonstr)
            # libc.parserEntrance.argtype = POINTER(c_ubyte)

            # 设置 输入类型 和 返回类型
            libc.parserEntrance.restype = c_char_p

            # 调用 获取database
            database = json.loads(libc.parserEntrance(configJsonstrForPaths, configJsonstr))

            # 清除 堆内存 #注意，libc的返回值用完之后，才能执行这一行
            libc.freeHeapMemoryGeneratedByParser()

            # 形式匹配
            formalization = Formalization(database) #inplace modify
            formalization.formalizeCpp2Python()

            # 产生缓存文件
            if self.config.isSaveCache:
                self.saveCache(database)

        # 如果已经存在缓存文件
        else:
            database = self.loadCache()

        return database




if __name__ == '__main__':
    yamlPath = "../../../../yaml/config.yaml"
    config = Configuration(yamlPath)

    parser = Parser(config)
    database = parser.parse()

    # painter = Painter(config)
    # img = painter.paint(database)

    painter = PainterFromPython(config, database)
    painter.paint()

    pass
