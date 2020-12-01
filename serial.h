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
#ifndef SERIAL_H
#define SERIAL_H
#pragma once
#include <math.h>
#include <stdio.h>      // standard input / output functions
#include <string.h>     // string function definitions
#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>    // POSIX terminal control definitionss
//#include <iostream>

#include"stdint.h"
#endif // SHAPEIDENTIFY_H
#pragma pack (1)

//enum DATATYPE { IMAGE, INFO};

struct senddatafram
{
    uint8_t start=0x66;
    uint8_t shape;
//    short dis_in_picture;
//    short lenth;
    int16_t dis_in_picture;
    int16_t length;
    uint8_t null_ = 0;
    uint8_t end=0x88;
};

struct receivedatafram
{
    uint8_t start;
    uint8_t rate;
    short distance;
    short angle;
    uint8_t end;
};


int openPort(const char * dev_name);

int configurePort(int fd);

bool sendData(int fd, char * data, int size);

bool reciveData(int fd,receivedatafram* q,int size);




