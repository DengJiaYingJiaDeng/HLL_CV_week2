# !/usr/bin/env python
# -*- coding: utf-8 -*-
# @Time    :# ${2022/4/12}
# @Author  : dengjiaying
# @File    : $problem2_2_1.py
# @Brief   :
# @Version : 0.1
import numpy as np
import cv2 as cv

#def自定义函数框架（RGB转灰度图）
def Gray_img(img):
    print(img.shape)
    (h, w, c) = img.shape
    #图像单个通道的像素：
    img_b = img[:, :, 0]
    img_g = img[:, :, 1]
    img_r = img[:, :, 2]
    #通过公式G=0.114*B+0.587*G+0.229*R来实现灰度化：
    img_gray = img_r * 0.299 + img_g * 0.587 + img_b * 0.114
    # 用于表示图像的numpy数组数据类型必须为整型，否则无法输出结果
    img_gray = img_gray.astype(np.uint8)
    #展示原图像：
    cv.imshow('img',img)
    #展示灰度图：
    cv.imshow('img_gray', img_gray)
    cv.waitKey(0)
    return 0
#def自定义函数框架（RGB转HSV）
def rgb2hsv(img):
    # 图像的高度
    h = img.shape[0]
    # 图像的宽度
    w = img.shape[1]
    # h行w列的矩阵：
    H = np.zeros((h,w),np.float32)
    S = np.zeros((h, w), np.float32)
    V = np.zeros((h, w), np.float32)
    # 图像通道的分离:
    r,g,b = cv.split(img)
    # 对图像进行归一化，范围为[0,1]:
    r, g, b = r/255.0, g/255.0, b/255.0
    #h,s,v计算公式
    for i in range(0, h):
        for j in range(0, w):
            mx = max((b[i, j], g[i, j], r[i, j]))
            mn = min((b[i, j], g[i, j], r[i, j]))
            V[i, j] = mx
            if V[i, j] == 0:
                S[i, j] = 0
            else:
                S[i, j] = (V[i, j] - mn) / V[i, j]
            if mx == mn:
                H[i, j] = 0
            elif V[i, j] == r[i, j]:
                if g[i, j] >= b[i, j]:
                    H[i, j] = (60 * ((g[i, j]) - b[i, j]) / (V[i, j] - mn))
                else:
                    H[i, j] = (60 * ((g[i, j]) - b[i, j]) / (V[i, j] - mn))+360
            elif V[i, j] == g[i, j]:
                H[i, j] = 60 * ((b[i, j]) - r[i, j]) / (V[i, j] - mn) + 120
            elif V[i, j] == b[i, j]:
                H[i, j] = 60 * ((r[i, j]) - g[i, j]) / (V[i, j] - mn) + 240
            H[i,j] = H[i,j] / 2
    return H, S, V
#读取图像
img = cv.imread('E:\\lena.jpg')

h,s,v = rgb2hsv(img)

img_gray = Gray_img(img)
#展示三种通道的图片：
cv.imshow("h",h)
cv.imshow("s",s)
cv.imshow("v",v)
#前面分离出来的三个通道
merged = cv.merge([h,s,v])
#展示HSV图片：
cv.imshow("hsv",merged)
cv.waitKey(0)
cv.destroyAllWindows()

