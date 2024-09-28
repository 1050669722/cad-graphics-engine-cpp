# -*- coding: utf-8 -*-
"""
---------------------------------------
File Name:  Utils
Description:
Author:     liuxingbo03
Date:       2021/11/30
---------------------------------------
"""

import os
import cv2
import sys
import shutil
import colorsys
import time
import pickle
import platform
import numpy as np
# import cv2

from PIL import Image, ImageDraw, ImageFont
from typing import List
from ctypes import *

if sys.version_info < (3, 8):
    from collections import Iterable
else:
    from collections.abc import Iterable


os.environ["PYGAME_HIDE_SUPPORT_PROMPT"] = "yes"


import pygame


# def assignScale(ori_func):
#     def new_func(name, img, scale=1.0):
#         img = cv2.resize(img, (int(scale * img.shape[1]), int(scale * img.shape[0])))
#         ori_func(name, img)
#     return new_func


def timeCounter(old_func):
    def new_func(*args, **kwargs): #*args, **kwargs为了传递原来函数的参数
        time0 = time.perf_counter()
        results = old_func(*args, **kwargs)
        # print("[INFO] {:.2f} seconds elapsed".format(time.perf_counter() - time0))
        print("[INFO] {} done, {:.15f} seconds elapsed".format(old_func.__name__, time.perf_counter() - time0))
        # print("[INFO] {} in {}".format(old_func.__name__, os.getcwd()))
        return results

    return new_func


def runOnlyOnce(old_func):
    """
     确保一次运行中，old_func仅被调用一次
    """
    runCount = [0] #此行仅会在首次运行时执行，因为涉及装饰器定义
    def new_func(*args, **kargs):
        runCount[0] += 1
        if runCount[0] <= 1:
            results = old_func(*args, **kargs)
            return results
    return new_func


# @assignScale
# def cvShow(name, img):
#     cv2.imshow(name, img)
#     cv2.waitKey(0)
#     cv2.destroyAllWindows()


def getPathNameExtFromPath(path):
    """
    从文件的路径中 获取 文件所在路径 文件名 扩展名
    path: 文件路径
    return: 文件所在路径 文件名 扩展名
    """
    # 分离 扩展名
    filename, extname = os.path.splitext(path)[0], os.path.splitext(path)[1]
    # 分离 文件名
    filepath, filename = os.path.split(filename)[:-1], os.path.split(filename)[-1]
    if isinstance(filepath, Iterable):
        filepath = filepath[0]
    # 返回 文件所在路径 文件名 扩展名
    return filepath, filename, extname


@runOnlyOnce
def copyToDeploy():
    """
    将动态链接库文件复制到项目中的指定位置
    TODO: 仅在开发测试时采用这一函数，实装用不到这一函数；该函数中的逻辑会降低运行效率；
    """
    if platform.system() == "Windows":
        fileName = "libAtuoCADDataAnalysisPyCADCpp.dll"
    else:
        fileName = "libAtuoCADDataAnalysisPyCADCpp.so"

    srcPath = os.path.join(getPathNameExtFromPath(os.path.realpath(__file__))[0], "../../cmake-build-debug")
    srcFile = os.path.join(srcPath, fileName)

    dstPath = os.path.join(getPathNameExtFromPath(os.path.realpath(__file__))[0], "../deploy")
    dstFile = os.path.join(dstPath, fileName)

    while os.path.exists(dstFile):
        os.remove(dstFile)

    if (not os.path.exists(srcPath)) or (not os.path.exists(dstPath)):
        return
    elif (not os.path.exists(srcFile)) or (os.path.exists(dstFile)):
        return
    else:
        shutil.copy(srcFile, dstFile)


def savePkl(obj, name, mode="wb"):
    """
    保存
    :param obj: 对象
    :param name: 名字
    :param mode: 打开方式，默认"wb"
    :return: None
    """
    if not name.endswith(".pkl"):
        name += ".pkl"
    with open(name, mode) as f:
        pickle.dump(obj, f, pickle.HIGHEST_PROTOCOL)
    return None


def loadPkl(name):
    """
    读取
    :param name: 名字
    :return: 对象
    """
    if not name.endswith(".pkl"):
        name += ".pkl"
    with open(name, "rb") as f:
        return pickle.load(f)


