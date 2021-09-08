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


        namedWindow(file, WINDOW_AUTOSIZE);
        imshow(file, imgs[i]);
    }
    waitKey(0);

    return 0;
}
