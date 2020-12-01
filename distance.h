#ifndef SHAPEIDENTIFY_H
#define SHAPEIDENTIFY_H

//#include <detect.h>
//#include <serial.h>
#endif





//

/***
 // 畸变参数
static double k1, k2, p1, p2;

// 内参
static double fx, fy, cx , cy ;void readFile()
{
    FileStorage fsread("../cameraParama.xml", FileStorage::READ);

    fsread["camera-matrix"] >> CAM_MATRIX;
    fsread["distortion"] >> DISTORTION_COEFF;

    k1 = DISTORTION_COEFF.at<double>(0,0);
    k2 = DISTORTION_COEFF.at<double>(1,0);
    p1 = DISTORTION_COEFF.at<double>(2,0);
    p2 = DISTORTION_COEFF.at<double>(1,0);


    fx = CAM_MATRIX.at<double>(0,0);
    fy = CAM_MATRIX.at<double>(1,1);
    cx = CAM_MATRIX.at<double>(0,2);
    cy = CAM_MATRIX.at<double>(1,2);

    Rect point = Detect::ROI;//(!)
}
Point2f point_change (Point2f tested){
    for (int v = point.tl().y; v < point.br().y; v++){
        for (int u = point.tl().x; u < point.br().x; u++) {

            double u_distorted = 0, v_distorted = 0;
            double x1,y1,x2,y2;
            x1 = (u-cx)/fx;
            y1 = (v-cy)/fy;

            double r2;
            r2 = pow(x1,2)+pow(y1,2);
            x2  = x1*(1+k1*r2+k2*pow(r2,2))+2*p1*x1*y1+p2*(r2+2*x1*x1);
            y2 = y1*(1+k1*r2+k2*pow(r2,2))+p1*(r2+2*y1*y1)+2*p2*x1*y1;

            u_distorted = fx*x2+cx;
            v_distorted = fy*y2+cy;

            if (u_distorted == tested.x && v_distorted == tested.y ) {
                float u = u;
                float v = v;

                return Point2f(u, v);

           }

        }
    }

}
void temp1(){
    dis_yaw = point_change(light).x - point_change(center).x;

}
***/


