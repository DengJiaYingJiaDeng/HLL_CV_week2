# !/usr/bin/env python
# -*- coding: utf-8 -*-
# @Time    :# ${2022/4/12}
# @Author  : dengjiaying
# @File    : $problem2_2_2.py
# @Brief   :
# @Version : 0.1

import numpy as np
import cv2 as cv

#def自定义函数框架（高斯滤波）
def gaussian_filter(img, K_size=3, sigma=2):

    if len(img.shape) == 3:
        H, W, C = img.shape#图像的属性
    else:
        img = np.expand_dims(img, axis=-1)
        H, W, C = img.shape

    ## Zero padding

    pad = K_size // 2
    #矩阵   ，  dtypt指返回数据元素的数据类型
    out = np.zeros((H + pad * 2, W + pad * 2, C), dtype=np.float)
    #astype()改变数据元素的数据类型
    out[pad: pad + H, pad: pad + W] = img.copy().astype(np.float)

    # 高斯函数计算模板（那个公式）
    K = np.zeros((K_size, K_size), dtype=np.float)
    for x in range(-pad, -pad + K_size):
        for y in range(-pad, -pad + K_size):
            K[y + pad, x + pad] = np.exp( -(x ** 2 + y ** 2) / (2 * (sigma ** 2)))#K=e^((x^2+y^2)/2*sigma^2)
    K /= (2 * np.pi * sigma * sigma)#K=K/(2*pi*sigma^2)

    K /= K.sum()

    tmp = out.copy()

    # filtering
    for y in range(H):
        for x in range(W):
            for c in range(C):
                out[pad + y, pad + x, c] = np.sum(K * tmp[y: y + K_size, x: x + K_size, c])
    #给定一个范围
    out = np.clip(out, 0, 255)

    out = out[pad: pad + H, pad: pad + W].astype(np.uint8)

    return out

# 读取图片
img = cv.imread("E:\\lena.jpg")
#展示图片
cv.imshow('lena',img)

# Gaussian Filter
out = gaussian_filter(img, K_size=3, sigma=2)

#输出变化后的图像
cv.imshow("result", out)

cv.waitKey(0)
cv.destroyAllWindows()
