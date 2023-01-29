#include <opencv2/highgui.hpp>                                //including needed files
#include <iostream>
#include "../include/filter.h"

int main( int argc, char** argv )
{
    cv::Mat image,filtered,filtered2,filtered3;
    image = cv::imread("../Images/testing.jpg",cv::IMREAD_COLOR);       //reading image
    if(image.empty())
        {
            std::cout<<"Could not open file" << std::endl;    //throw error if file not present
            return 1;
        }    
    cv::namedWindow("First_Image", cv::WINDOW_AUTOSIZE);      //create a window to display image
    cv::namedWindow("After_Filter", cv::WINDOW_AUTOSIZE); 

    cv::imshow("First_Image", image); 
    
    //int h = grayscale(image,filtered);
    //int h = blur5x5(image,filtered);
    //int h = blurQuantize(image,filtered,15);
    int h = negative(image);
    filtered = image;
    //int h1 = cartoon(image,filtered,14,14);

                            //display image
    cv::imshow("After_Filter",filtered);
    cv::moveWindow("After_Filter",1000,0);

    int key = cv::waitKey(0);
    while (true){                   //wait till user presses 'esc' key or 'q'
    if(key == 27 || key==113)       //27 ascii of escape key, 113 ascii for 'q'
    {
        break;
    }
    key = cv::waitKey(0);
    }                                                         
    return 0;
}