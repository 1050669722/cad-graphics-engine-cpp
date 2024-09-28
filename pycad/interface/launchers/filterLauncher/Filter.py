# -*- coding: utf-8 -*-
"""
---------------------------------------
File Name:  Filter
Description:
Author:     liuxingbo03
Date:       2021/12/01
---------------------------------------
"""

import json

from pycad.interface.Utils import *
from pycad.interface.converters.configConverter.Configuration import Configuration
from pycad.interface.converters.databaseConverter.Formalization import Formalization
from pycad.interface.launchers.parserLauncher.Parser import Parser
# from pycad.interface.launchers.painterLauncher.Painter import Painter
from pycad.interface.launchers.painterLauncher.PainterFromPython import PainterFromPython


class Filter(object):
    def __init__(self, config):
        # depoly
        copyToDeploy() #TODO: 调试专用，实装注释

        # config对象 转换为 jsonstr
        self.configJsonstrForPaths, self.configJsonstr = config.cvtToJsonstr()


    def filter(self, database):
        # 形式匹配
        formalization = Formalization(database)
        formalization.formalizePython2Cpp() #inplace modify

        # 获取 库文件对象
        libc = getLibc()

        # 获取 编码后的jsonstrs
        configJsonstrForPaths, configJsonstr, databaseJsonstr = encodeJsonstr(self.configJsonstrForPaths, self.configJsonstr, json.dumps(database)) #TODO: 这里的编码问题是否需要考虑 #", ensure_ascii=False"在此处不能写，这句话只能是为了路径而存在，因为下面由json.loads，而且编码方式需要与.yaml的config相同

        # 设置 输入类型 和 返回类型
        libc.filterEntrance.argtypes = [c_char_p, c_char_p, c_char_p]
        libc.filterEntrance.restype = c_char_p

        # 调用
        newDatabase = libc.filterEntrance(configJsonstrForPaths, configJsonstr, databaseJsonstr)
        newDatabase = json.loads(newDatabase)

        # 清除 堆内存 #注意，libc的返回值用完之后，才能执行这一行
        libc.freeHeapMemoryGeneratedByFilter()

        # 形式匹配
        formalization.database = newDatabase
        formalization.formalizeCpp2Python()

        # 形式匹配（恢复）
        formalization.database = database
        formalization.formalizeCpp2Python()

        return newDatabase




if __name__ == '__main__':
    yamlPath = "../../../../yaml/config.yaml"
    config = Configuration(yamlPath)

    parser = Parser(config)
    database = parser.parse()

    filter = Filter(config)
    newDatabase = filter.filter(database)

    # painter = Painter(config)
    # img = painter.paint(newDatabase)

    painter = PainterFromPython(config, newDatabase)
    painter.paint()

    pass
