/*
 *
*
* @file problem2_2_3.cpp
* @author dengjiaying (511492494@qq.com)
* @brief
* @version 0.1
* @date 2022-04-12
*
* @copyend Copyend (c) 2022
*
*/
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include<opencv2/imgproc/imgproc_c.h>

using namespace std;
using namespace cv;

//sobel算子/
//阶乘
int factorial(int n)
{
    int fac = 1;
    //0的阶乘
    if (n == 0)
        return fac;
    for (int i = 1; i <= n; ++i)
    {
        fac *= i;
    }

    return fac;
}

//获得Sobel平滑算子
Mat getSobelSmoooth(int wsize)
{
    int n = wsize - 1;
    Mat SobelSmooothoper = Mat::zeros(Size(wsize, 1), CV_32FC1);
    for (int k = 0; k <= n; k++)
    {
        float *pt = SobelSmooothoper.ptr<float>(0);
        pt[k] = factorial(n) / (factorial(k)*factorial(n - k));
    }
    return SobelSmooothoper;
}

//获得Sobel差分算子
Mat getSobeldiff(int wsize)
{
    Mat Sobeldiffoper = Mat::zeros(Size(wsize, 1), CV_32FC1);
    Mat SobelSmoooth = getSobelSmoooth(wsize - 1);
    for (int k = 0; k < wsize; k++)
    {
        if (k == 0)
            Sobeldiffoper.at<float>(0, k) = 1;
        else if (k == wsize - 1)
            Sobeldiffoper.at<float>(0, k) = -1;
        else
            Sobeldiffoper.at<float>(0, k) = SobelSmoooth.at<float>(0, k) - SobelSmoooth.at<float>(0, k - 1);
    }
    return Sobeldiffoper;
}

//卷积实现
void conv2D(Mat& src, Mat& dst, Mat kernel, int ddepth, Point anchor = Point(-1, -1), int delta = 0, int borderType = BORDER_DEFAULT)
{
    Mat  kernelFlip;
    flip(kernel, kernelFlip, -1);
    filter2D(src, dst, ddepth, kernelFlip, anchor, delta, borderType);
}


//可分离卷积———先垂直方向卷积，后水平方向卷积
void sepConv2D_Y_X(Mat& src, Mat& dst, Mat kernel_Y, Mat kernel_X, int ddepth, Point anchor = Point(-1, -1), int delta = 0, int borderType = BORDER_DEFAULT)
{
    Mat dst_kernel_Y;
    conv2D(src, dst_kernel_Y, kernel_Y, ddepth, anchor, delta, borderType); //垂直方向卷积
    conv2D(dst_kernel_Y, dst, kernel_X, ddepth, anchor, delta, borderType); //水平方向卷积
}

//可分离卷积———先水平方向卷积，后垂直方向卷积
void sepConv2D_X_Y(Mat& src, Mat& dst, Mat kernel_X,Mat kernel_Y, int ddepth, Point anchor = Point(-1, -1), int delta = 0, int borderType = BORDER_DEFAULT)
{
    Mat dst_kernel_X;
    conv2D(src, dst_kernel_X, kernel_X, ddepth, anchor, delta, borderType); //水平方向卷积
    conv2D(dst_kernel_X, dst, kernel_Y, ddepth, anchor, delta, borderType); //垂直方向卷积
}


//Sobel算子边缘检测
//dst_X 垂直方向
//dst_Y 水平方向
void Sobel(Mat& src, Mat& dst_X, Mat& dst_Y, Mat& dst, int wsize, int ddepth, Point anchor = Point(-1, -1), int delta = 0, int borderType = BORDER_DEFAULT)
{

    Mat SobelSmooothoper = getSobelSmoooth(wsize); //平滑系数
    Mat Sobeldiffoper = getSobeldiff(wsize); //差分系数

    //可分离卷积———先垂直方向平滑，后水平方向差分——得到垂直边缘
    sepConv2D_Y_X(src, dst_X, SobelSmooothoper.t(), Sobeldiffoper, ddepth);

    //可分离卷积———先水平方向平滑，后垂直方向差分——得到水平边缘
    sepConv2D_X_Y(src, dst_Y, SobelSmooothoper, Sobeldiffoper.t(), ddepth);

    //边缘强度（近似）
    dst = abs(dst_X) + abs(dst_Y);//绝对值
    convertScaleAbs(dst, dst); //求绝对值并转为无符号8位图
}


//确定一个点的坐标是否在图像内
bool checkInRang(int r,int c, int rows, int cols)
{
    if (r >= 0 && r < rows && c >= 0 && c < cols)
        return true;
    else
        return false;
}

//从确定边缘点出发，延长边缘
void trace(Mat &edgeMag_noMaxsup, Mat &edge, float TL,int r,int c,int rows,int cols)
{
    if (edge.at<uchar>(r, c) == 0)
    {
        edge.at<uchar>(r, c) = 255;
        for (int i = -1; i <= 1; ++i)
        {
            for (int j = -1; j <= 1; ++j)
            {
                float mag = edgeMag_noMaxsup.at<float>(r + i, c + j);
                if (checkInRang(r + i, c + j, rows, cols) && mag >= TL)
                {
                    trace(edgeMag_noMaxsup, edge, TL, r + i, c + j, rows, cols);
                }
            }
        }
    }
}

