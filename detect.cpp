#include"detect.h"


#define CV_SHOW ;
Point2f Detect::midpoint(Point2f Pa, Point2f Pb){
    return Point2f((Pa.x+Pb.x)*0.5,(Pa.y+Pb.y)*0.5);
}

double Detect::getDistence(Point2f Pa, Point2f Pb){
    double dis;
    dis=sqrt(powf((Pa.x-Pb.x),2)+powf((Pa.y-Pb.y),2));
    return dis;
}

double Detect::mean_Value(vector<double> vec){
    double mean=0;
    if(!MOVE_FLAG){
        unsigned int i;
        for (i=0; i <vec.size(); i++) {
            mean +=vec[i];
        }
    }
    mean=mean/vec.size();
    return mean;
}

void Detect::preprocess(Mat img){
    src_show =img.clone();//过程图
    src_center=Point2f((img.rows/2),(img.cols/2));
    Wave();

    Mat img2;
    img2=fit_color();
    //cvtColor(img2,img2,COLOR_BGR2GRAY);
    medianBlur(img2,img2,9);
    GaussianBlur(img2,img2,Size(7,7),0);
#ifdef CV_SHOW
    imshow("gray",img2);
//    waitKey(0);
#endif
    Mat element=getStructuringElement(MORPH_RECT,Size(15,15));
    //morphologyEx(img2,img2,MORPH_OPEN,element);

    medianBlur(img2,img2,15);
    Canny(img2,edge,50,100);
    dilate(edge,edge,element);
    erode(edge,edge,element);
#ifdef CV_SHOW
    imshow("edge",edge);
//    waitKey(0);
#endif
}

