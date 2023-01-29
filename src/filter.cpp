#include <opencv2/highgui.hpp>                                //including needed files
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
using namespace std;

int grayscale(cv::Mat &src,cv::Mat &dst){

    dst = cv::Mat::zeros(src.size(),CV_8U);

    for (int i = 0; i<src.rows; i++){
        cv::Vec3b *rptr = src.ptr<cv::Vec3b>(i);
        uchar *dptr = dst.ptr<uchar>(i);

        for(int j = 0; j<src.cols; j++){

            dptr[j] = 0.114*rptr[j][0] + 0.587*rptr[j][1] + 0.299*rptr[j][2];
            // the follwing conversion was found on opencv website on color conversion provided in the assignment
            // https://docs.opencv.org/3.4/de/d25/imgproc_color_conversions.html#color_convert_rgb_gray
        }
    }
    return 0;
}


int blur5x5(cv::Mat &src,cv::Mat &dst){ // gaussian blur

    cv::Mat temp;
    temp = cv::Mat::zeros(src.size(),src.type());
    int arr[5] = {1,2,4,2,1};
    int sum = 0;
    for (int i = 0;i<5;i++){
        if (arr[i]>=0){
            sum = sum + arr[i];
        }
    }
    //using separable gaussian blur method. First convolving the image row-wise
    for(int i = 0; i<src.rows; i++){            
        cv::Vec3b *rptr = src.ptr<cv::Vec3b>(i);
        cv::Vec3b *dptr = temp.ptr<cv::Vec3b>(i);

        for(int j = 0; j<src.cols; j++){

            for(int k = 0; k<3; k++){ //considering 3 color channel as well

                if(j == 0){
                    dptr[j][k] = (rptr[0][k]*arr[2] + rptr[1][k]*arr[3] + rptr[2][k]*arr[4])/sum; //edge case first/
                }
                else if(j == 1){
                    dptr[j][k] = (rptr[0][k]*arr[1] + rptr[1][k]*arr[2] + rptr[2][k]*arr[3] + rptr[3][k]*arr[4])/sum; // edge case second
                }
                else if(j == src.cols-1){
                    dptr[j][k] = (rptr[j][k]*arr[2] + rptr[j-1][k]*arr[1] + rptr[j-2][k]*arr[0])/sum; //edge case last row
                }
                else if(j == src.cols-2){
                    dptr[j][k] = (rptr[j+1][k]*arr[3] + rptr[j][k]*arr[2] + rptr[j-1][k]*arr[1] + rptr[j-2][k]*arr[0])/sum; //edge case secondlast row
                }
                else{
                    dptr[j][k] = (rptr[j+2][k]*arr[4] + rptr[j+1][k]*arr[3] + rptr[j][k]*arr[2] + rptr[j-1][k]*arr[1] + rptr[j-2][k]*arr[0])/sum;// remaining
                }
            }
        }
    }

    dst = cv::Mat::zeros(src.size(),src.type());

    for(int i = 0; i<src.rows; i++){ // Now convolving column-wise

        cv::Vec3b *rptrm2;
        cv::Vec3b *rptrm1;
        cv::Vec3b *rptr = temp.ptr<cv::Vec3b>(i);
        cv::Vec3b *rptrp1;
        cv::Vec3b *rptrp2;
        if(i>=2){
            rptrm2 = temp.ptr<cv::Vec3b>(i-2);
        }
        if(i>=1){
            rptrm1 = temp.ptr<cv::Vec3b>(i-1);
        }
        if(i<=src.rows-1){
            rptrp1 = temp.ptr<cv::Vec3b>(i+1);
        }
        if(i<=src.rows-2){
            rptrp2 = temp.ptr<cv::Vec3b>(i+2);
        }

        cv::Vec3b *dptr = dst.ptr<cv::Vec3b>(i);

        for(int j = 0; j<src.cols; j++){

            for(int k =0; k<3; k++){

                if(i == 0){
                    dptr[j][k] = (rptr[j][k]*arr[2] + rptrp1[j][k]*arr[3] + rptrp2[j][k]*arr[4])/sum;
                }
                else if(i == 1){
                    dptr[j][k] = (rptrm1[j][k]*arr[1] + rptr[j][k]*arr[2] + rptrp1[j][k]*arr[3] + rptrp2[j][k]*arr[4])/sum;
                }
                else if(i == src.rows-1){
                    dptr[j][k] = (rptr[j][k]*arr[2] + rptrm1[j][k]*arr[1] + rptrm2[j][k]*arr[0])/sum;
                }
                else if(i == src.rows-2){
                    dptr[j][k] = (rptrp1[j][k]*arr[3] + rptr[j][k]*arr[2] + rptrm1[j][k]*arr[1] + rptrm2[j][k]*arr[0])/sum;
                }
                else{
                    dptr[j][k] = (rptrp2[j][k]*arr[4] + rptrp1[j][k]*arr[3] + rptr[j][k]*arr[2] + rptrm1[j][k]*arr[1] + rptrm2[j][k]*arr[0])/sum;
                }
            }
        }
    }

    return 0;
}



