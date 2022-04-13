/*
*
* @file problem2_1.cpp
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

using namespace cv;
using namespace std;

//当前视频帧索引
int pos = 0;
//需要使用滑动条功能，所以设置全局变量
VideoCapture cap;
 void onTrackbarSlide(int pos,void* )//回调函数
{
     //设置当前的视频帧数与滑块位置相对应
    cap.set(CAP_PROP_POS_FRAMES,pos);
}

int main()
{
    //读取视频
    cap.open("E:\\SJZtest\\dengjiaying\\problem2_1\\sing.mp4");
    //如果视频无法打开
    if (!cap.isOpened())
    {
        cout << "Play video failed!" << endl;
    }

    //获取视频帧数
    int frame_count = cap.get(CAP_PROP_FRAME_COUNT);
    //视频窗口
    namedWindow("Video Player", 1);
    int frames = cap.get(CAP_PROP_FRAME_COUNT);
    //创建滑动控件（滑动控件名称，滑动控件的窗口名称，初始化阈值，滑动控件范围，回调函数）
    //将一个名为scrollbar的滑动条显示在名为Video Player的窗口上，当滑块位置pos变化时调用回调函数onTrackbarSlide。
    createTrackbar("scrollbar", "Video Player", &pos, frames, onTrackbarSlide);
    Mat frame;
    int delay=30;//每秒的帧数
    while (1)
    {
        // Mat frame;
        cap>>frame;
        if (frame.empty()) break;
        imshow("Video Player", frame);

        //键盘响应
        char c=waitKey(30);
        //Esc键的ASCII码值为27,按下Esc键，结束放映
        if (c == 27)
        {
            break;
        }
        //空格的ASCII码值为32，控制视频暂停和播放
        else if(delay>=0&&c==32)
        {
            waitKey(0);
        }
        //A的ASCII码值为65,a的ASCII码值为97,控制视频回放
        else if (c ==97||c==65 )
        {
            pos-=30;
            cap.set(CAP_PROP_POS_FRAMES, pos);

        }
        //D的ASCII码值为68,d的ASCII码值为100,控制视频快进（4帧）
        else if(c==68||c==100)
        {
            pos+=4;
            cap.set(CAP_PROP_POS_FRAMES, pos);
            //cap.set(CAP_PROP_POS_FRAMES,pos+4);

         }
     }
       pos = cap.get(CAP_PROP_POS_FRAMES); //获取当前视频帧所在位置


    system("PAUSE");
    return 0;
}