//Canny边缘检测函数
void Edge_Canny(Mat &src, Mat &edge, float TL, float TH, int wsize=3, bool L2graydient = false)
{
    int rows = src.rows;
    int cols = src.cols;

    //高斯滤波
    GaussianBlur(src,src,Size(5,5),0.8);
    //sobel算子
    Mat dx, dy, sobel_dst;
    Sobel(src, dx, dy, sobel_dst, wsize, CV_32FC1);

    //计算梯度幅值
    Mat edgeMag;
    if (L2graydient)
    {
        magnitude(dx, dy, edgeMag); //开平方
    }
    else
    {
        edgeMag = abs(dx) + abs(dy); //绝对值之和近似
    }

    //计算梯度方向 以及 非极大值抑制
    Mat edgeMag_noMaxsup = Mat::zeros(rows, cols, CV_32FC1);
    for (int r = 1; r < rows - 1; ++r)
    {
        for (int c = 1; c < cols - 1; ++c)
        {
            float x = dx.at<float>(r, c);
            float y = dy.at<float>(r, c);
            float angle =atan2f(y, x) / CV_PI * 180; //当前位置梯度方向
            float mag = edgeMag.at<float>(r, c);  //当前位置梯度幅值

            //非极大值抑制
            //垂直边缘--梯度方向为水平方向-3*3邻域内左右方向比较
            if (abs(angle)<22.5 || abs(angle)>157.5)
            {
                float left = edgeMag.at<float>(r, c - 1);
                float right = edgeMag.at<float>(r, c + 1);
                if (mag >= left && mag >= right)
                    edgeMag_noMaxsup.at<float>(r, c) = mag;
            }

            //水平边缘--梯度方向为垂直方向-3*3邻域内上下方向比较
            if ((angle>=67.5 && angle<=112.5 ) || (angle>=-112.5 && angle<=-67.5))
            {
                float top = edgeMag.at<float>(r-1, c);
                float down = edgeMag.at<float>(r+1, c);
                if (mag >= top && mag >= down)
                    edgeMag_noMaxsup.at<float>(r, c) = mag;

            }
            //+45°边缘--梯度方向为其正交方向-3*3邻域内右上左下方向比较
            if ((angle>112.5 && angle<=157.5) || (angle>-67.5 && angle<=-22.5))
            {
                float right_top = edgeMag.at<float>(r - 1, c+1);
                float left_down = edgeMag.at<float>(r + 1, c-1);
                if (mag >= right_top && mag >= left_down)
                    edgeMag_noMaxsup.at<float>(r, c) = mag;
            }


            //+135°边缘--梯度方向为其正交方向-3*3邻域内右下左上方向比较
            if ((angle >=22.5 && angle < 67.5) || (angle >= -157.5 && angle < -112.5))
            {
                float left_top = edgeMag.at<float>(r - 1, c - 1);
                float right_down = edgeMag.at<float>(r + 1, c + 1);
                if (mag >= left_top && mag >= right_down)
                    edgeMag_noMaxsup.at<float>(r, c) = mag;
            }
        }
    }

    //双阈值处理及边缘连接
    edge =Mat::zeros(rows, cols, CV_8UC1);
    for (int r = 1; r < rows - 1; ++r)
    {
        for (int c = 1; c < cols - 1; ++c)
        {
            float mag = edgeMag_noMaxsup.at<float>(r, c);
            //大于高阈值，为确定边缘点
            if (mag >= TH)
            {
                trace(edgeMag_noMaxsup, edge, TL, r, c, rows, cols);
            }
            else if (mag < TL)
            {
                edge.at<uchar>(r, c) = 0;
            }
        }
    }

}

int main()
{
    Mat src = imread("E:\\hough_line_detection.png");

    if (src.empty())
    {
        return -1;
    }
    if (src.channels() > 1)
    {
        cvtColor(src, src, COLOR_RGB2GRAY);
    }
    Mat edge,dst;

    //Canny
    Edge_Canny(src, edge, 20,60);
    //转化边缘检测后的图为灰度图
    cvtColor(edge,dst, CV_GRAY2BGR);
    //进行霍夫线变换
    //定义一个矢量结构lines用于存放得到的线段矢量集合
     vector<Vec2f> lines;
     HoughLines(edge, lines, 1, CV_PI/180, 150, 0, 0 );
     //把得到的直线显示在图中
     //Scalar color = Scalar(255, 0, 0);
    //依次在图中绘制出每条线段
     for( size_t i = 0; i < lines.size(); i++ )
     {
         //以像素为单位的距离精度
         float rho = lines[i][0], theta = lines[i][1];
         Point pt1, pt2;
         //以弧度为单位的角度精度
         double a = cos(theta), b = sin(theta);
         //r=x*cos(theta)+y*sin(theta)
         double x0 = a*rho, y0 = b*rho;
         pt1.x = cvRound(x0 + 1000*(-b));
         pt1.y = cvRound(y0 + 1000*(a));
         pt2.x = cvRound(x0 - 1000*(-b));
         pt2.y = cvRound(y0 - 1000*(a));
         //绘制直线
         line( dst, pt1, pt2, Scalar(255,0,0), 1, CV_AA );
     }


    //Canny(src, dst, 20, 80);
    //新窗口
    namedWindow("src", WINDOW_NORMAL);
    //展示原图像
    imshow("src", src);
    //新窗口
    namedWindow("My_canny", WINDOW_NORMAL);
    //展示canny边缘提取之后的图像
    imshow("My_canny", edge);
    //展示边缘检测后的图
    //imshow("after1",edge);
    //展示最后的效果图
    imshow("after2", dst);

    waitKey(0);

    system("PAUSE");
    return 0;
}
