#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "../include/filter.h"
#include <string>

int main(int argc, char *argv[]) {
        cv::VideoCapture *capdev;

        // open the video device
        capdev = new cv::VideoCapture(0);
        if( !capdev->isOpened() ) {
                printf("Unable to open video device\n");
                return(-1);
        }
        using namespace std;

        // get some properties of the image
        cv::Size refS( (int) capdev->get(cv::CAP_PROP_FRAME_WIDTH ),
                       (int) capdev->get(cv::CAP_PROP_FRAME_HEIGHT));
        printf("Expected size: %d %d\n", refS.width, refS.height);

        cv::namedWindow("Video", 1); // identifies a window
        cv::Mat frame;
        cv::Mat frame_filtered;
        cv::Mat frame_filtered2;
        
        frame_filtered = cv::Mat::zeros(frame.size(),CV_8UC3);
        frame_filtered2 = cv::Mat::zeros(frame.size(),CV_8UC3);


        bool gray = false;              //declaring toggles for different filters/effects
        bool gray_alt = false;
        bool blur = false;
        bool sobelX = false;
        bool sobelY = false;
        bool sobelMag = false;
        bool blurQuant = false;
        bool cart = false;
        bool effect = false;
        bool text = false;
        string message;

        bool record = false;
        cv::VideoWriter vw("../Output/Video.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 10, refS, true);
        for(;;) {
                *capdev >> frame; // get a new frame from the camera, treat as a stream
                if (frame.empty()) {
                  printf("frame is empty\n");
                  break;
                }                
                int h = 1;
                // see if there is a waiting keystroke
                bool res = false;
                char key = cv::waitKey(10);
                

                // Activating Different Filters based on their respective toggles
                if(effect == true){
                        h = negative(frame);
                        if(h != 0){
                                printf("\n effect failed");
                        }
                }
                if(gray == true){
                        cv::cvtColor(frame,frame, 6);
                }
                if(blurQuant ==true){
                        h = blurQuantize(frame,frame_filtered,15);
                        frame = frame_filtered;
                        if(h!=0){
                                printf("\n Blur Quantize Failed");
                        }
                }
                if(blur == true){
                        h = blur5x5(frame,frame_filtered);
                        frame = frame_filtered;
                        if(h!=0){
                                printf("\n Blurring Failed");
                        }
                }
                if(gray_alt == true){
                        h = grayscale(frame,frame_filtered);
                        frame = frame_filtered;
                        if (h != 0){
                                printf("\n Grayscale conversion Unsuccessful");
                        }
                }
                if(sobelX == true){
                        h = sobelX3x3(frame,frame_filtered);
                        cv::convertScaleAbs(frame_filtered,frame);
                        if (h != 0){
                                printf("\n SobelX3x3 failed");
                        }
                }
                if(sobelY == true){
                        h = sobelY3x3(frame,frame_filtered);
                        cv::convertScaleAbs(frame_filtered,frame);
                        if (h != 0){
                                printf("\n SobelY3x3 failed");
                        }
                }
                if(sobelMag == true){
                        int hx = sobelX3x3(frame,frame_filtered); // getting sobelX first
                        if(hx == 0){
                                int hy = sobelY3x3(frame,frame_filtered2); //getting sobelY next
                                if(hy == 0){
                                        h = magnitude(frame_filtered,frame_filtered2,frame);    //using the result from sobelX and sobelY
                                                                                                //to find magnitude
                                }
                                else{printf("sobelY failed");}
                        }
                        else{printf("sobelX failed");}
                        
                        cv::convertScaleAbs(frame,frame_filtered);
                        
                        frame = frame_filtered;
                        if (h != 0){
                                printf("\n Sobel Magnitude failed");
                        }
                }
                if(cart == true){
                        h = cartoon(frame,frame_filtered,14,14);
                        frame = frame_filtered;
                        if(h != 0){
                                printf("\n cartoonization failed");
                        }
                }
                if(text == true){
                        h = TextAdd(frame,message);
                }





                // Assigning different keys to set and reset toggles for different filters/effects
                if (key == 'q') {
                    break;
                }
                else if (key == 's'){
                        res = cv::imwrite("../Output/frame.jpg",frame);
                        if(res == false){
                                printf("\nNo Image was saved");
                        }
                        else{
                                printf("\nImage Saved Successfully");
                        }
                }
                else if (key == 'g'){
                        if(gray == false){
                                gray = true;
                                gray_alt = false;
                        }
                        else{
                                gray = false;

                        }
                }
                else if (key == 'h'){
                        if(gray_alt == false){
                                gray_alt = true;
                                gray = false;
                        }
                        else{
                                gray_alt = false;
                        }
                }
                else if(key == 'b'){
                        if(blur == false){
                                blur = true;
                        }
                        else{
                                blur = false;
                        }
                }
                else if(key =='x'){
                        if(sobelX == false){
                                sobelX = true;
                                sobelMag = false;
                        }
                        else{
                                sobelX = false;
                        }
                }
                else if(key =='y'){
                        if(sobelY == false){
                                sobelY = true;
                                sobelMag = false;
                        }
                        else{
                                sobelY = false;
                        }
                }
                else if(key =='m'){
                        if(sobelMag == false){
                                sobelMag = true;
                                sobelX = false;
                                sobelY = false;
                        }
                        else{
                                sobelMag = false;
                        }
                }
                else if(key =='i'){
                        if(blurQuant == false){
                                blurQuant = true;
                                blur = false;
                        }
                        else{
                                blurQuant = false;
                        }
                }
                else if(key =='c'){
                        if(cart == false){
                                cart = true;
                        }
                        else{
                                cart = false;
                        }
                }
                else if(key =='n'){
                        if(effect == false){
                                effect = true;
                        }
                        else{
                                effect = false;
                        }
                }
                else if(key =='r'){
                        if(record == false){
                                record = true;
                                printf("recording started");
                        }
                        else{
                                record = false;
                                vw.release();
                                printf("recording stopped");
                        }
                }
                else if(key =='t'){
                        if(text == false){
                                text = true;
                        }
                        else{
                                text = false;
                        }
                }
                else if(key == 'p'){
                        std::cout<< "Enter Message : "<<std::endl; // getting text to display on the text mode
                        std::getline(cin,message);
                }


                if(record == true){ //recording realtime
                        vw.write(frame);
                }
                cv::imshow("Video", frame); 


        }


        delete capdev;
        return(0);
}