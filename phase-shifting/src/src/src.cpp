#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
using namespace cv;
int main()
{
    // CV_8UC3 depicts : (3 channels,8 bit image depth)
    Mat img(1080, 1920, CV_8UC1, Scalar(255));

    namedWindow("Image", WINDOW_AUTOSIZE);
    imshow("Image", img);
    waitKey(0);

    destroyWindow("Image");

    return 0;
}
