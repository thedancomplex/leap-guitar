//! [includes]
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "Leap.h"


#include <iostream>

// using namespace cv;
//! [includes]

int display_image()
{


    //Uses Cinder OpenGL wrapper
    Leap::Controller controller;
    Leap::Frame frame = controller.frame();

    Leap::ImageList images = frame.images();
    Leap::Image image = images[0];

    //! [imread]
    //std::string image_path = samples::findFile("starry_night.jpg");
    // Mat img = imread(image_path, IMREAD_COLOR);
    const unsigned char* image_buffer = image.data();

    //Draw the raw image data as a greyscale bitmap
    int x = image.width();
    int y = image.height();
    std::cout << "x = " << x << "  y = " << y << std::endl;
    cv::Mat img = cv::Mat::zeros(x, y, CV_8UC1);
    for(int ix = 0; ix < x; ix++)
    {
	    for(int iy = 0; iy < y; iy++)
	    {
		    int ii = x * ix + iy;
		    img.at<uchar>(ix,iy) = image_buffer[ii];
		   // std::cout << "dan" << std::endl;
	    }
    }

    //! [imread]
    // Mat img = image_buffer;

    //! [empty]
    if(img.empty())
    {
        std::cout << "Could not read the image" << std::endl;
        return 1;
    }
    //! [empty]

    //! [imshow]
    cv::imshow("Display window", img);
    int k = cv::waitKey(0); // Wait for a keystroke in the window
    //! [imshow]

    //! [imsave]
    if(k == 's')
    {
	    cv::imwrite("starry_night.png", img);
    }
    //! [imsave]

    return 0;
}
