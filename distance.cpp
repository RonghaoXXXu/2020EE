//#include <distance.h>
#include <detect.h>
#include <serial.h>

float compute_(Point2f center, Point2f core){

    float disp = center.x - core.x;//if disp>0,turn left
    return disp;

}

void Detect::TranMsg(senddatafram *q){

    senddatafram aa = *q;

    int b0 = RECT.size;
    int b1 = CIRCLE.d;
    int b2 = TRIA.lenth;

    int c = 10*(src_center.x - core.x);

    q ->dis_in_picture = c;
    cout<<c<<"      "<<aa.dis_in_picture<<endl;

    q -> shape = 0x00;
    q ->length = 0;

    switch (type) {
    case 0:
        q -> shape = 0x00;
        break;
    case 1:
        q -> shape = 0x01;


        break;
    case 2:
        q -> shape = 0x02;

        q ->length = b1;
        break;
    case 3:
        q -> shape = 0x03;

        q ->length = b2;
        break;
    default:
        cout<<"ERROR!NO USEABLE SHAPE_FLAG!"<<endl;
        break;
    }

        //q = & aa;
}