def getLibc():
    # 当前脚本路径
    filePath, _, _ = getPathNameExtFromPath(os.path.realpath(__file__)) #TODO: 只要该路径不被高层调用和拼接，这里写realpath是可以的
    # filePath = "." #此处，相对路径比较好，因为这一脚本未来不会处于高层，所以不应该用它自己的绝对路径，TODO: 应该用该脚本自己的相对路径

    # 依据平台类型，以不同方式编码config
    if platform.system() == "Windows":
        # dllPath = os.path.join(filePath, "..", "deploy", "libAtuoCADDataAnalysisPyCADCpp.dll") #"..",
        # dllPath = os.path.join(os.path.split(sys.executable)[0], "../Lib/site-packages/pycad/deploy", "libAtuoCADDataAnalysisPyCADCpp.dll") #"..",
        dllPath = "libAtuoCADDataAnalysisPyCADCpp.dll"
        dllDirCandidates = [
            os.path.join(filePath, "..", "deploy"),
            os.path.join(os.path.split(sys.executable)[0], "../Lib/site-packages/pycad/deploy"),
        ]
        for dllDirCandidate in dllDirCandidates: #TODO: 这样的candidate逻辑或许可以与Configuration.py的candidate逻辑合并
            if os.path.exists(dllDirCandidate):
                if dllPath in os.listdir(dllDirCandidate):
                    dllPath = os.path.join(dllDirCandidate, dllPath)
                    break
        libc = cdll.LoadLibrary(dllPath)
    else:
        # soPath = os.path.join(filePath, "..", "deploy", "libAtuoCADDataAnalysisPyCADCpp.so") #"..",
        # soPath = os.path.join(os.path.split(sys.executable)[0], "../Lib/site-packages/pycad/deploy", "libAtuoCADDataAnalysisPyCADCpp.so")
        soPath = "libAtuoCADDataAnalysisPyCADCpp.so"
        soDirCandidates = [
            os.path.join(filePath, "..", "deploy"),
            os.path.join(os.path.split(sys.executable)[0], "../Lib/site-packages/pycad/deploy"),
        ]
        for soDirCandidate in soDirCandidates:
            if os.path.exists(soDirCandidate):
                if soPath in os.listdir(soDirCandidate):
                    soPath = os.path.join(soDirCandidate, soPath)
                    break
        libc = cdll.LoadLibrary(soPath)

    return libc


def encodeJsonstr(*jsonstrs):
    # 依据平台类型，以不同方式编码config
    jsonstrs = list(jsonstrs)
    for idx, jsonstr in enumerate(jsonstrs):
        if platform.system() == "Windows":
            jsonstrs[idx] = jsonstr.encode("gbk")
        else:
            jsonstrs[idx] = jsonstr.encode("utf-8")

    if len(jsonstrs) == 1:
        return jsonstrs[0]
    return jsonstrs


def RGB2BGR(img):
    return img[:, :, ::-1]


def screen2Array(screen):
    """
    将
    pygame的screen类 实例
    转换为
    numpy.ndarray类 实例
    """
    pil_string_image = pygame.image.tostring(screen, "RGB", False)  # 将当前图像输出为字符变量
    pli_image = Image.frombytes('RGB', screen.get_size(), pil_string_image, 'raw')  # 将字符变量变为Image变量
    img = np.array(pli_image)  # 转变为numpy
    # small = pli_image.resize((100, 100), resample=Image.BOX)  # 改变尺寸
    # small.show()
    return img


def Array2Image(img):
    return Image.fromarray(cv2.cvtColor(img, cv2.COLOR_BGR2RGB))

def Image2Array(img):
    return cv2.cvtColor(np.asarray(img), cv2.COLOR_RGB2BGR)


def getJsonsFromPath(jsonsPath):
    """
    从指定目录中获取所有json文件的路径
    输入: jsonsPath, 包含json文件的目录
    返回: 列表，该目录中所有json文件的路径
    TODO: one-size-fits-all -> 自适应获取指定目录中的所有json文件 #根据单一职责原则，这里不添加筛选条件功能 #强内聚，松耦合
    """
    jsonsList = []
    for root, dirs, files in os.walk(jsonsPath): #one-size-fits-all
        for file in files:
            if not file.endswith(".json"):
                continue
            filepath = os.path.join(root, file)
            jsonsList.append(filepath)
    return jsonsList


