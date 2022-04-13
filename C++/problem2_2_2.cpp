/*
*
* @file problem2_2_2.cpp
* @author dengjiaying (511492494@qq.com)
* @brief
* @version 0.1
* @date 2022-04-12
*
* @copyend Copyend (c) 2022
*
*/
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <math.h>


using namespace cv;
using namespace std;


//高斯滤波
cv::Mat gaussian_filter(cv::Mat img, double sigma, int kernel_size)
{
    int height = img.rows;//高度
    int width = img.cols;//宽度
    //图像通道个数
    int channel = img.channels();
    // prepare output
    cv::Mat out = cv::Mat::zeros(height, width, CV_8UC3);

    // prepare kernel
    //floor()函数，向下取整
    int pad = floor(kernel_size / 2);
    int _x = 0, _y = 0;
    double kernel_sum = 0;

    // get gaussian kernel
    float kernel[kernel_size][kernel_size];

    for (int y = 0; y < kernel_size; y++)
    {
        for (int x = 0; x < kernel_size; x++)
        {
          _y = y - pad;
          _x = x - pad;
          //高斯滤波公式：
          kernel[y][x] = 1 / (2 * M_PI * sigma * sigma) * exp( - (_x * _x + _y * _y) / (2 * sigma * sigma));
          kernel_sum += kernel[y][x];
        }
    }

    for (int y = 0; y < kernel_size; y++)
    {
        for (int x = 0; x < kernel_size; x++)
        {
          kernel[y][x] /= kernel_sum;
        }
    }


    // 遍历图像属性
    double v = 0;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
          for (int c = 0; c < channel; c++)
          {
              v = 0;

            for (int dy = -pad; dy < pad + 1; dy++)
            {
                for (int dx = -pad; dx < pad + 1; dx++)
                {
                    if (((x + dx) >= 0) && ((y + dy) >= 0))
                    {
                        v += (double)img.at<cv::Vec3b>(y + dy, x + dx)[c] * kernel[dy + pad][dx + pad];//<Vec3b>8U类型的RGB彩色图像（0-255）
                    }
                }
            }
             out.at<cv::Vec3b>(y, x)[c] = v;
           }
        }
    }
   return out;
}

int main()
{

    // 读取原图片
    cv::Mat img = cv::imread("E:\\lena.jpg", cv::IMREAD_COLOR);
    cv::imshow("img",img);
    //根据题意，卷积核为3*3，方差为2
    cv::Mat out = gaussian_filter(img, 2, 3);
    //展示处理后的图片
    cv::imshow("after", out);
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}