void Detect::Contour_process(Mat img){
//    Mat dst=img.clone();
    findContours(img,contours,hierarchy,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
    //drawContours(dst,contours, -1, (0,0,255), 3, 8, hierarchy,0);
    for(unsigned i=0;i<contours.size();i++){
        if (contourArea(contours[i])<1000)
            break;

        RotatedRect rec=minAreaRect(contours[i]);
        Rect rec1=boundingRect(contours[i]);

        ROI=rec1;
        rectangle(src_show,rec1,(0,0,0),2);

        Point2f points[4];
        rec.points(points);

        PA=midpoint(points[0],points[1]);
        PB=midpoint(points[1],points[2]);
        PC=midpoint(points[2],points[3]);
        PD=midpoint(points[3],points[0]);

        circle(src_show,PA,5,(0,0,255));
        circle(src_show,PB,5,(0,0,255));
        circle(src_show,PC,5,(0,0,255));
        circle(src_show,PD,5,(0,0,255));

        line(src_show,PA,PC,(0,255,0),2);
        line(src_show,PB,PD,(0,255,0),2);

        vector<Point> approx;
        double epsilon = 0.001 * arcLength(contours[i],true);
        approxPolyDP(contours[i], approx, epsilon, true);

        fit_contour(approx);
        approx.clear();
        end_result();

        RECT.flag=false;
        TRIA.flag=false;
        CIRCLE.flag=false;
    }
#ifdef CV_SHOW
    imshow("Contours",src_show);
//      waitKey(0);
#endif

}

void Detect::fit_contour(vector<Point>contt){
    float radius;
    Point2f center;
    minEnclosingCircle(contt,center,radius);
    double area_circle=3.1415926*powf(radius,2);

    vector<Point>triangle;
    double area_triangle= minEnclosingTriangle(contt,triangle);

    RotatedRect rect=minAreaRect(contt);
    double area_rect=rect.size.area();

    RECT.k=area_rect/contourArea(contt);
    CIRCLE.k=area_circle/contourArea(contt);
    TRIA.k=area_triangle/contourArea(contt);

    vector<double>ks;
    ks.push_back(RECT.k);
    ks.push_back(CIRCLE.k);
    ks.push_back(TRIA.k);
    std::sort(ks.begin(),ks.end());

    //分析几何形状
    if(ks[0]==RECT.k){
        RECT.flag=true;
        size.push_back((getDistence(PB,PD)+getDistence(PA,PC))*0.5);
        core=((PA+PC)/2+(PB+PD)/2)/2;
        RECT.size=mean_Value(size);

        type=1;

    }
    else if(ks[0]==CIRCLE.k){
        CIRCLE.flag=true;
        d.push_back((2*radius+getDistence(PA,PC)+getDistence(PB,PD))/3);
        core=center;
        CIRCLE.d=mean_Value(d);

        type=2;
    }
    else if(ks[0]==TRIA.k){
        TRIA.flag=true;
        lenth.push_back((getDistence(triangle[0],triangle[1])+getDistence(triangle[1],triangle[2])+getDistence(triangle[2],triangle[0]))/3);
        core=Point2f((triangle[0].x+triangle[1].x+triangle[2].x)/3,((triangle[0].y+triangle[1].y+triangle[2].y)/3));
        TRIA.lenth=mean_Value(lenth);

        type=3;
    }
    circle(src_show,core,2,(0,0,0),4);
    //diff=abs(core.y-wave_center.y);
}

Mat Detect::fit_color(){
    Mat temp=src.clone();

    Mat test;
    cvtColor(temp,test,COLOR_BGR2HSV);

    Mat blue_mask,red_mask,green_mask;
    inRange(test,Scalar(81,43,46),Scalar(128,255,255),blue_mask);
    inRange(test,Scalar(156,43,46),Scalar(180,255,255),red_mask);
    inRange(test,Scalar(35,43,46),Scalar(77,255,255),green_mask);

    Mat result;
    bitwise_or(red_mask,green_mask,result);
    bitwise_or(result,blue_mask,result);

    //cvtColor(temp,temp,COLOR_BGR2GRAY);
    //bitwise_and(result,temp,result);
    Mat color;
    src.copyTo(color,result);
#ifdef CV_SHOW
    imshow("COLOR",color);
//    waitKey(0);7
#endif
    return color;
}

void Detect::Wave(){
    Mat img2;
    cvtColor(src,img2,COLOR_BGR2GRAY);

    double sumval;
    int i,j;
    MatIterator_<uchar> it,end;

    for(it=img2.begin<uchar>(),end=img2.end<uchar>();it!=end;it++){
        ((*it)>Threshold)? sumval+=(*it) :NULL;
    }

    double m=0,n=0;
    for(i=0;i<img2.cols;i++){
        for(j=0;j<img2.rows;j++){
            double s=img2.at<uchar>(j,i);
            if(s<Threshold)
                s=0;
            m+=i*s/sumval;
            n+=j*s/sumval;
        }
    }
    wave_center=Point2f(cvRound(m),cvRound(n));
}

bool Detect::Judge(){
    Mat img=src.clone();
    const char* classNames[]= {"background", "person", "bicycle", "car", "motorcycle", "airplane", "bus", "train", "truck", "boat", "traffic light",
    "fire hydrant", "background", "stop sign", "parking meter", "bench", "bird", "cat", "dog", "horse", "sheep", "cow", "elephant", "bear", "zebra", "giraffe", "background", "backpack",
    "umbrella", "background", "background", "handbag", "tie", "suitcase", "frisbee","skis", "snowboard", "sports ball", "kite", "baseball bat","baseball glove", "skateboard", "surfboard", "tennis racket",
    "bottle", "background", "wine glass", "cup", "fork", "knife", "spoon","bowl", "banana",  "apple", "sandwich", "orange","broccoli", "carrot", "hot dog",  "pizza", "donut",
    "cake", "chair", "couch", "potted plant", "bed", "background", "dining table", "background", "background", "toilet", "background","tv", "laptop", "mouse", "remote", "keyboard",
    "cell phone", "microwave", "oven", "toaster", "sink", "refrigerator", "background","book", "clock", "vase", "scissors","teddy bear", "hair drier", "toothbrush"};

    String weights = "/home/ronghao/Desktop/2020ELE/ELE/frozen_inference_graph.pb";
    String prototxt = "/home/ronghao/Desktop/2020ELE/ELE/graph.pbtxt";
    dnn::Net net=dnn::readNetFromTensorflow(weights,prototxt);

    Mat blob=dnn::blobFromImage(img,1,Size(300,300));
    net.setInput(blob);
    Mat output=net.forward();

    Mat detectionMat(output.size[2], output.size[3], CV_32F, output.ptr<float>());

    float thresold=0.8;
    for (int i = 0; i < detectionMat.rows; i++)
    {
        float confidence = detectionMat.at<float>(i, 2);

        if(confidence>thresold)
        {
            size_t objectClass = (size_t)(detectionMat.at<float>(i, 1));
            if(classNames[objectClass]=="sports ball"){
                return true;
            }
            else return false;

        }
        else return false;
    }
}

void Detect::ThreeD(){
    Mat img_=src.clone();
    Mat harris;
    Mat edge_;

    cvtColor(img_,img_,COLOR_BGR2GRAY);
    medianBlur(img_,img_,7);

    GaussianBlur(img_,img_,Size(7,7),0);
    Mat element=getStructuringElement(MORPH_RECT,Size(7,7));
    //morphologyEx(img2,img2,MORPH_OPEN,element);

    Canny(img_,edge_,50,100);
    dilate(edge_,edge_,element);
    erode(edge_,edge_,element);

#ifdef CV_SHOW
    imshow("3D",edge_);
//    waitKey(0);
#endif
    vector<vector<Point>> contours_;
    vector<Vec4i> hierarchy_;

    findContours(edge_,contours_,hierarchy_,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE,Point(0, 0));
    float radius_;
    Point2f center_;

    for(unsigned i=0;i<contours_.size();i++){
        if (contourArea(contours_[i])<100)
            break;
        //RotatedRect rec_=minAreaRect(contours[i]);
        Rect rec1_=boundingRect(contours_[i]);
        rectangle(img_,rec1_,(0,0,0),2);

        minEnclosingCircle(contours_[i],center_,radius_);
        Diameter=2*radius_;

    }
#ifdef CV_SHOW
    imshow("3D_edge",img_);
//    waitKey(0);
#endif
    cornerHarris(img_,harris,2,3,0.04);
#ifdef CV_SHOW
    imshow("3D_Harris",harris);
//    waitKey(0);
#endif
}

void Detect::end_result(){

    if(RECT.flag){
        cout<<"类型："<<"正方形"<<endl;
        cout<<"边长："<<RECT.size<<endl;
    }
    else if(CIRCLE.flag){
        cout<<"类型："<<"圆"<<endl;
        cout<<"直径："<<CIRCLE.d<<endl;
    }

    else if(TRIA.flag){
        cout<<"类型："<<"正三角形"<<endl;
        cout<<"边长："<<TRIA.lenth<<endl;
    }//basketball 4, soccer 5, volleyball 6

}