int sobelX3x3( cv::Mat &src, cv::Mat &dst ){

    cv::Mat temp;
    temp = cv::Mat::zeros(src.size(),CV_16SC3);
    // separable filter for 3x3 sobelX
    //int ar1[3] = {1,2,1};
    //int ar2[3] = {-1,0,1};

    for(int i = 0; i<src.rows; i++){ //convolution row-wise
        cv::Vec3b *rptr = src.ptr<cv::Vec3b>(i);
        cv::Vec3s *dptr = temp.ptr<cv::Vec3s>(i);

        for(int j = 0; j<src.cols; j++){

            for(int k = 0; k<3; k++){

                if(j == 0){
                    dptr[j][k] = rptr[1][k];
                }
                else if(j == src.cols-1){
                    dptr[j][k] = -rptr[j-1][k];
                }
                else{
                    dptr[j][k] = rptr[j+1][k] - rptr[j-1][k];
                }
            }
        }
    }

    dst = cv::Mat::zeros(src.size(),CV_16SC3);

    for(int i = 0; i<src.rows; i++){ //convolution columbn-wise

        cv::Vec3s *rptrm1;
        cv::Vec3s *rptr = temp.ptr<cv::Vec3s>(i);
        cv::Vec3s *rptrp1;
        if(i>=1){
            rptrm1 = temp.ptr<cv::Vec3s>(i-1);
        }
        if(i<=src.rows-1){
            rptrp1 = temp.ptr<cv::Vec3s>(i+1);
        }

        cv::Vec3s *dptr = dst.ptr<cv::Vec3s>(i);

        for(int j = 0; j<src.cols; j++){

            for(int k =0; k<3; k++){

                if(i == 0){
                    dptr[j][k] = (rptr[j][k]*2 + rptrp1[j][k])/4;
                }
                else if(i == src.rows-1){
                    dptr[j][k] = (rptr[j][k]*2 + rptrm1[j][k])/4;
                }
                else{
                    dptr[j][k] = (rptrp1[j][k] + rptr[j][k]*2 + rptrm1[j][k])/4;
                }
            }
        }
    }
    return 0;
}

int sobelY3x3( cv::Mat &src, cv::Mat &dst ){
    
    cv::Mat temp;
    temp = cv::Mat::zeros(src.size(),CV_16SC3);
    // separable filter for sobelY 3x3
    //int ar1[3] = {1,0,-1};
    //int ar2[3] = {1,2,1};

    for(int i = 0; i<src.rows; i++){ //convolution row-swise
        cv::Vec3b *rptr = src.ptr<cv::Vec3b>(i);
        cv::Vec3s *dptr = temp.ptr<cv::Vec3s>(i);

        for(int j = 0; j<src.cols; j++){

            for(int k = 0; k<3; k++){

                if(j == 0){
                    dptr[j][k] = (rptr[0][k]*2 + rptr[1][k])/4;
                }
                else if(j == src.cols-1){
                    dptr[j][k] = (rptr[j][k]*2 + rptr[j-1][k])/4;
                }
                else{
                    dptr[j][k] = (rptr[j+1][k] + rptr[j][k]*2 + rptr[j-1][k])/4;
                }
            }
        }
    }

    dst = cv::Mat::zeros(src.size(),CV_16SC3);

    for(int i = 0; i<src.rows; i++){ //convolution column wise

        cv::Vec3s *rptrm1;
        cv::Vec3s *rptr = temp.ptr<cv::Vec3s>(i);
        cv::Vec3s *rptrp1;
        if(i>=1){
            rptrm1 = temp.ptr<cv::Vec3s>(i-1);
        }
        if(i<=src.rows-1){
            rptrp1 = temp.ptr<cv::Vec3s>(i+1);
        }

        cv::Vec3s *dptr = dst.ptr<cv::Vec3s>(i);

        for(int j = 0; j<src.cols; j++){

            for(int k =0; k<3; k++){

                if(i == 0){
                    dptr[j][k] = -rptrp1[j][k];
                }
                else if(i == src.rows-1){
                    dptr[j][k] = rptrm1[j][k];
                }
                else{
                    dptr[j][k] = -rptrp1[j][k] + rptrm1[j][k];
                }
            }
        }
    }
    return 0;
}

