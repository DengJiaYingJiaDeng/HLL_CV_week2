/*
*
* @file problem2_2_1.cpp
* @author dengjiaying (511492494@qq.com)
* @brief
* @version 0.1
* @date 2022-04-12
*
* @copyend Copyend (c) 2022
*
*/
#include<opencv2/opencv.hpp>
#include<iostream>
#include<time.h>
#include<string>
#include<vector>
using namespace std;
using namespace cv;

void Rgb2Hsv(float R, float G, float B, float& H, float& S, float&V)
{
    // r,g,b的值是0到1之间的实数
    // h = [0,360], s = [0,1], v = [0,1]
    // if s == 0, then h = -1 (undefined)
    float min, max, delta, tmp;
    //三目运算符 给min赋值（min为r,g,b中的最小值）
    tmp = R>G ? G : R;
    min = tmp>B ? B : tmp;
    //三目运算符 给max赋值（min为r,g,b中的最大值）
    tmp = R>G ? R : G;
    max = tmp>B ? tmp : B;
    //s（饱和度）  v(亮度)
    V = max / 255 * 100; // v
    //计算公式中的分母
    delta = max - min;

    if (max != 0)
        S = delta / max *100; // s
    else
    {
        // r = g = b = 0 // s = 0, v is undefined
        S = 0;
        H = 0;
        return;
    }
    //灰色
    if (delta == 0)
    {
        H = 0;
        return;
    }
    //s，v计算公式
    else if (R == max)
    {
        if (G >= B)
            H = (G - B) / delta; //yellow 到 magenta
        else
            H = (G - B) / delta + 6.0;
    }
    else if (G == max)
        H = 2.0 + (B - R) / delta; //cyan 到 yellow
    else if (B == max)
        H = 4.0 + (R - G) / delta; //magenta 到 cyan

    H *= 60.0; // degrees（计算公式）

}
int main()
{
    Mat srcImage =cv::imread("E:\\lena.jpg");
    //如果图片不存在
    if(srcImage.empty())
    {
        cout << "can't read this image!" << endl;
        return -1;
    }

    Mat gray = Mat::zeros(srcImage.rows, srcImage.cols, CV_8UC1);
    for (int i = 0; i < srcImage.rows; i++)
    {
        for (int j = 0; j < srcImage.cols; j++)
        {
            //查找两数最大值并返回最大数
            gray.at<uchar>(i, j) = fmax(srcImage.at<Vec3b>(i, j)[0],fmax(srcImage.at<Vec3b>(i, j)[1], srcImage.at<Vec3b>(i, j)[2]));
        }
    }

    namedWindow("img", WINDOW_AUTOSIZE);
    namedWindow("gray", WINDOW_AUTOSIZE);
    //展示原图像
    imshow("img",srcImage);
    //展示灰度图
    imshow("gray", gray);

    int row = srcImage.rows;
    int col = srcImage.cols;
    //创建HSV图像
    Mat hsvimg= Mat(row, col ,CV_8UC3);

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            float b = srcImage.at<Vec3b>(i, j)[0];
            float g = srcImage.at<Vec3b>(i, j)[1];
            float r = srcImage.at<Vec3b>(i, j)[2];
            float h, s, v;

            Rgb2Hsv(r, g, b, h, s, v);
            hsvimg.at<Vec3b>(i, j)[0] = h;//h数值若大于255会自动减去255
            hsvimg.at<Vec3b>(i, j)[1] = s;
            hsvimg.at<Vec3b>(i, j)[2] = v;
        }
    }
    //展示HSV图像
    imshow("hsv",hsvimg);

    waitKey(0);
    return 0;
}
