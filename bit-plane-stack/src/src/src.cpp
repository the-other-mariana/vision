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

/* function that takes a value between low and high and converts it into a value between 0 and 255 */
int remap(int value, int low, int high) {
    return ((255 * (value - low)) / (high - low));
}

int main()
{
    int w = 1920;
    int h = 1080;
    const int size = 10;
    int steps = 10;

    Mat imgs[size];

    for (int i = 0; i < size; i++) {
        string file = "test-images/" + to_string(i + 1) + ".png";
        Mat img = imread(file, IMREAD_GRAYSCALE);
        threshold(img, imgs[i], 128, 255, THRESH_BINARY);
    } 

    Mat aux(h, w, CV_32SC1, Scalar(255));
    Mat result(h, w, CV_8UC1, Scalar(255));

    for (int i = 0; i < aux.rows; i++) {
        for (int j = 0; j < aux.cols; j++) {
            string byte = "";
            for (int k = 0; k < size; k++) {
                if (imgs[k].at<uchar>(i, j) == 255) byte += "1";
                if (imgs[k].at<uchar>(i, j) == 0) byte += "0";
            }
            
            int pixel = stoi(byte, 0, 2);
            aux.at<__int32>(i, j) = pixel;
            result.at<uchar>(i, j) = remap(pixel, 0, 1024);
        }
    }

    namedWindow("result", WINDOW_AUTOSIZE);
    imshow("result", result);
    imwrite("../result.png", result);

    waitKey(0);

    return 0;
}
