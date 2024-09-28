# -*- coding: utf-8 -*-
"""
---------------------------------------
File Name:  PainterFromPython
Description:
Author:     liuxingbo03
Date:       2021/12/28
---------------------------------------
"""

import sys
import math

if sys.version_info < (3, 8):
    from collections import Iterable
else:
    from collections.abc import Iterable

from collections import OrderedDict

from pycad.interface.launchers.painterLauncher.PainterToolsFromPython import PainterTools as PT
from pycad.interface.Utils import *


class PainterFromPython(object):
    def __init__(self, config, dataDisk, xmin_xmax_ymin_ymax=()):
        # 配置 数据结构 初始化环境
        self.config = config
        self.dataDisk = dataDisk
        self.xmin_xmax_ymin_ymax = xmin_xmax_ymin_ymax
        self._setEnv()

        # 用于搜集信息的数据结构
        self.aggData = [] #OrderedDict()


    def _adjustAppropriateScaleFactor(self, cadxmin, cadymin, cadxmax, cadymax):
        # resotreResolution参与
        # if hasattr(self.config, "resotreResolution") and self.config.resotreResolution:
        if self.config.resotreResolution > 0.0:
            self.config.restoreScaleFactor = self.config.resotreResolution / max(cadymax - cadymin, cadxmax - cadxmin)
        # else:
        #     # self.config.restoreScaleFactor = self.config.restoreScaleFactor
        #     pass

        # 绘图尺寸限制 比例反馈机制
        currentResolutionHeight = self.config.restoreScaleFactor * (cadymax - cadymin)
        limitHeight = 10000.0
        if currentResolutionHeight > limitHeight: #hard-coded
            self.config.restoreScaleFactor = limitHeight / (cadymax - cadymin)
            print("[WARNING] the window is too large, and \"painter.config.restoreScaleFactor\" has been modified to {}".format(self.config.restoreScaleFactor))


    # def _getMaxAreaLBRT(self):
    #     return PT.getMaxAreaLBRT(dataDisk=self.dataDisk)


    def _getMaxAreaLBRT(self):
        # 特殊情况
        if len(self.xmin_xmax_ymin_ymax) != 0:
            xmin, xmax, ymin, ymax = self.xmin_xmax_ymin_ymax
            return xmin, xmax, ymin, ymax
        # 一般情况
        return PT.getMaxAreaLBRT(dataDisk=self.dataDisk)


    def _setEnv(self):
        # 在pygame画图期间不显示窗口
        os.environ['SDL_VIDEODRIVER'] = "dummy"
        # 初始化背景 和 屏幕
        pygame.init()

        # 根据图形 确定 绘图的范围 屏幕的尺寸
        cadxmin, cadymin, cadxmax, cadymax = self._getMaxAreaLBRT()  # (-1000, -1000, 1000, 1000)
        self.cadxmin, self.cadymin, self.cadxmax, self.cadymax = cadxmin, cadymin, cadxmax, cadymax #作为Painter的成员属性
        self._adjustAppropriateScaleFactor(cadxmin, cadymin, cadxmax, cadymax) #这里必须修改self.config，因为下面多处用到

        pygameleft, pygametop, pygameright, pygamebottom = list(map(lambda x: math.ceil(x * self.config.restoreScaleFactor), [cadxmin, cadymin, cadxmax, cadymax]))
        self.pygameleft, self.pygametop, self.pygameright, self.pygamebottom = pygameleft, pygametop, pygameright, pygamebottom  # 作为Painter的成员属性

        # 假·screen
        # CAD -> pygame 仿射变换矩阵 以及对齐 （没有冗余的）#此步骤需要在真正的screen前面，因pygame只维护一个screen实例 #[单例模式] screen is screenWithoutRedundant: True
        screenWithoutRedundant = pygame.display.set_mode((pygameright - pygameleft + 0, pygamebottom - pygametop + 0))
        self.restoreAffineWithoutRedundant = PT.CAD2Pygame(config=self.config, screen=screenWithoutRedundant, originalPoint=(cadxmin, cadymin))

        # 真·screen
        screen = pygame.display.set_mode((pygameright - pygameleft + self.config.redundantBorder, pygamebottom - pygametop + self.config.redundantBorder))  # (800, 600) #加上10个pixel的冗余, pygame.FULLSCREEN, pygame.RESIZABLE, pygame.NOFRAME
        screen.fill(self.config.colorMap["white"]) #(255, 255, 255)
        self.screen = screen

        # CAD -> pygame 仿射变换矩阵 以及对齐
        self.restoreAffine = PT.CAD2Pygame(config=self.config, screen=self.screen, originalPoint=(cadxmin, cadymin))


    def paint(self):
        """
        逐 图元类型 画
        """

        # plot commonTypes
        self._paintC_lines()
        self._paintC_polylines()
        self._paintC_circles()
        self._paintC_arcs()
        self._paintC_ellipses()
        self._paintC_points()
        self._paintC_splines()
        self._paintC_solids()

        # plot txtTypes
        self._paintC_texts()
        self._paintC_mtexts()

        # plot weirdTypes

        # plot attributes
        self._paintC_attributes()

        # plot dimenTypes
        self._paintC_alignedDimensions()
        self._paintC_rotatedDimensions()
        self._paintC_lineAngularDimensions()
        self._paintC_arcDimensions()
        self._paintC_radialDimensions()
        self._paintC_diametricDimensions()
        self._paintC_ordinateDimensions()
        self._paintC_radialDimensionLarges()

        # 保存图片
        if self.config.isSaveFig:
            os.makedirs(self.config.savePath, exist_ok=True)
            pygame.image.save(self.screen, os.path.join(self.config.savePath, self.config.saveFileName))
            if self.config.verbose:#True:#
                print("[INFO] The picture has been saved to {}".format(self.config.savePath))

        # 图像转换
        img = RGB2BGR(screen2Array(self.screen))

        # 返回
        return img#, self.config.restoreScaleFactor, frame_text_dict, bboxCheckmark_list, bboxCheckmark_T_list, (pygameleft, pygametop, pygameright, pygamebottom), cad_points_dict, pygame_points_dict


    def _loadData(self, eType):
        """
        加载数据
        当前正在画的[current painting]这类元素的 坐标、颜色
        """
        # 坐标
        # 对于非polyline来说是这样的：[[()], [()], ...]
        # 对于polyline来说是这样的：[[(), (), ...], [(), (), ...], ...]
        self.painting_coords_x = self.dataDisk[eType]["coords_x"]
        self.painting_coords_y = self.dataDisk[eType]["coords_y"]
        # 颜色 #['c', 'c', ...]
        self.painting_colors = self.dataDisk[eType]["colors"] #名字的改变：color -> colors


    def _paintCore(self, eType): #与Tree中的enrichDataCore方法并不是很类似
        # 加载数据
        self._loadData(eType)

        # 遍历 #遍历格式统一到原子级别
        # assert eType in RenderingDatabase.commonTypes, \
        #     "Expect eType: {} is in commonTypes: {}".format(eType, RenderingDatabase.commonTypes)
        """
        commonTypes = ("lines", "polylines", "circles", "arcs", "ellipses", "points", "splines", )
        """
        for coord_x, coord_y, color in zip(self.painting_coords_x, self.painting_coords_y, self.painting_colors):
            for coord_x_atom, coord_y_atom in zip(coord_x, coord_y):
                # 这些 是 这个原子的：coord_x_atom, coord_y_atom, color
                # 空间变换：CAD -> pygame
                coord_x_atom, coord_y_atom = PT.affineTransform(coord_x_atom, coord_y_atom, self.restoreAffine)
                p_list = [(cxa, cya) for (cxa, cya) in zip(coord_x_atom, coord_y_atom)]
                # # for-test
                # if eType == "polylines" and len(p_list) < 2:
                #     print(1234)
                if eType == "polylines": #TODO: 更新了clip模块以后，这里会出现仅有两个点的p_list；cpp版本不会出现问题，可是pygame这里不允许； #已修复
                    pygame.draw.polygon(self.screen, self.config.colorMap[color], p_list)
                    pygame.draw.aalines(self.screen, self.config.colorMap[color], False, p_list, 1)
                elif eType == "points":
                    pointSize = 1e-4 * (self.screen.get_rect().size[0] + self.screen.get_rect().size[1]) / 2  # 用圆来替代点 #半径比例经测试是合理的，只是颜色尚值得商榷 #应该手动画CAD验证
                    for point in p_list:
                        pygame.draw.circle(self.screen, self.config.colorMap[color], point, pointSize, round(pointSize)) # pointSize 用原的半径和线宽代替 #线宽必须得是整型
                else:
                    # try:
                    # for test
                    # print(eType)
                    # if eType == "alignedDimensions":
                    #     print(123)
                    pygame.draw.aalines(self.screen, self.config.colorMap[color], False, p_list, 1)
                    # except:
                    #     print(p_list)

                    # # for test
                    # if eType == "splines" or eType == "ellipses":
                    #     print(eType, p_list)


    # commonTypes ##########################
    def _paintC_lines(self):
        self._paintCore("lines")

    def _paintC_polylines(self):
        self._paintCore("polylines")

    def _paintC_circles(self):
        self._paintCore("circles")

    def _paintC_arcs(self):
        self._paintCore("arcs")

    def _paintC_ellipses(self):
        self._paintCore("ellipses")

    def _paintC_points(self):
        self._paintCore("points")

    def _paintC_splines(self):
        self._paintCore("splines")

    def _paintC_solids(self):
        self._paintCore("solids")
    ########################################


    # txtTypes #############################
    def _isPlot(self, eType, TextOrBox):
        """
        在yaml中的开关还做了两类：Text 和 Box
        根据 开关类型 和 eType 来进行判断
        """
        for sw, _bool in getattr(self.config, "isPlot" + TextOrBox.title()).items():
            if eType.lower() in sw.lower():
                return _bool
        return True

    def _paintC_texts(self, eType="texts"):
        """
        抛开paintCore，重新定义paintC_texts
        """
        # 元素类型为"texts" #RenderingDatabase.txtTypes
        assert (eType in ("texts", "mtexts", )) or eType == "attributes", \
            "Expect eType: {} is in txtTypes: {} or eType == 'attributes'".format(eType, ("texts", "mtexts", )) #RenderingDatabase.txtTypes
        """
        txtTypes = ("texts", "mtexts", )
        """

        # 加载数据
        self._loadData(eType)
        # 加载数据 - texts特有
        # 文本信息 ['', '', ...]
        infos = self.dataDisk[eType]["info"]  # 名字：info -> infos
        # 旋转弧度 [0.0, 0.0, ...]
        rotas = self.dataDisk[eType]["rota"]  # 名字：rota -> rotas
        # 单字 宽度 与 高度 的比例
        widthFactors = self.dataDisk[eType]["widthFactor"]  # 名字：widthFactor -> widthFactors

        # 画
        # 实例化字体
        font = pygame.font.Font(self.config.fontfile, self.config.fontsize)
        # 遍历每一个文本实例的数据
        # 同在zip中，遍历以最短Iterable为准
        # 例如，若coords_x为[]，而其它的结构长度不为0，则这个zip也是不会遍历的
        for idx, (coord_x, coord_y, color, info, rota, widthFactor) in \
                enumerate(zip(self.painting_coords_x, self.painting_coords_y, self.painting_colors, infos, rotas, widthFactors)):
            # if info == "\"\"":  # 空字符串必须排除 #有些空字符串的尺寸很诡异
            # if info == "\"\"" or info == "\'\'":  # 空字符串必须排除 #有些空字符串的尺寸很诡异
            # if info in {"\"\"", "\'\'", "\""}:  # 空字符串必须排除 #有些空字符串的尺寸很诡异
            if (info in self.config.vacantTextInfo) and (self.config.isExcludeVacantTextInfo is True):  # 空字符串必须排除 #有些空字符串的尺寸很诡异
                continue
            for coord_x_atom, coord_y_atom in zip(coord_x, coord_y):
                # 获得高 文本框的高 在pygame空间中的
                backRotateAffine = np.eye(3)
                # backRotateAffine[0][1] = 1.0 #既有缩放又有旋转的时候再添加这两行
                # backRotateAffine[1][0] = 1.0
                backRotation = -rota  # * np.pi / 180.0  # 角度换弧度 #想要把矩形文本框转回去？ #矩形文本框在parse阶段已经被旋转了，那时候的坐标系y轴正方向朝上 #此时的矩形文本框的坐标已然经过了affineTransform，因此-rotation也应该经过一下affineTransform # 或者更好的办法是在affineTransform之前，先做-rotation
                backRotateAffine[:2, :2] = \
                    np.array([[np.cos(backRotation), -np.sin(backRotation)], [np.sin(backRotation), np.cos(backRotation)]])
                csx, csy = PT.affineTransform(coord_x_atom, coord_y_atom, backRotateAffine)  # 旋转回来
                _, csy = PT.affineTransform(csx, csy, self.restoreAffine)  # CAD -> pygame
                h_tbox = max(csy) - min(csy)

                # 画 文本框
                coord_x_atom, coord_y_atom = PT.affineTransform(coord_x_atom, coord_y_atom, self.restoreAffine) #CAD -> pygame
                # 在此处搜集 经过滤除和变换了的 文本 和 文本框 数据 #以字典的形式 #逐条 #逐键值对 #所谓信息，一般地应该以这种字典形式呈现
                self._aggregateData({"text_info": info, "text_coord_x_atom": coord_x_atom, "text_coord_y_atom": coord_y_atom})
                # 转换为点列表
                p_list = [(cxa, cya) for (cxa, cya) in zip(coord_x_atom, coord_y_atom)]
                if self._isPlot(eType=eType, TextOrBox="Box"): #这个判断如果放到上面 效率应该会更好
                    pygame.draw.aalines(self.screen, self.config.colorMap[color], False, p_list, 1)

                if eType == "texts" or eType == "attributes":
                    # 文本
                    text = font.render(info, True, self.config.colorMap[color], None)
                    w_text, h_text = text.get_size()

                    # 第一比例因子缩放
                    text = pygame.transform.rotozoom(text, rota * 180 / np.pi, h_tbox / h_text)
                    # 第二比例因子缩放
                    text = pygame.transform.rotozoom(text, 0.0 * 180 / np.pi, widthFactor)

                    # offsetVector向量
                    # 文本框中心
                    tbox_center = ((min([p[0] for p in p_list]) + max([p[0] for p in p_list])) / 2,
                                   (min([p[1] for p in p_list]) + max([p[1] for p in p_list])) / 2)
                    # 文本坐标
                    try:
                        text_coord = list(map(lambda x: (x[0] + p_list[3][0], x[1] + p_list[3][1]),
                                              [text.get_rect().topleft, text.get_rect().topright,
                                               text.get_rect().bottomright, text.get_rect().bottomleft]))
                    except:
                        pass

                    # 文本中心
                    text_center = ((min([tc[0] for tc in text_coord]) + max([tc[0] for tc in text_coord])) / 2,
                                   (min([tc[1] for tc in text_coord]) + max([tc[1] for tc in text_coord])) / 2)
                    # 目标
                    dst_x, dst_y = tbox_center[0], tbox_center[1]
                    # 起始
                    ori_x, ori_y = text_center[0], text_center[1]
                    # 向量
                    offsetVector = (dst_x - ori_x, dst_y - ori_y)

                    # 放置文本
                    settle_xmin = p_list[3][0] + offsetVector[0]
                    settle_ymin = p_list[3][1] + offsetVector[1]
                    if self._isPlot(eType=eType, TextOrBox="Text"): #这个判断如果放到上面 效率应该会更好
                        self.screen.blit(text, (settle_xmin, settle_ymin))

                elif eType == "mtexts": #mtexts中是否含有 文本框 比例 位移 等 的处理?
                    words = [word for word in info.splitlines()]
                    lineNum = self.dataDisk[eType]["lineNums"][idx] // 5
                    max_width, max_height = max([p[0] for p in p_list]) - min([p[0] for p in p_list]), max([p[1] for p in p_list]) - min([p[1] for p in p_list])
                    # 根据开始估算的行数 计算每行大致的行高
                    # lineHeight = max_height / lineNum
                    lineHeight = max_height / max(lineNum, 1)
                    # 初始位置 当前文本框左上角的点
                    init_pos= p_list[3][0], p_list[3][1]

                    # y_list = np.linspace(init_pos[1], p_list[3][1], num=lineNum)

                    x, y = init_pos
                    for line in words:
                        for word in line:
                            word_surface = font.render(word, False, self.config.colorMap[color], None)
                            word_w, word_h = word_surface.get_size()
                            # 缩放
                            word_surface = pygame.transform.rotozoom(word_surface, rota * 180 / np.pi, lineHeight / word_h)
                            word_width, word_height = word_surface.get_size()
                            if x + word_width >= init_pos[0] + max_width:
                                # reset init_x and start a new row
                                x = init_pos[0]
                                y += lineHeight
                            if self._isPlot(eType=eType, TextOrBox="Text"):
                                self.screen.blit(word_surface, (x, y))
                            x += word_width
                        x = init_pos[0]
                        y += lineHeight
                    pass

    def _paintC_mtexts(self):
        self._paintC_texts(eType="mtexts")
        pass
    ########################################


    # weirdTypes ###########################
    def _paintC_hatchs(self):
        pass
    ########################################


    # referTypes ###########################
    def _paintC_attributes(self):
        self._paintC_texts(eType="attributes")
    ########################################


    # dimenTypes ###########################
    def _paintC_alignedDimensions(self):
        self._paintCore(eType="alignedDimensions")

    def _paintC_rotatedDimensions(self):
        self._paintCore(eType="rotatedDimensions")

    def _paintC_lineAngularDimensions(self):
        self._paintCore(eType="lineAngularDimensions")

    def _paintC_arcDimensions(self):
        self._paintCore(eType="arcDimensions")

    def _paintC_radialDimensions(self):
        self._paintCore(eType="radialDimensions")

    def _paintC_diametricDimensions(self):
        self._paintCore(eType="diametricDimensions")

    def _paintC_ordinateDimensions(self):
        self._paintCore(eType="ordinateDimensions")

    def _paintC_radialDimensionLarges(self):
        self._paintCore(eType="radialDimensionLarges")
    ########################################


    def _aggregateData(self, aggData):
        """
        搜集信息
        """
        self.aggData.append(aggData)


    def outputTextsData(self):
        """
        处理文本信息数据
        如果有 其它信息， 应该另建一个类似方法
        """
        # 为 文本信息 定义容器
        text = []
        frame = OrderedDict() #defaultdict()
        frame["coords_x"], frame["coords_y"] = [], []
        text_frame_dict = OrderedDict()

        # 为 其它信息 定义容器

        # 整理搜集信息中的文本信息
        for _dict in self.aggData:
            for key, value in _dict.items():
                # 与文本信息相关的键 #处理这一键值对
                """
                text指示的东西
                text: {
                    "texts", 
                    "mtexts", 
                    "attributes", 
                    }
                """
                if "text" in key: #"text"为 文本信息 的标识关键字
                    if "text_info" in key: #文本
                        if self.config.isExcludeVacantTextInfo is True:
                            assert value != "\"\"" #在paint方法中已经判定if info == "\"\"": continue
                        text.append(value) #装入容器
                    elif "text_coord_x_atom" in key: #横坐标
                        frame["coords_x"].append(value) #装入容器
                    elif "text_coord_y_atom" in key: #纵坐标
                        frame["coords_y"].append(value) #装入容器
                    else:
                        raise KeyError("Key {} is not found or not for text".format(key))

                #其它信息elif #与文本信息无关的键 #跳过这一键值对
                else:
                    continue

        # 为 文本信息 组装容器
        text_frame_dict["text"] = text
        text_frame_dict["frame"] = frame

        # 为 其它信息 组装容器

        # 返回
        return text_frame_dict



if __name__ == "__main__":
    pass