def PILBBox(kinds: List[str], pic, category_idx: int, bbox: List[int]):
    """
    在指定数组格式的图片中，画框，写类别
    :kinds: 所有待检测目标的类别 #List[str]
    :param pic: 数组格式的图片 #np.ndarray
    :param category_idx: 类别索引 #从1开始 #int #是不是应该考虑不从1开始，而是从0开始
    :param bbox: 框坐标 #x1, y1, x2, y2 #List[int]
    :return: 画完框，写好类别的，数组格式的图片 #np.ndarray
    """
    # 缺陷类别
    defectKinds = kinds #["boaderDefect", "cornerDefect", "whiteSpot", "grayBlobDefect", "deepSpot", "irisDefect", "marker", "scratch"]

    # 颜色
    hsv_tuples = [(x / len(defectKinds), 1., 1.) for x in range(len(defectKinds))]
    colors = list(map(lambda x: colorsys.hsv_to_rgb(*x), hsv_tuples))
    colors = list(map(lambda x: (int(x[0] * 255), int(x[1] * 255), int(x[2] * 255)), colors))

    # 类别名字
    category = "{}".format(defectKinds[category_idx - 1])  # .encode("utf-8") #categories中的类别序号是从1开始的

    # 坐标
    left, top, right, bottom = [round(coor) for coor in bbox]  # 这里使用了round，而非int

    # 画框 #写类别
    image = Array2Image(pic)
    draw = ImageDraw.Draw(image)
    font = ImageFont.truetype(font='./simhei.ttf', size=np.floor(3e-2 * np.shape(image)[1] + 0.5).astype('int32'))
    thickness = (np.shape(image)[0] + np.shape(image)[1]) // np.shape(image)[0]
    label_size = draw.textsize(category, font)
    text_origin = np.array([left, top - label_size[1]]) if top - label_size[1] >= 0 else np.array(
        [left, top + 1])  # 默认将标签写在框的上方，如果字体过大则将标签写在框内部
    for t in range(thickness):
        draw.rectangle([left + t, top + t, right - t, bottom - t],
                       outline=colors[category_idx - 1])  # [左上角x，左上角y，右下角x，右下角y], outline边框颜色
    draw.rectangle([tuple(text_origin), tuple(text_origin + label_size)], fill=colors[category_idx - 1])
    draw.text(text_origin, str(category.encode("utf-8"), "utf-8"), fill=(0, 0, 0), font=font)
    del draw

    # 输出数组格式的图片
    return Image2Array(image)


