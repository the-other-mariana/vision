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
    Mat img = imread("pattern-photos/centered.jpg", 1);
    Mat centroids(h, w, CV_8UC3, Scalar(255,255,255));

    Size size(w, h);// aspect ratio 3:4
    resize(img, img, size);//resize image
    Mat img_gray;
    cvtColor(img, img_gray, COLOR_BGR2GRAY);
    //Canny(img, img, 50, 150, 3);
    GaussianBlur(img_gray, img_gray, Size(9, 9), 2, 2);

    vector<Vec2f> centers_outer;
    vector<Vec2f> centers_inner;
    vector<Vec3f> circles_outer; // in std
    vector<Vec3f> circles_inner;
    HoughCircles(img_gray, circles_outer, HOUGH_GRADIENT, 1.5, 10, 100, 75, 35, 50);
    HoughCircles(img_gray, circles_inner, HOUGH_GRADIENT, 1, 10, 70, 45, 5, 34);

    // original image detection marker
    for (int i = 0; i < circles_outer.size(); i++)
    {
        Point center(cvRound(circles_outer[i][0]), cvRound(circles_outer[i][1]));
        int radius = cvRound(circles_outer[i][2]);
        // circle center
        circle(img, center, 3, Scalar(255, 0, 255), -1, 8, 0);
        // circle outline
        circle(img, center, radius, Scalar(255, 0, 255), 3, 8, 0);
        Vec2f c(center.x, center.y);
        centers_outer.push_back(c);
    }
    for (int i = 0; i < circles_inner.size(); i++)
    {
        Point center(cvRound(circles_inner[i][0]), cvRound(circles_inner[i][1]));
        int radius = cvRound(circles_inner[i][2]);
        // circle center
        circle(img, center, 3, Scalar(255, 0, 0), -1, 8, 0);
        // circle outline
        circle(img, center, radius, Scalar(255, 0, 0), 3, 8, 0);
        Vec2f c(center.x, center.y);
        centers_inner.push_back(c);
    }
    
    // separate image centroid marker
    for (int i = 0; i < centers_outer.size(); i++) {
        Vec2f p = centers_outer[i];
        drawMarker(centroids, Point(p[0], p[1]), Scalar(255, 0, 255), MARKER_TILTED_CROSS, 10, 2, 8);
    }
    for (int i = 0; i < centers_inner.size(); i++) {
        Vec2f p = centers_inner[i];
        drawMarker(centroids, Point(p[0], p[1]), Scalar(255, 0, 0), MARKER_CROSS, 10, 2, 8);
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