int magnitude( cv::Mat &sx, cv::Mat &sy, cv::Mat &dst ){

    cv::Mat::zeros(sx.size(),CV_8UC3);
    for(int i=0; i<sx.rows; i++){
        cv::Vec3s *xptr = sx.ptr<cv::Vec3s>(i);
        cv::Vec3s *yptr = sy.ptr<cv::Vec3s>(i);
        cv::Vec3b *dptr = dst.ptr<cv::Vec3b>(i);

        for(int j=0; j<sx.cols; j++){ //calculating magnitude 
            dptr[j][0] = sqrt(pow(xptr[j][0],2) + pow(yptr[j][0],2));
            dptr[j][1] = sqrt(pow(xptr[j][1],2) + pow(yptr[j][1],2));
            dptr[j][2] = sqrt(pow(xptr[j][2],2) + pow(yptr[j][2],2));
        }
    }
    return 0;
}

int blurQuantize( cv::Mat &src, cv::Mat &dst, int levels ){
    cv::Mat blurred;
    int h = blur5x5(src,blurred);
    if(h!=0)
    {
        printf("Blur in Quantize failed");
    }
    int b = 255/levels;
    
    dst = cv::Mat::zeros(src.size(),CV_8UC3);

    for(int i=0; i<src.rows; i++){
        cv::Vec3b *rptr = blurred.ptr<cv::Vec3b>(i);
        cv::Vec3b *dptr = dst.ptr<cv::Vec3b>(i);

        for(int j=0; j<src.cols; j++){ // quantization of 3 color channels (not in loop for quicker processing)
            short x0 = rptr[j][0]/b;
            short x1 = rptr[j][1]/b;
            short x2 = rptr[j][2]/b;

            dptr[j][0] = x0* b;
            dptr[j][1] = x1* b;
            dptr[j][2] = x2* b;
        }
    }
    return 0;
}

int cartoon( cv::Mat &src, cv::Mat&dst, int levels, int magThreshold ){
    
    cv::Mat sx,sy,mag,converted;
    int h=1,hx=1,hy=1,q=1;
    //dst = cv::Mat::zeros(src.size(),CV_8UC3);
    mag = cv::Mat::zeros(src.size(),src.type());


    hx = sobelX3x3(src,sx);
    if(hx == 0){
        hy = sobelY3x3(src,sy);

        if(hy == 0){
            h = magnitude(sx,sy,mag); //getting magnitude

            if(h != 0){
                printf("magnitude failed");
            }
        }
        else{printf("sobelY failed");}
            
    }
    else{printf("sobelX failed");}

    cv::convertScaleAbs(mag,converted);
    q = blurQuantize(src,dst,levels); //getting blur + Quantize

    if(q==0){

        for(int i=0; i<dst.rows; i++){
            cv::Vec3b *dptr = dst.ptr<cv::Vec3b>(i);
            cv::Vec3b *rptr = converted.ptr<cv::Vec3b>(i);

            for(int j=0; j<dst.cols; j++){ // thresholding the blurQuantize based on the magnitude of sobel filter
                if(rptr[j][0] > magThreshold){
                    dptr[j][0] = 0;
                }
                if(rptr[j][1] > magThreshold){
                    dptr[j][1] = 0;
                }
                if(rptr[j][2] > magThreshold){
                    dptr[j][2] = 0;
                }
            }
        }
    }
    else{printf("blurQuantization failed");}
     return 0;
}

int negative( cv::Mat &src){

    //dst = cv::Mat::zeros(src.size(),CV_8UC3);

    for(int i=0; i<src.rows; i++){
        //cv::Vec3b *dptr = dst.ptr<cv::Vec3b>(i);
        cv::Vec3b *rptr = src.ptr<cv::Vec3b>(i);

        for(int j=0; j<src.cols; j++){ //finding negative of the image
            rptr[j][0] = 255 - rptr[j][0];
            rptr[j][1] = 255 - rptr[j][1];
            rptr[j][2] = 255 - rptr[j][2];

        }
    }
    return 0;
}

int TextAdd(cv::Mat &src, std::string text){ // adding text over the image 

    cv::Point pos(100,100);
    cv::Scalar color(10, 10, 150);

    int size = 3;
    int boldness = 7;

    putText(src, text, pos,cv::FONT_HERSHEY_COMPLEX, size,color, boldness);

    return 0;
}
