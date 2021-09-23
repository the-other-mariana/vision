#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <math.h>
#include <string>

#define PI 3.14159265

using namespace cv;
using namespace std;
int main()
{
    int w = 1920;
    int h = 1080;
    int pixels = 192;

    int waves = w / pixels;
    float shift = 0.0; // for pure waves, set this to zero
    float factor = (w * 1.0) / (2.0 * PI);

    Mat img(h, w, CV_8UC1, Scalar(255));
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            float angle = (j / factor) * waves;
            uchar value = (255 / 2) + (255 / 2) * cos(angle + shift);
            img.at<uchar>(i, j) = value;
        }
    }

    namedWindow("Image", WINDOW_AUTOSIZE);
    imshow("Image", img);
    imwrite("../zero.png", img);
    waitKey(0);
    

    return 0;
}