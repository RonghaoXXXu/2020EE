/*******************************************************************************************************************
Copyright 2017 Dajiang Innovations Technology Co., Ltd (DJI)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
documentation files(the "Software"), to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense, and / or sell copies of the Software, and
to permit persons to whom the Software is furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all copies or substantial portions of
the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.
*******************************************************************************************************************/


#include "serial.h"
//开串口,配置串口，接收，发送。


//#include "distance.h"
#include <iostream>>
using namespace std;

int openPort(const char * dev_name){
    int fd; // file description for the serial port
    fd = open(dev_name, O_RDWR | O_NOCTTY | O_NDELAY );
    //返回值：成功则返回文件描述符，否则返回-1

    if(fd == -1){ // if open is unsucessful
        printf("open_port: Unable to open /dev/ttyS0. \n");
    }
    else  {
        fcntl(fd, F_SETFL, 0);//恢复串口阻塞状态。
        printf("port is open.\n");
    }
    return(fd);
}

int configurePort(int fd){
    // configure the port
    // 起始位，数据位，（校验位），停止位；空闲位。
    struct termios port_settings;               // structure to store the port settings in
    cfsetispeed(&port_settings, B460800);          // set baud rates
    cfsetospeed(&port_settings, B460800);

    port_settings.c_cflag &= ~PARENB;   //无奇偶校验位     pip install git+https://github.com/onnx/tensorflow-onnx // set no parity, stop bits, data bits
    port_settings.c_cflag &= ~CSTOPB;   //停止位
    port_settings.c_cflag &= ~CSIZE;    //设置字符大小
    port_settings.c_cflag |= CS8;       //设置停止位
    //port_settings.c_cflag &= ~ICANON;
    //举例来说，假设传输的数据位为01001100，如果是奇校验，则奇校验位为0（要确保总共有奇数个1）
    //如果是偶校验，则偶校验位为1（要确保总共有偶数个1）。

    tcsetattr(fd, TCSANOW, &port_settings);     // apply the settings to the port
    return(fd);
}

//bool send(int fd, double * info){

//    if(NULL == info){
//        if (8 == write(fd, DisPoint::send_bytes, 8))  //Send data
//            return true;
//        return false;
//    }

//    short * data_ptr = (short *)(DisPoint::send_bytes + 1);
//    data_ptr[0] = (short)info[0];
//    data_ptr[1] = (short)info[1];
//    data_ptr[2] = (short)info[2];
//    if (8 == write(fd, DisPoint::send_bytes, 8))      //Send data
//        return true;
//    return false;
//}

bool sendData(int fd, char * data, int size){

    if (size == write(fd, data, size)){
        printf("send Data success\n");
        return true;
    }
    return false;
}
bool reciveData (int fd,receivedatafram* q,int size){

    while(read(fd, q, size)!=size){
    }

    if(q->start==0x66                                                                                      ){
        printf("Recive Success\n");
//            cout<<endl;
        printf("%d\n", q->start);
        printf("%d", q->rate);
        return true;
    }

}
