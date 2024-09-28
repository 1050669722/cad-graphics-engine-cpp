# -*- coding: utf-8 -*-
"""
---------------------------------------
File Name:  PainterToolsFromPython
Description:
Author:     liuxingbo03
Date:       2021/12/28
---------------------------------------
"""

import sys
import warnings
import numpy as np

if sys.version_info < (3, 8):
    from collections import Iterable
else:
    from collections.abc import Iterable


class PainterTools(object):
    @classmethod
    def isEqual(cls, a, b, threshold=1e-2):
        return abs(a - b) < threshold


    @classmethod
    def calculateDistanceBetweenTwoPoints(cls, p0, p1):
        """
        计算两点之间的距离
        :param p0:
        :param p1:
        :return:
        """
        return np.sqrt((p0[0] - p1[0])**2 + (p0[1] - p1[1])**2)


    @classmethod
    def distanceFromPointToLine(cls, point, line):
        """
        :param point: x, y = point
        :param line: line = (x1, y1, x2, y2)
        """
        # 直线两端点重合为一点的情况
        if cls.isEqual(line[0], line[2]) and cls.isEqual(line[1], line[3]):
            return cls.calculateDistanceBetweenTwoPoints(point, (line[0], line[1]))

        # numpy.ndarray
        point = np.array(point)
        line_point1 = np.array(line[:2])
        line_point2 = np.array(line[2:])

        # 计算向量
        vec1 = line_point1 - point
        vec2 = line_point2 - point
        distance = np.abs(np.cross(vec1, vec2)) / np.linalg.norm(line_point1-line_point2)

        # 返回
        return distance


    @classmethod
    def calculateAreaOfPolygon(cls, coords_x, coords_y):
        area = 0.0

        # # 划分三角形的作法 ###这种做法是错误的，即使是针对凸多边形，有例为证：(485928.5079508659, 485928.50794313353, 488028.5079476411, 488028.5079509051, 486028.50795087666, 486028.50795087306, 485928.5079508659) and (94941.9791155754, 95241.97910421916, 95241.97913849172, 95041.97917025686, 95041.97917033869, 94941.9791155789, 94941.9791155754)
        # for idx in range(1, len(coords_x) - 2, 1):
        #     area += cls.calculateAreaOfTriangle((coords_x[0], coords_y[0]), (coords_x[idx], coords_y[idx]), (coords_x[idx + 1], coords_y[idx + 1]))

        # Green formula #计算任意多边形面积，无论凹凸，但问是否逆时针或者顺时针排列
        coords_x = list(coords_x) + [coords_x[0]]
        coords_y = list(coords_y) + [coords_y[0]]
        for idx in range(0, len(coords_x) - 1, 1):
            area += (coords_x[idx] + coords_x[idx + 1]) * (coords_y[idx + 1] - coords_y[idx])
        area = 0.5 * abs(area)

        # # 判断 凸多边形 或者 凹多边形
        # convexFlag = cls.isConvex(coords_x, coords_y)

        # # print("area: {}".format(area))
        # with open("./areas.txt", mode='a') as f:
        #     f.write("isConvex: {} ## ".format(convexFlag) + str(area) + "\n")
        return area


    @classmethod
    def affineTransform(cls, coords_x, coords_y, _Affine):
        """
        # 坐标
        coords_x = [100.123, 100,456, 16.0, ...]
        coords_y = [2345.890, 12345.567, 324627.867, ...]

        # 外部在构造_Affine的时候应该采用矩阵相乘的方式，即rotate x scale (注意，先做scale，再做rotate)
        # 下面给矩阵的元素赋值的做法仅适用于缩放和矩阵都存在有效值的情况
        # 矩阵
        _Affine = np.eye(3)
        _Affine[0][1] = 1.0
        _Affine[1][0] = 1.0
        # 缩放
        _Affine[0][0] *= scaleFactors["x"]
        _Affine[1][1] *= scaleFactors["y"]
        _Affine[0][1] *= scaleFactors["y"]
        _Affine[1][0] *= scaleFactors["x"]
        # 旋转
        _Affine[0][0] *= np.cos(rotation)
        _Affine[0][1] *= -np.sin(rotation)
        _Affine[1][0] *= np.sin(rotation)
        _Affine[1][1] *= np.cos(rotation)
        # 位移
        _Affine[0][2] = position["x"]
        _Affine[1][2] = position["y"]
        """
        # 坐标矩阵 #每列表示一个点的坐标
        coords_x, coords_y = np.expand_dims(np.array(coords_x), 0), np.expand_dims(np.array(coords_y), 0)
        ones = np.ones_like(coords_x)
        coords = np.concatenate((coords_x, coords_y, ones), 0)

        # 进行仿射变换
        coords = np.dot(_Affine, coords)
        if not isinstance(coords[0], Iterable):
            tmp0 = list()
            tmp0.append(coords[0])
            # coords_x = deepcopy(tmp)
            coords_x = tmp0
        else:
            coords_x = list(coords[0])
        if not isinstance(coords[1], Iterable):
            tmp1 = list()
            tmp1.append(coords[1])
            # coords_y = deepcopy(tmp)
            coords_y = tmp1
        else:
            coords_y = list(coords[1])

        # 返回
        return coords_x, coords_y


    @classmethod
    # 某点绕着某点旋转 #弧度制
    def rotateAroundCenter(cls, point, center, rotation):
        x, y = point
        cx, cy = center

        # 平移
        _Affine_0 = np.eye(3)
        _Affine_0[0][2] = -cx
        _Affine_0[1][2] = -cy
        # 旋转
        _Affine_1 = np.eye(3)
        _Affine_1[:2, :2] = np.array([[np.cos(rotation), -np.sin(rotation)], [np.sin(rotation), np.cos(rotation)]])
        # 平移
        _Affine_2 = np.eye(3)
        _Affine_2[0][2] = cx
        _Affine_2[1][2] = cy
        # 点乘
        x_y = np.dot(np.dot(_Affine_2, np.dot(_Affine_1, _Affine_0)), np.array([[x], [y], [1]]))

        return [x_y[0][0], x_y[1][0]]


    @classmethod
    # def CAD2Pygame(cls, config, screen, originalPoint):
    def CAD2Pygame(cls, config, screen, originalPoint):
        # CAD两个最小横纵坐标
        cadxmin, cadymin = originalPoint
        # 初始化
        restoreAffine = np.eye(3)
        # restoreAffine[0][1] = 1.0 #既有缩放又有旋转的时候再添加这两行
        # restoreAffine[1][0] = 1.0
        # 比例缩放
        restoreAffine[0][0] *= config.restoreScaleFactor #sx
        restoreAffine[1][1] *= config.restoreScaleFactor #sy
        # restoreAffine[0][1] *= self.config.restoreScaleFactor #sy #既有缩放又有旋转的时候再添加这两行
        # restoreAffine[1][0] *= self.config.restoreScaleFactor #sx
        # 纵坐标翻折
        restoreAffine[1][1] *= -1
        restoreAffine[1][2] += screen.get_rect().top + screen.get_rect().bottom  # -y + y0 = -y + top + bottom
        # 平移 左下角对齐
        cadleft, cadbottom = cls.affineTransform([cadxmin], [cadymin], restoreAffine)  # 不论是ymin，还是bottom，这样的命名暗示着这个变量是接近于y轴的原点的东西
        # restoreAffine[0][2] += screen.get_rect().left - cadleft[0] + 5  # 5个像素的冗余
        restoreAffine[0][2] += screen.get_rect().left - cadleft[0] + config.redundantBorder // 2
        # restoreAffine[1][2] += screen.get_rect().bottom - cadbottom[0] - 5  # 5个像素的冗余
        restoreAffine[1][2] += screen.get_rect().bottom - cadbottom[0] - config.redundantBorder // 2
        # 返回
        return restoreAffine


    @classmethod
    def getMaxAreaLBRT(cls, dataDisk):
        # 最大面积 及其 横纵坐标列表 #左下右上
        max_area = 0.0
        max_area_coords_x = []
        max_area_coords_y = []

        # # for test
        # print(len(dataDisk["polylines"]["ori_coords_x"]))

        # 每一个polyline，在转换为梯形之前的横纵坐标
        for ori_coords_x, ori_coords_y in zip(dataDisk["polylines"]["ori_coords_x"], dataDisk["polylines"]["ori_coords_y"]):
            for ori_coords_x_atom, ori_coords_y_atom in zip(ori_coords_x, ori_coords_y):
                # 判断为封闭图形
                if cls.isEqual(ori_coords_x_atom[-1], ori_coords_x_atom[0]) and cls.isEqual(ori_coords_y_atom[-1], ori_coords_y_atom[0]):
                    cur_area = cls.calculateAreaOfPolygon(ori_coords_x_atom, ori_coords_y_atom)
                    # # for test
                    # print("cur_area: ", cur_area)
                    # 最大面积 以及 坐标 是否应该更新
                    if max_area < cur_area:
                        max_area = cur_area
                        # max_area_coords_x = deepcopy(ori_coords_x_atom)
                        max_area_coords_x = ori_coords_x_atom
                        # max_area_coords_y = deepcopy(ori_coords_y_atom)
                        max_area_coords_y = ori_coords_y_atom

        # 获得xmin, ymin, xmax, ymax
        # print("max_area: {}".format(max_area))

        xmin, xmax = min(max_area_coords_x), max(max_area_coords_x)
        ymin, ymax = min(max_area_coords_y), max(max_area_coords_y)

        # print("(xmin, ymin, xmax, ymax) of max_area are: {}".format((xmin, ymin, xmax, ymax)))

        # # for test
        # H, W = ymax - ymin, xmax - xmin
        # H *= 3
        # W *= 2
        # print(xmin - W / 2, ymin - H / 2, xmax + W / 2, ymax + H / 2)
        # return xmin - W / 2, ymin - H / 2, xmax + W / 2, ymax + H / 2

        return xmin, ymin, xmax, ymax
        # return 613827, 174522, 655877, 204222 #115
        # return 571777, 174522, 613827, 204222 #114
        # return 462.0, 426.0, 4976.0, 3292.0
        # return 479087.0, 83393.0, 568187.0, 125393.0


    @staticmethod
    def isOverlapForTwoPoints(pointA, pointB, threshold):
        """
        判断两点是否重合，在阈值允许范围内
        pointA: 点a
        pointB: 点b
        threshold: 阈值
        return: 返回判断结果
        """
        xA, yA = pointA
        xB, yB = pointB
        return PainterTools.isEqual(xA, xB, threshold) and PainterTools.isEqual(yA, yB, threshold)


    @classmethod
    def isOverlapForTwoLines(cls, lineAStartPoint, lineAEndPoint, lineBStartPoint, lineBEndPoint, threshold):
        """
        判断两条线段是否重合（在阈值允许范围内）
        lineAStartPoint: 线段A的起点
        lineAEndPoint: 线段A的终点
        lineBStartPoint: 线段B的起点
        lineBEndPoint: 线段B的终点
        threshold: 点是否重合的阈值
        """
        return (
                    (cls.isOverlapForTwoPoints(lineAStartPoint, lineBStartPoint, threshold))
                    and
                    (cls.isOverlapForTwoPoints(lineAEndPoint, lineBEndPoint, threshold))
            ) \
            or (
                    (cls.isOverlapForTwoPoints(lineAStartPoint, lineBEndPoint, threshold))
                    and
                    (cls.isOverlapForTwoPoints(lineAEndPoint, lineBStartPoint, threshold))
            )


    @staticmethod
    def getslope(pointa, pointb, threshold):
        """
        求斜率
        """
        xA, yA = pointa
        xB, yB = pointb
        assert not PainterTools.isEqual(xA, xB, threshold), "Expect a non-vertical line"
        return (yB - yA) / (xB - xA)


    @staticmethod
    def collimate(pointA, pointB, pointC, pointThreshold, slopeThreshold):
        """
        判断三点共线
        """
        # 坐标获取
        xA, yA = pointA
        xB, yB = pointB
        xC, yC = pointC

        # 如果有两点重合，则此三点一定共线
        if PainterTools.isOverlapForTwoPoints(pointA, pointB, threshold=pointThreshold) \
                or PainterTools.isOverlapForTwoPoints(pointA, pointC, threshold=pointThreshold) \
                or PainterTools.isOverlapForTwoPoints(pointB, pointC, threshold=pointThreshold):
            return True

        # 竖直
        if PainterTools.isEqual(xA, xB, pointThreshold):
            return PainterTools.isEqual(xA, xC, pointThreshold)

        # 水平
        elif PainterTools.isEqual(yA, yB, pointThreshold):
            return PainterTools.isEqual(yA, yC, pointThreshold)

        # 一般情况
        else:
            # 在A和B既不竖直也不水平的情况下，如果B和C水平或者竖直，则三点不共线
            if PainterTools.isEqual(xB, xC, pointThreshold) or PainterTools.isEqual(yB, yC, pointThreshold):
                return False
            # 现在这三点中的任意两点都不会水平或者竖直
            else:
                return PainterTools.isEqual(
                    PainterTools.getslope(pointA, pointB, pointThreshold),
                    PainterTools.getslope(pointB, pointC, pointThreshold),
                    slopeThreshold
                )


    @staticmethod
    def removeRedundant(coords_x, coords_y, painter):
        """
        将pygame中的坐标去冗余
        coords_x: pygame中的横坐标列表
        coords_y: pygame中的纵坐标列表
        painter: 绘图时的Painter实例
        返回：pygame中的去冗余之后的横坐标列表 以及 pygame中的去冗余之后的纵坐标列表
        """
        # pygame 图像的 高 宽
        height = painter.pygamebottom - painter.pygametop
        width = painter.pygameright - painter.pygameleft
        assert height > 0 and width > 0

        # 冗余值
        border = painter.config.redundantBorder
        borderA, borderB = border // 2, border - border // 2

        # pygame 图像的 左 上 右 下
        left, top, right, bottom = 0, 0, width, height
        # pygame 图像 去除冗余之后的 左 上 右 下
        realleft, realtop, realright, realbottom = left + borderA, top + borderA, right - borderB, bottom - borderB
        # # pygame 图像 去除冗余之后的 高 宽
        # realheight, realwidth = height - (borderA + borderB), width - (borderA + borderB)

        # 新的横坐标列表
        coords_x = coords_x if isinstance(coords_x, list) else list(coords_x)
        for idx, coord_x in enumerate(coords_x):
            coords_x[idx] = min(max(coord_x - borderA, 0), realright - borderA)

        # 新的纵坐标列表
        coords_y = coords_y if isinstance(coords_y, list) else list(coords_y)
        for idx, coord_y in enumerate(coords_y):
            coords_y[idx] = min(max(coord_y - borderA, 0), realbottom - borderA)

        pass
        # # bbox的 左 上 右 下 占图像真正尺寸 的比例
        # xmin, ymin, xmax, ymax = xmin / realwidth, ymin / realheight, xmax / realwidth, ymax / realheight
        #
        # # bbox的 左 上 右 下 在CAD中的像素位置
        # xmin, ymin, xmax, ymax = xmin * self.cadwidth, ymin * self.cadheight, xmax * self.cadwidth, ymax * self.cadheight
        pass

        # 返回
        return coords_x, coords_y
