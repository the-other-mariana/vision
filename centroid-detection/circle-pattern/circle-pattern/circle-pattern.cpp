#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
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
    // user variables
    int w = 3000;
    int h = 2250;
    int hCircles = 8;
    int margin = 60;
    int hGap = 80;

    // letter paper aspect ratio: 1:1.2941
    int vCircles = hCircles / 1.2941;
    int outRadius = ((w - margin * 2 - (hGap * hCircles)) / hCircles) * 0.5;
    int inRadius = outRadius * 0.40;
    int vGap = (h - ((margin * 2) + (vCircles * outRadius * 2))) / (vCircles - 1);
    cout << vCircles;

    Mat img(h, w, CV_8UC1, Scalar(255));
    
    for (int i = 0; i < vCircles; i++) {
        for (int j = 0; j < hCircles; j++) {
            int xPos = margin + (hGap / 2.0) + (outRadius) + (j * (outRadius * 2)) + j * hGap;
            int yPos = margin + (outRadius) + (i * (outRadius * 2)) + i * vGap;
            circle(img, Point(xPos, yPos), outRadius, Scalar(0), FILLED, LINE_8, 0);
            circle(img, Point(xPos, yPos), inRadius, Scalar(255), FILLED, LINE_8, 0);
        }
    }

    // circle(img, Point(100, 100), 30, Scalar(0),FILLED, LINE_8, 0);
    namedWindow("Image", WINDOW_AUTOSIZE);
    imshow("Image", img);
    imwrite("../test.png", img);
    waitKey(0);


    return 0;
}