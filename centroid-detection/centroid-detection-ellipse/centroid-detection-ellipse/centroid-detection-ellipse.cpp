#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <iostream>

#include <ctime>
#pragma warning(disable : 4996)// tells c++ that ctime is safe

using namespace cv;
using namespace std;

string getFilename(string id) {
    time_t now = time(0);
    char* dt = ctime(&now);
    string ts(dt);
    ts.replace(ts.find(":"), sizeof(":") - 1, "_");
    ts.replace(ts.find(":"), sizeof(":") - 1, "_");
    string filename = "tests/" + ts + "-" + id + ".png";
    filename.replace(filename.find("\n"), sizeof("\n") - 1, "");

    return filename;
}

int main(int argc, char** argv)
{
    const int w = 1152;
    const int h = 864;
    const int hCircles = 8;
    const int vCircles = 6;

    int outMax = 7000;
    int outMin = 2000;
    int inMax = 1500;

    string pics[3] = { "center.jpg", "side-tilt.jpg", "front-tilt.jpg" };
    Mat img = imread("pattern-photos/" + pics[2], 1);
    Mat centroids(h, w, CV_8UC3, Scalar(255, 255, 255));

    Size size(w, h);// aspect ratio 3:4
    resize(img, img, size);//resize image
    Mat img_gray;
    Mat binary;
    cvtColor(img, img_gray, COLOR_BGR2GRAY);
    //Canny(img, img, 50, 150, 3);
    GaussianBlur(img_gray, img_gray, Size(9, 9), 2, 2);
    threshold(img_gray, binary, 150, 255, THRESH_BINARY);

    Mat canny_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    // detect edges using canny
    Canny(img_gray, canny_output, 50, 150, 3);

    // find contours
    findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));


    for (size_t i = 0; i < contours.size(); i++)
    {
        if (contourArea(contours[i]) < outMax && contourArea(contours[i]) > outMin) {
            Mat pointsf;
            Mat(contours[i]).convertTo(pointsf, CV_32F);
            RotatedRect box = fitEllipse(pointsf);
            Point2f rectCenter = box.center;

            ellipse(img, box, Scalar(255, 0, 255), 2, 8);
            drawMarker(img, Point(rectCenter.x, rectCenter.y), Scalar(255, 0, 255), MARKER_TILTED_CROSS, 10, 2, 8);
            drawMarker(centroids, Point(rectCenter.x, rectCenter.y), Scalar(255, 0, 255), MARKER_TILTED_CROSS, 10, 2, 8);
        }
        if (contourArea(contours[i]) < inMax) {
            Mat pointsf;
            Mat(contours[i]).convertTo(pointsf, CV_32F);
            RotatedRect box = fitEllipse(pointsf);
            Point2f rectCenter = box.center;

            ellipse(img, box, Scalar(255, 0, 0), 2, 8);
            drawMarker(img, Point(rectCenter.x, rectCenter.y), Scalar(255, 0, 0), MARKER_CROSS, 10, 2, 8);
            drawMarker(centroids, Point(rectCenter.x, rectCenter.y), Scalar(255, 0, 0), MARKER_CROSS, 10, 2, 8);
        }
        
    }


    // timestamp for test img storing
    string patternImg = getFilename("pattern");
    string centroidImg = getFilename("centroids");

    namedWindow("Centroids", WINDOW_AUTOSIZE);
    imshow("Centroids", centroids);

    namedWindow("Image", WINDOW_AUTOSIZE);
    imshow("Image", img);
    imwrite(patternImg, img);
    imwrite(centroidImg, centroids);
    waitKey(0);

    return 0;
}