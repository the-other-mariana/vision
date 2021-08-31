#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <math.h>
#include <string>

using namespace cv;
using namespace std;
int main()
{
    int w = 1920;
    int h = 1080;
    int steps = 10;

    // CV_8UC3 depicts : (3 channels,8 bit image depth)
    
    int stepWidth = w;

    for (int s = 1; s <= steps; s++) {
        Mat img(h, w, CV_8UC1, Scalar(255));
        int div = pow(2, s);
        stepWidth = ceil((w * 1.0) / div);
        int currWidth = 0;
        for (int k = 1; k <= div; k++) {
            currWidth += stepWidth;
            for (int i = 0; i < img.rows; i++) {
                for (int j = ((k - 1) * stepWidth); j < currWidth; j++) {
                    if (i < 0 || i >= img.rows || j < 0 || j >= img.cols) continue;
                    if (k % 2 == 1) {
                        img.at<uchar>(i, j) = 255;
                    }
                    else {
                        img.at<uchar>(i, j) = 0;
                    }
                    
                }
            }
        }
        imwrite("../" + to_string(s) + ".png", img);
    }
    
    /*
    namedWindow("Image", WINDOW_AUTOSIZE);
    imshow("Image", img);
    imwrite("../1.png", img);
    waitKey(0);
    */

    return 0;
}
