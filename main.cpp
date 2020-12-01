#include <iostream>
#include <opencv2/opencv.hpp>
//#include <calib.h>
#include "serial.h"
//#include "detect.h"

using namespace cv;
using namespace std;
//using namespace Detect;

//#define VIDEO ;
//#define SINGAL ;
#define SERIAL ;

char buff[20];

int main()
{

#ifdef SERIAL
    senddatafram S_data;
    senddatafram* p;
    p= &S_data;

    receivedatafram R_data;
    receivedatafram* q;
    q= &R_data;

    int fd=openPort("/dev/ttyUSB3");
    configurePort(fd);

    //Start 0x66
    p ->dis_in_picture = 1;
    p ->shape = 0x02;
    p ->length = 10;
    // null_ 0
    //End 0x88
    sendData(fd,(char*)p,sizeof(S_data));

    //cout<<sizeof(R_data);

//    cout<<q->rate<<endl;
//    cout<<q->distance<<endl;
//    cout<<q->angle<<endl;
//    cout<<q->end<<endl;
    while(true){

        if (reciveData(fd,q,sizeof(R_data))){
//            cout<<q->rate<<endl;
//            cout<<q->distance<<endl;
//            cout<<q->angle<<endl;
//            cout<<q->end<<endl;

            memset(q,0,sizeof(R_data));
            //sleep(5);
        }
        else{
            cout<<"recive data ERROR"<<endl;
        }
    }

#endif

#ifdef VIDEO

    Detect fi;
    senddatafram data;
    receivedatafrom re;
    Mat src;
    VideoCapture cap(0);
        cap.set(CAP_PROP_FRAME_WIDTH, 1080);
        cap.set(CAP_PROP_FRAME_HEIGHT, 720);
        cap.set(CAP_PROP_FPS,100 );
    int fd = openPort("/dev/ttyUSB0");
    configurePort(fd);



//   unsigned char a[20]={0};


//   int readCount = 0;

   senddatafram* p;
   p=&data;


   fi.MOVE_FLAG=false;
   fi.MODE_FLAG=true;
   while(cap.isOpened())
   {
        cap>>fi.src;
        if(fi.MODE_FLAG){
           fi.go();
           fi.TranMsg(p);
           sendData(fd,(char*)p,sizeof(data));
        }
        else{
           bool flag=fi.Judge();
           if(flag){
                cout<<"3D"<<endl;
            }
           else{
                fi.go();
            }
        }

        tcflush(fd, TCIFLUSH);

        if (waitKey(1) == 'q') break;
    }
    cap.release();
    destroyAllWindows();


//        receivedatafrom* q;
//        q  = & re;


//        memset(&re,0,sizeof(re));//clean

//        cout<<recive(fd,q,sizeof(re))<<endl;

//        int readCount = 0;
//        while (readCount < int(sizeof(re)))
//        {

//            int onceReadCount;
//            try
//            {
//                onceReadCount = read(fd, ((unsigned char *)(&re)) + readCount, sizeof(re) - readCount);
//            }
//            catch(exception e)
//            {
//            }

//            readCount += onceReadCount;
//        }


#endif
#ifdef SINGAL
    VideoCapture cap(0);
    while(cap.isOpened()){
        cap>>fi.src;
        //imshow("yy",fi.src);
        //fi.preprocess(fi.src);
        fi.go();
        if (waitKey(1) == 'q') break;
    }
    cap.release();
    destroyAllWindows();
#endif
    return 0;

}
