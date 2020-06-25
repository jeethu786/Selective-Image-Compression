
#include <iostream>
# include <fstream>
#include "compress.cpp"
#include <vector>
#include <string>
#include "Bitmap.cpp"
#include "DWT.cpp"
#include "Huffman.cpp"
#include "bitfile.cpp"
#include <cstring>
#include <cstdio>


using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
  Mat img=imread(argv[1], IMREAD_UNCHANGED);
  Mat img1=imread(argv[2], IMREAD_UNCHANGED);
  
  resize(img1,img1,Size(512,512));
  resize(img,img,Size(512,512));

  cvtColor(img,img, COLOR_BGR2HSV);
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
  //cout<<"ok3"<<endl;
  bitwise_not(mask3,mask_inv);

  imwrite("maskcpp.jpg",mask3);
  
  bitwise_and(img1,img1,res,mask3);
  //bitwise_and(img1,img1,res1,mask2);
  //cout<<"ok4"<<endl;
  
  imwrite("maskinvcpp.jpg",mask_inv);
  bitwise_and(img1,img1,res_inv,mask_inv);
  
   medianBlur(res,cc,1);
   medianBlur(res_inv,cc1,1);
   //we will  obtain two different images 
  imwrite("llcpp.jpg",cc);//lossless image 
  imwrite("lcpp.jpg",cc1);//lossy image
   
  /////////////////////////////////////////////////////////lossy
        Compressor c,c1,c2;
        Compressor d,d1,d2;
        string lossy_inp="lcpp.jpg";
        string lossless_inp="llcpp.jpg";
        Mat3b image1 = imread(lossy_inp, 1);

        Mat bgr[3];
        split(image1,bgr);
    	//Mat1b image1_gray,image2;
    	//cvtColor(image1, image1_gray, COLOR_BGR2GRAY);
        resize(bgr[0],bgr[0],Size(3840,2400));
        resize(bgr[1],bgr[1],Size(3840,2400));
        resize(bgr[2],bgr[2],Size(3840,2400));
        imwrite("bl.jpg",bgr[0]);
        imwrite("gn.jpg",bgr[1]);
        imwrite("rd.jpg",bgr[2]);
        
       
    	c.compress("bl.jpg");
	d.decompress("bl.compressed");

        c1.compress("gn.jpg");
	d1.decompress("gn.compressed");  
 
        c2.compress("rd.jpg");
	d2.decompress("rd.compressed");

        
        
        vector<Mat> mats;
        Mat m1,m2,m3,result;
        m1=imread("bl.cmp.jpg",0);
        m2=imread("gn.cmp.jpg",0);
        m3=imread("rd.cmp.jpg",0);
    	
        mats.push_back(m1);
        mats.push_back(m2);
        mats.push_back(m3); 
        //merge the channels together again
        merge(mats,result);
        resize(result,result,Size(720,512));
        //imwrite("result.jpg",result);
        cout<<"lossy done"<<endl;
       ////////////////////////////////////////////////////////////lossless
        Mat3b image2 = imread(lossless_inp, IMREAD_UNCHANGED);
        Mat bgr1[3];
        split(image2,bgr1);

        unsigned int bpp = 16;
        resize(bgr1[0],bgr1[0],Size(3840,2400));
        resize(bgr1[1],bgr1[1],Size(3840,2400));
        resize(bgr1[2],bgr1[2],Size(3840,2400));
 
      
        imwrite("lossless_bl.bmp",bgr1[0]);
        imwrite("lossless_gn.bmp",bgr1[1]);
        imwrite("lossless_rd.bmp",bgr1[2]);


        string in_put1="lossless_bl.bmp";
        string in_put2="lossless_gn.bmp";
        string in_put3="lossless_rd.bmp";

        string out_put1="lossless_cmp_bl.bmp";
        string out_put2="lossless_cmp_gn.bmp";
        string out_put3="lossless_cmp_rd.bmp";

        string output="middle";
        
        ////////////////////////////////////////////////////compress
        Bitmap *src1 = new Bitmap();
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
        m1_1=imread("lossless_cmp_bl.bmp",0);
        m2_1=imread("lossless_cmp_gn.bmp",0);
        m3_1=imread("lossless_cmp_rd.bmp",0);
    	//cvtColor(m1, m1, COLOR_BGR2GRAY);
    	//cvtColor(m2, m2, COLOR_BGR2GRAY);
    	//cvtColor(m3, m3, COLOR_BGR2GRAY);
        mats1.push_back(m1_1);
        mats1.push_back(m2_1);
        mats1.push_back(m3_1); 

        merge(mats1,result_1);
        
        resize(result_1,result_1,Size(720,512));
        
        //imwrite("result.jpg",result);
        //merege lossy and lossless color image
        addWeighted(result,1.01,result_1,1,10,c_output);
        //addWeighted(res,1,res_inv,1,30,c_output1);
        medianBlur(c_output,cc2,3);
        medianBlur(cc2,cc3,3);
        imwrite("result.jpg",cc3);
        //imwrite("gud.jpg",c_output1);
        cout << "all done" <<endl;
   waitKey();
   return 0;
}
