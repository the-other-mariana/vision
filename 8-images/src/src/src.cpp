#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp> // threshold

#include<stdlib.h>
#include<stdio.h>
#include <iostream>
#include <math.h>
#include <string>

using namespace cv;
using namespace std;
int main()
{
    int w = 1920;
    int h = 1080;
    const int size = 8;
    int steps = 10;

    Mat imgs[size];

    for (int i = 0; i < size; i++) {
        string file = "test-images/" + to_string(i + 1) + ".png";
        Mat img = imread(file, IMREAD_GRAYSCALE);
        threshold(img, imgs[i], 128, 255, THRESH_BINARY);
    } 

    Mat result(h, w, CV_8UC1, Scalar(255));

    for (int i = 0; i < result.rows; i++) {
        for (int j = 0; j < result.cols; j++) {
            string byte = "";
            for (int k = 0; k < size; k++) {
                if (imgs[k].at<uchar>(i, j) == 255) byte += "1";
                if (imgs[k].at<uchar>(i, j) == 0) byte += "0";
            }
            uchar pixel = static_cast<uchar>(stoi(byte, 0, 2));
            result.at<uchar>(i, j) = pixel;
        }
    }

    namedWindow("result", WINDOW_AUTOSIZE);
    imshow("result", result);
    imwrite("../result.png", result);

    waitKey(0);

    return 0;
}
