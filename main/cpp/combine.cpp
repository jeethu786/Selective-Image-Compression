#include <jni.h>
#include <string>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include "compress.cpp"
#include <vector>
#include "Bitmap.h"
#include "DWT.cpp"
#include "bitfile.h"
#include <cstring>
#include <cstdio>
#include <deque>
#include <stdio.h>
#include <stdlib.h>


using namespace cv;
using namespace std;

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_imagecompressor_MainActivity_ImageFromJNI(JNIEnv *env, jobject thiz, jlong src,jlong dest,jstring des) {
    string hello = "Hello from C++";

    const char *path= env->GetStringUTFChars(des,0) ;
    string des1 =  path;
    Mat &img=*(Mat*)src;
    Mat &out=*(Mat*)dest;
    Mat img1=img;
    int i= img1.cols;
    int j= img1.rows;
    int m=rand()%5+1;
    int f,f1;
        f= i/1.8;
        f1=j/1.8;

    resize(img,img,Size(f,f1));
    resize(img1,img1,Size(f,f1));
    Mat jee=img1;
    Mat jee1,jee2;
    cvtColor(img,img,COLOR_RGB2HSV);
    int low_H=0,low_S=110,low_V=110;
    int high_H=5,high_S=255,high_V=255;
    int low_H1=170,low_S1=60,low_V1=60;
    int high_H1=180,high_S1=255,high_V1=255;
    Mat mask1,mask2;
    //cout<<"ok"<<endl;

    inRange(img, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V),mask1);
    inRange(img, Scalar(low_H1, low_S1, low_V1), Scalar(high_H1, high_S1, high_V1),mask2);
    //cout<<"ok2"<<endl;

    Mat mask3,res,res1,mask_inv,res_inv,lossless,cc,cc1;
    mask3=mask1+mask2;
    medianBlur(jee,jee1,3);

    //cout<<"ok3"<<endl;
    bitwise_not(mask3,mask_inv);

    //imwrite("maskcpp.jpg",mask3);

    bitwise_and(img1,img1,res,mask3);

    //bitwise_and(img1,img1,res1,mask2);
    //cout<<"ok4"<<endl;

    //imwrite("maskinvcpp.jpg",mask_inv);
    bitwise_and(img1,img1,res_inv,mask_inv);
    medianBlur(jee1,jee2,3);
    //addWeighted(res,0.5,res1,0.5,0.0,lossless);
    //namedWindow( "lossless", WINDOW_NORMAL );// Create a window for display.
    //namedWindow( "lossy", WINDOW_NORMAL );
    medianBlur(res,cc,1);
    medianBlur(res_inv,cc1,1);

    imwrite(des1+"llcpp.jpg",cc);

    imwrite(des1+"lcpp.jpg",cc1);
    /////////////////////////////////////////////////////////lossy
    Compressor c,c1,c2;
    Compressor d,d1,d2;

    string lossy_inp=des1+"lcpp.jpg";
    string lossless_inp=des1+"llcpp.jpg";
    Mat3b image1 = imread(lossy_inp,IMREAD_COLOR);

    Mat bgr[3];

    split(cc,bgr);
    //Mat1b image1_gray,image2;
    //cvtColor(image1, image1_gray, COLOR_BGR2GRAY);
    resize(bgr[0],bgr[0],Size(3840,2400));
    resize(bgr[1],bgr[1],Size(3840,2400));
    resize(bgr[2],bgr[2],Size(3840,2400));

    imwrite(des1+"bl.jpg",bgr[0]);
    imwrite(des1+"gn.jpg",bgr[1]);
    imwrite(des1+"rd.jpg",bgr[2]);

    /*
    c.compressing(des1+"bl.jpg");
    d.decompressing(des1+"bl.compressed");

    c1.compressing(des1+"gn.jpg");
    d1.decompressing(des1+"gn.compressed");

    c2.compressing(des1+"rd.jpg");
    d2.decompressing(des1+"rd.compressed");



    vector<Mat> mats;
    Mat m1,m2,m3,result;
    m1=imread(des1+"bl.cmp.jpg",0);
    m2=imread(des1+"gn.cmp.jpg",0);
    m3=imread(des1+"rd.cmp.jpg",0);
    //cvtColor(m1, m1, COLOR_BGR2GRAY);
    //cvtColor(m2, m2, COLOR_BGR2GRAY);
    //cvtColor(m3, m3, COLOR_BGR2GRAY);
    mats.push_back(m1);
    mats.push_back(m2);
    mats.push_back(m3);

    merge(mats,result);

    resize(result,result,Size(720,512));
    //imwrite("result.jpg",result);
    //cout<<"lossy done"<<endl;
     */
    ////////////////////////////////////////////////////////////lossless
    Mat3b image2 = imread(lossless_inp, IMREAD_COLOR);
    Mat bgr1[3];

    split(cc1,bgr1);
    unsigned int bpp = 16;
    resize(bgr1[0],bgr1[0],Size(3840,2400));
    resize(bgr1[1],bgr1[1],Size(3840,2400));
    resize(bgr1[2],bgr1[2],Size(3840,2400));


    imwrite("lossless_bl.bmp",bgr1[0]);
    imwrite(des1+"lossless_gn.bmp",bgr1[1]);
    imwrite(des1+"lossless_rd.bmp",bgr1[2]);


    string in_put1="lossless_bl.bmp";
    string in_put2=des1+"lossless_gn.bmp";
    string in_put3=des1+"lossless_rd.bmp";

    string out_put1=des1+"lossless_cmp_bl.bmp";
    string out_put2=des1+"lossless_cmp_gn.bmp";
    string out_put3=des1+"lossless_cmp_rd.bmp";

    string output=des1+"middle";
    out=jee2;

    ////////////////////////////////////////////////////compress
    Bitmap *src1 = new Bitmap();
    //Mat jeeth=imread("lena.bmp",0);
    /*
    src1->readImage(in_put1);
    DWT *dwt1 = new DWT(src1, bpp);
    dwt1->transform();
    dwt1->save(output);

    delete src1;
    delete dwt1;


    ////decompress
    DWT *dwt_1 = new DWT(bpp);
    dwt_1->load(output);
    dwt_1->untrasform();
    Bitmap *out_1 = dwt_1->toBitmap();
    out_1->writeImage(out_put1);
    delete dwt_1;
    delete out_1;

    ///////////////////////////////////////////////////compress
    Bitmap *src2 = new Bitmap();
    src2->readImage(in_put2);
    DWT *dwt2 = new DWT(src2, bpp);
    dwt2->transform();
    dwt2->save(output);
    delete src2;
    delete dwt2;


    ////decompress
    DWT *dwt_2 = new DWT(bpp);
    dwt_2->load(output);
    dwt_2->untrasform();
    Bitmap *out_2 = dwt_2->toBitmap();
    out_2->writeImage(out_put2);
    delete dwt_2;
    delete out_2;

    /////////////////////////////////////////////////////compress
    Bitmap *src3 = new Bitmap();
    src3->readImage(in_put3);
    DWT *dwt3 = new DWT(src3, bpp);
    dwt3->transform();
    dwt3->save(output);
    delete src3;
    delete dwt3;


    ////decompress
    DWT *dwt_3 = new DWT(bpp);
    dwt_3->load(output);
    dwt_3->untrasform();
    Bitmap *out_3 = dwt_3->toBitmap();
    out_3->writeImage(out_put3);
    delete dwt_3;
    delete out_3;

    vector<Mat> mats1;
    Mat m1_1,m2_1,m3_1,result_1,c_output,cc2,cc3;
    m1_1=imread(des1+"lossless_cmp_bl.bmp",0);
    m2_1=imread(des1+"lossless_cmp_gn.bmp",0);
    m3_1=imread(des1+"lossless_cmp_rd.bmp",0);
    //cvtColor(m1, m1, COLOR_BGR2GRAY);
    //cvtColor(m2, m2, COLOR_BGR2GRAY);
    //cvtColor(m3, m3, COLOR_BGR2GRAY);
    mats1.push_back(m1_1);
    mats1.push_back(m2_1);
    mats1.push_back(m3_1);

    merge(mats1,result_1);

    resize(result_1,result_1,Size(720,512));

    //imwrite("result.jpg",result);

    addWeighted(result,1.01,result_1,1,10,c_output);
    //addWeighted(res,1,res_inv,1,30,c_output1);
    medianBlur(c_output,cc2,3);
    //medianBlur(cc2,outputMat,3);
    medianBlur(cc2,cc3,3);
    out=cc3;
    imwrite(des1+"result.jpg",1,cc3);
    //imwrite("gud.jpg",c_output1);
    //cout << "all done" <<endl;
   // waitKey();
*/

  return env->NewStringUTF(des1.c_str());
}