# # --- I am a boundary --------------------------------------------------------------------------------------------------
#
#
# # 以下是额外备用函数
# def removeRedundant(coords_x, coords_y, painter):
#     """
#     将pygame中的坐标去冗余
#     coords_x: pygame中的横坐标列表
#     coords_y: pygame中的纵坐标列表
#     painter: 绘图时的Painter实例
#     返回：pygame中的去冗余之后的横坐标列表 以及 pygame中的去冗余之后的纵坐标列表
#     """
#     # pygame 图像的 高 宽
#     height = painter.pygamebottom - painter.pygametop
#     width = painter.pygameright - painter.pygameleft
#     assert height > 0 and width > 0
#
#     # 冗余值
#     border = painter.config.redundantBorder
#     borderA, borderB = border // 2, border - border // 2
#
#     # pygame 图像的 左 上 右 下
#     left, top, right, bottom = 0, 0, width, height
#     # pygame 图像 去除冗余之后的 左 上 右 下
#     realleft, realtop, realright, realbottom = left + borderA, top + borderA, right - borderB, bottom - borderB
#     # # pygame 图像 去除冗余之后的 高 宽
#     # realheight, realwidth = height - (borderA + borderB), width - (borderA + borderB)
#
#     # 新的横坐标列表
#     coords_x = coords_x if isinstance(coords_x, list) else list(coords_x)
#     for idx, coord_x in enumerate(coords_x):
#         coords_x[idx] = min(max(coord_x - borderA, 0), realright - borderA)
#
#     # 新的纵坐标列表
#     coords_y = coords_y if isinstance(coords_y, list) else list(coords_y)
#     for idx, coord_y in enumerate(coords_y):
#         coords_y[idx] = min(max(coord_y - borderA, 0), realbottom - borderA)
#
#     pass
#     # # bbox的 左 上 右 下 占图像真正尺寸 的比例
#     # xmin, ymin, xmax, ymax = xmin / realwidth, ymin / realheight, xmax / realwidth, ymax / realheight
#     #
#     # # bbox的 左 上 右 下 在CAD中的像素位置
#     # xmin, ymin, xmax, ymax = xmin * self.cadwidth, ymin * self.cadheight, xmax * self.cadwidth, ymax * self.cadheight
#     pass
#
#     # 返回
#     return coords_x, coords_y
#
#
# def affineTransform(coords_x, coords_y, _Affine):
#     """
#     # 坐标
#     coords_x = [100.123, 100,456, 16.0, ...]
#     coords_y = [2345.890, 12345.567, 324627.867, ...]
#
#     # 外部在构造_Affine的时候应该采用矩阵相乘的方式，即rotate x scale (注意，先做scale，再做rotate)
#     # 下面给矩阵的元素赋值的做法仅适用于缩放和矩阵都存在有效值的情况
#     # 矩阵
#     _Affine = np.eye(3)
#     _Affine[0][1] = 1.0
#     _Affine[1][0] = 1.0
#     # 缩放
#     _Affine[0][0] *= scaleFactors["x"]
#     _Affine[1][1] *= scaleFactors["y"]
#     _Affine[0][1] *= scaleFactors["y"]
#     _Affine[1][0] *= scaleFactors["x"]
#     # 旋转
#     _Affine[0][0] *= np.cos(rotation)
#     _Affine[0][1] *= -np.sin(rotation)
#     _Affine[1][0] *= np.sin(rotation)
#     _Affine[1][1] *= np.cos(rotation)
#     # 位移
#     _Affine[0][2] = position["x"]
#     _Affine[1][2] = position["y"]
#     """
#     # 坐标矩阵 #每列表示一个点的坐标
#     coords_x, coords_y = np.expand_dims(np.array(coords_x), 0), np.expand_dims(np.array(coords_y), 0)
#     ones = np.ones_like(coords_x)
#     coords = np.concatenate((coords_x, coords_y, ones), 0)
#
#     # 进行仿射变换
#     coords = np.dot(_Affine, coords)
#     if not isinstance(coords[0], Iterable):
#         tmp0 = list()
#         tmp0.append(coords[0])
#         # coords_x = deepcopy(tmp)
#         coords_x = tmp0
#     else:
#         coords_x = list(coords[0])
#     if not isinstance(coords[1], Iterable):
#         tmp1 = list()
#         tmp1.append(coords[1])
#         # coords_y = deepcopy(tmp)
#         coords_y = tmp1
#     else:
#         coords_y = list(coords[1])
#
#     # 返回
#     return coords_x, coords_y
#
#
# def CAD2Pygame(config, screen, originalPoint):
#     # CAD两个最小横纵坐标
#     cadxmin, cadymin = originalPoint
#     # 初始化
#     restoreAffine = np.eye(3)
#     # restoreAffine[0][1] = 1.0 #既有缩放又有旋转的时候再添加这两行
#     # restoreAffine[1][0] = 1.0
#     # 比例缩放
#     restoreAffine[0][0] *= config.restoreScaleFactor #sx
#     restoreAffine[1][1] *= config.restoreScaleFactor #sy
#     # restoreAffine[0][1] *= self.config.restoreScaleFactor #sy #既有缩放又有旋转的时候再添加这两行
#     # restoreAffine[1][0] *= self.config.restoreScaleFactor #sx
#     # 纵坐标翻折
#     restoreAffine[1][1] *= -1
#     restoreAffine[1][2] += screen.get_rect().top + screen.get_rect().bottom  # -y + y0 = -y + top + bottom
#     # 平移 左下角对齐
#     cadleft, cadbottom = affineTransform([cadxmin], [cadymin], restoreAffine)  # 不论是ymin，还是bottom，这样的命名暗示着这个变量是接近于y轴的原点的东西
#     # restoreAffine[0][2] += screen.get_rect().left - cadleft[0] + 5  # 5个像素的冗余
#     restoreAffine[0][2] += screen.get_rect().left - cadleft[0] + config.redundantBorder // 2
#     # restoreAffine[1][2] += screen.get_rect().bottom - cadbottom[0] - 5  # 5个像素的冗余
#     restoreAffine[1][2] += screen.get_rect().bottom - cadbottom[0] - config.redundantBorder // 2
#     # 返回
#     return restoreAffine
