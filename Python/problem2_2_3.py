# !/usr/bin/env python
# -*- coding: utf-8 -*-
# @Time    :# ${2022/4/12}
# @Author  : dengjiaying
# @File    : $problem2_2_3.py
# @Brief   :
# @Version : 0.1
import cv2 as cv
import numpy as np

original_img = cv.imread("E:\\hough_line_detection.png", 0)#读取图片
#图片缩放
img = cv.resize(original_img, None, fx=0.8, fy=0.8,interpolation=cv.INTER_CUBIC)
#用高斯滤波器对图像进行平滑处理
img = cv.GaussianBlur(img, (3, 3), 0)
#对图像进行边缘检测
edges = cv.Canny(img, 50, 150, apertureSize=3)
#在二值图像中查找直线
lines = cv.HoughLines(edges, 1, np.pi / 180, 100)#最后一个数字随便输的....
result = img.copy()
for line in lines:
    #rho : 参数极径 r 以像素值为单位的分辨率. 使用 1 像素.
    rho = line[0][0]  # 第一个元素是距离rho
    #theta: 参数极角 \theta 以弧度为单位的分辨率. 使用 1度 (即CV_PI/180)
    theta = line[0][1]  # 第二个元素是角度theta
    print(rho)
    print(theta)
    if (theta < (np.pi / 4.)) or (theta > (3. * np.pi / 4.0)):  # 垂直直线
        pt1 = (int(rho / np.cos(theta)), 0)  # 该直线与第一行的交点
        # 该直线与最后一行的焦点
        pt2 = (int((rho - result.shape[0] * np.sin(theta)) / np.cos(theta)), result.shape[0])
        # 绘制一条白线
        cv.line(result, pt1, pt2, (255))
    else:  # 水平直线
        pt1 = (0, int(rho / np.sin(theta)))  # 该直线与第一列的交点
        # 该直线与最后一列的交点
        pt2 = (result.shape[1], int((rho - result.shape[1] * np.cos(theta)) / np.sin(theta)))
        # 绘制一条直线
        cv.line(result, pt1, pt2, (255), 1)

cv.imshow('img', original_img)
cv.imshow('Canny', edges)
#输出最后图像
cv.imshow('Result', result)
cv.waitKey(0)
cv.destroyAllWindows()
