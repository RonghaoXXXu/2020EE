#ifndef DETECT_H
#define DETECT_H

#pragma once

#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <serial.h>

using namespace cv;
using namespace std;

#endif // DETECT_H

#define pi 3.1415926 ;
#define CV_SHOW ;
//#define Use_Perspective ;


class Detect
{


public:
    Detect() {

    }

    Mat src;//原图
    Mat src_show;//显示图
    Mat edge;//边缘图


    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    Point2f PA,PB,PC,PD;//中点

    //动：判断3D，2D；一直算
    //不动：直接2D；算平均值
    struct RECT{
        bool flag;
        double size;
        //double y;
        double k;
    }RECT;
    vector<double> size;

    struct CIRCLE{
        bool flag;
        double d;
        double k;
    }CIRCLE;
    vector<double> d;

    struct TRIA{
        bool flag;
        double k;
        double lenth;
    }TRIA;
    vector<double> lenth;

/******ThreeD*******/
    struct Basket{
        bool flag;
        double diameter;
    }Basket;
    struct Volley{
        bool flag;
        double diameter;
    }Volley;
    struct Foot{
        bool flag;
        double diameter;
    }Foot;

    bool MOVE_FLAG;//动为true
    bool MODE_FLAG;//模式 1直接2D；模式2 判断2D，3D；
    double Threshold=50;//激光阈值
    double max=130,min=100;//3D阈值
/*****W.*****/
    Rect ROI;//Rect
    Point2f core;//物体重心点位置
    Point2f src_center;// 图片中心点
    Point2f wave_center;//激光点位置
    double Diameter;//3D直径
    int type=0;//1:矩形，2：圆形，3：三角形
    //float diff;//差值,|物体重心点.y-激光点位.y|
/***********/
    Point2f midpoint(Point2f Pa,Point2f Pb);
    double getDistence(Point2f Pa,Point2f Pb);
    double mean_Value(vector<double> vec);

    void preprocess(Mat img);//处理图片中心，激光位置
    Mat  fit_color();
    void Contour_process(Mat img);
    void fit_contour(vector<Point>contt);
    void Wave();
    void end_result();

    void ThreeD();
    bool Judge();


    void TranMsg(senddatafram *q);

    void go()
    {
        preprocess(src);
        Contour_process(edge);
        //end_result();
    }

};

