# -*- coding: utf-8 -*-
"""
---------------------------------------
File Name:  Formalization
Description:
Author:     liuxingbo03
Date:       2021/12/14
---------------------------------------
"""

from pycad.interface.Utils import timeCounter


class Formalization(object):
    """
    此类用于
    将
    Cpp版本中的database
    与
    Python版本中的database
    的格式
    进行相互转换
    因为，Cpp版本中的database将一些属性塞进了criterions字段中
    """
    def __init__(self, database):
        self.database = database #TODO: 自动识别这个database来自于哪一层


    def isThisDatabaseFromCpp(self):
        """
        判断self.database是否来自Cpp层，一种可能的方式
        """
        return not ("layers" in self.database["lines"])


    def fieldCpp2Python(self, fieldCpp, fieldPython):
        for key, value in self.database.items():
            self.database[key][fieldPython] = [] #TODO: 新建这个字段
            # if not ("criterions" in value):
            #     continue #TODO: 由于尚未开发完成，有的图元中的criterions字段的值为空，进而没有这个字段
            for criterion in value["criterions"]:
                self.database[key][fieldPython].append(criterion[fieldCpp])
                criterion[fieldPython] = criterion[fieldCpp]
                criterion.pop(fieldCpp)


    @timeCounter
    def formalizeCpp2Python(self):
        if not self.isThisDatabaseFromCpp():
            print("[WARNING] This database is not from Cpp, formalization has been shut down")
            return self.database
        self.fieldCpp2Python("layerHistory", "layers")
        self.fieldCpp2Python("isDynHistory", "isDyns")
        self.fieldCpp2Python("belongToWhichBlock", "belongToWhichBlocks")
        self.fieldCpp2Python("BRHandleHistory", "BRHandles")
        self.fieldCpp2Python("pedigreeLogger", "blockHandlesPedigree")
        self.fieldCpp2Python("visibleHistory", "visibles")
        self.fieldCpp2Python("isFromHatchHistory", "isFromHatchs")
        return self.database


    def isThisDatabaseFromPython(self):
        """
        判断self.database是否来自Python层，一种可能的方式
        """
        return "layers" in self.database["lines"]


    def fieldPython2Cpp(self, fieldPython, fieldCpp):
        for key, value in self.database.items():
            value.pop(fieldPython)
            for criterion in value["criterions"]:
                criterion[fieldCpp] = criterion[fieldPython]
                criterion.pop(fieldPython)


    @timeCounter
    def formalizePython2Cpp(self):
        if not self.isThisDatabaseFromPython():
            print("[WARNING] This database is not from Python, formalization has been shut down")
            return self.database
        self.fieldPython2Cpp("layers", "layerHistory")
        self.fieldPython2Cpp("isDyns", "isDynHistory")
        self.fieldPython2Cpp("belongToWhichBlocks", "belongToWhichBlock")
        self.fieldPython2Cpp("BRHandles", "BRHandleHistory")
        self.fieldPython2Cpp("blockHandlesPedigree", "pedigreeLogger")
        self.fieldPython2Cpp("visibles", "visibleHistory")
        self.fieldPython2Cpp("isFromHatchs", "isFromHatchHistory")
        return self.database
