# -*- coding: utf-8 -*-
"""
---------------------------------------
File Name:  Configuration
Description:
Author:     liuxingbo03
Date:       2021/11/30
---------------------------------------
"""

import os, sys
import json
import yaml
import datetime
import platform, getpass

if sys.version_info < (3, 8):
    from collections import Iterable
else:
    from collections.abc import Iterable

from pycad.interface.Utils import *


class Configuration(object): #TODO: 这个名字原先是Config
    def __init__(self, yamlPath):
        """
        初始化 Configuration类
        """
        # 配置文件路径
        self.yamlPath = yamlPath

        # 验证后缀名
        _, _, ext = getPathNameExtFromPath(self.yamlPath)
        assert ext in ['.yml', '.yaml'], "only support yaml files for now"

        # 读取配置文件
        self._load()

        # 设置缺省配置
        self._default()

        # # 将配置设置为属性
        # self._setattr()


    def __call__(self, *args, **kwargs):
        """
        重载 __call__方法
        """
        return self.__dict__


    def _load(self):
        """
        从.yaml或.yml加载配置
        """
        with open(self.yamlPath, mode="r", encoding="utf-8") as f:
            cont = f.read()
        data = yaml.load(cont, Loader=yaml.FullLoader)
        self.__dict__.update(data)


    def _default(self):
        """
        设置缺省配置
        """
        if not self.__dict__["saveFileName"]: #None 或者 ""
            self.__dict__["saveFileName"] = datetime.datetime.now().strftime("%Y-%m-%d_%H-%M-%S") + ".png"

        if not self.__dict__["fontfile"]: #sys.executable
            # 自动选择 .ttc文件 的路径
            goalPath = ""
            candidatePaths = [
                                # os.path.join(os.path.split(os.path.realpath(__file__))[0], "files"), #'D:\\cad_draw_engine_cpp\\cad-graphics-engine-cpp\\AtuoCADDataAnalysisPyCADCpp\\pycad\\interface\\configConverter\\files'
                                os.path.join(os.path.split(os.path.realpath(__file__))[0], "../../../files"), #'D:\\cad_draw_engine_cpp\\cad-graphics-engine-cpp\\AtuoCADDataAnalysisPyCADCpp\\pycad\\files'
                                os.path.join(os.path.split(sys.executable)[0], "pycad", "files"), #'D:\\Anaconda3\\pycad\\files' #base
                                os.path.join(os.path.split(sys.executable)[0], "../Lib/site-packages/pycad/files"), #/home/liu/anaconda3/envs/paddle/Lib/site-packages/pycad/files #envs中的某个环境
                                os.path.join(os.path.split(os.path.split(sys.executable)[0])[0], "pycad", "files"), #'D:\\pycad\\files'
                                "./usr/local/bin/pycad/files",
                              ]
            for candidatePath in candidatePaths:
                if os.path.exists(candidatePath):
                    if "simsun.ttc" in os.listdir(candidatePath):
                        goalPath = os.path.join(candidatePath, "simsun.ttc")
                        break
            if goalPath == "":
                raise ValueError("[ERROR] .ttc can not be found")
            self.__dict__["fontfile"] = goalPath

        if not self.__dict__["featurefile"]: #TODO: 这五行应该与上面一样做修改
            try:
                self.__dict__["featurefile"] = os.path.abspath(os.path.join(os.path.split(os.path.realpath(__file__))[0], "files/feature.csv")) #'D:\\cad_draw_engine_cpp\\cad-graphics-engine-cpp\\AtuoCADDataAnalysisPyCADCpp\\pycad\\interface\\configConverter\\files\\feature.csv'
            except:
                self.__dict__["featurefile"] = os.path.join(os.path.split(sys.executable)[0], 'pycad/files', 'feature.csv') #'D:\\Anaconda3\\pycad/files\\feature.csv'

        if not self.__dict__["savePath"]: #TODO: 应该添加其它平台的情况
            if platform.system() == "Windows":
                self.__dict__["savePath"] = r"C:\Users\Administrator\Desktop\figs"
            elif platform.system() == "Linux":
                self.__dict__["savePath"] = "/home/" + getpass.getuser() + "/figs"
            else:
                raise ValueError("Unknown Platform: {}".format(platform.system()))

        # 全局修改restoreScaleFactor #保证在Pinter之前不会用到restoreScaleFactor #保证在Pinter之外不会用到restoreScaleFactor
        # if "resotreResolution" in self.data and self.data["resotreResolution"]:
        #     self.data["restoreScaleFactor"] = "[Error] For this situation, config.resotreResolution takes effect"
        if not self.__dict__["resotreResolution"]:
            self.__dict__["resotreResolution"] = 0.0

        if not self.__dict__["cache"]:
            self.__dict__["cache"] = "./cache"
        if not os.path.isabs(self.__dict__["cache"]):
            self.__dict__["cache"] = os.path.join(getPathNameExtFromPath(self.yamlPath)[0], self.__dict__["cache"])


    def _isJsonPathEmpty(self):
        if (not hasattr(self, "jsonPath")) or (self.__dict__["jsonPath"] == "") or (self.__dict__["jsonPath"] is None):
            return True
        return False


    def _update(self):
        # if not hasattr(self, "judgements"):
        #     self.judgements = {"layers": {}}
        #     self.judgements["layers"]["blackList"] = []
        pass


    def __setattr__(self, name, value):
        """
        重载 __setattr__方法
        """
        self.__dict__[name] = value #这个__dict__是object.__setattr__(self, name, value)默认具有的，不可删除
        if name == "jsonPath" and (not self._isJsonPathEmpty()): #条件：name == "jsonPath" and hasattr(self, "data") and "jsonPath" in self.data
            self._update()


    # def _setattr(self):
    #     """
    #     将配置设置为属性
    #     """
    #     for k, v in self.__dict__.items():
    #         setattr(self, k, v)


    def cvtToJsonstr(self):
        """
        将 python中的Configuration类 转换为 jsonstr
        :return: 转换后的json字符串（非unicode编码，unicode编码） #前者用于获取configjsonstr中的路径，后者用于获取configjsonstrzhong中的其它中文字符（对应于原先json文件的编码解码）
        """
        return json.dumps(self(), ensure_ascii=False), json.dumps(self()) #, ensure_ascii=False #TODO: 如果这里写了ensure_ascii=False，Filter功能就不对了，可能说明图纸json的ensure_ascii=True #而且rapidjson读取图纸json的编码与ConfigConverter一致




if __name__ == '__main__':
    config = Configuration("./config.yaml")
    jsonstr = config.cvtToJsonstr()
    with open('../../../../json/config.json', 'w') as jsonfile:
        jsonfile.write(jsonstr)
    pass
