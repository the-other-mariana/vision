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
    Scalar colorInner = Scalar(255, 0, 255);
    Scalar colorOuter = Scalar(255, 0, 0);

    int outMax = 7000;
    int outMin = 2000;
    int inMax = 1500;

    string pics[3] = { "1.jpg", "2.jpg", "3.jpg" };
    Mat img = imread("pattern-photos/" + pics[0], 1);
    Mat centroids(h, w, CV_8UC3, Scalar(255, 255, 255));

    Size size(w, h);// aspect ratio 3:4
    resize(img, img, size);//resize image

    
    Mat img_gray;
    Mat img_gray_inv;
    Mat binary;
    cvtColor(img, img_gray, COLOR_BGR2GRAY);
    GaussianBlur(img_gray, img_gray, Size(9, 9), 2, 2);
    threshold(img_gray, binary, 150, 255, THRESH_BINARY);

    SimpleBlobDetector::Params params1;
    params1.filterByColor = false;
    params1.minThreshold = 50;
    params1.maxThreshold = 255;
    params1.filterByArea = true;
    params1.maxArea = 750;
    params1.filterByCircularity = false;
    params1.filterByConvexity = false;
    params1.filterByInertia = false;
    vector<KeyPoint> pts_inner;
    Ptr<SimpleBlobDetector> detector1 = SimpleBlobDetector::create(params1);

    bitwise_not(img_gray, img_gray_inv);
    detector1->detect(img_gray, pts_inner);
    for (int i = 0; i < pts_inner.size(); i++) {
        KeyPoint k = pts_inner[i];
        Point center = Point(k.pt.x, k.pt.y);
        Scalar color = Scalar(250, 250, 250);
        circle(img, center, k.size / 2.0, Scalar(255, 0, 255), 2, 8, 0);
        drawMarker(img, center, colorInner, MARKER_TILTED_CROSS, 10, 2, 8);
        drawMarker(centroids, center, colorInner, MARKER_TILTED_CROSS, 10, 2, 8);
        circle(img_gray_inv, center, k.size, color, FILLED, 8, 0);
        
    }
    
    
    SimpleBlobDetector::Params params2;
    params2.filterByColor = false;
    params2.minThreshold = 80;
    params2.maxThreshold = 255;
    params2.filterByArea = true;
    params2.minArea = 750;
    //params2.maxArea = 1500;
    params2.filterByCircularity = false;
    params2.filterByConvexity = false;
    params2.filterByInertia = false;
    vector<KeyPoint> pts_outer;
    Ptr<SimpleBlobDetector> detector2 = SimpleBlobDetector::create(params2);

    detector2->detect(img_gray_inv, pts_outer);
    for (int i = 0; i < pts_outer.size(); i++) {
        KeyPoint k = pts_outer[i];
        Point center = Point(k.pt.x, k.pt.y);
        circle(img, center, k.size / 2.0, Scalar(255, 0, 0), 2, 8, 0);
        drawMarker(img, center, colorOuter, MARKER_CROSS, 10, 2, 8);
        drawMarker(centroids, center, colorOuter, MARKER_CROSS, 10, 2, 8);
    }


    // timestamp for test img storing
    string patternImg = getFilename("pattern");
    string centroidImg = getFilename("centroids");

    namedWindow("Detection", WINDOW_AUTOSIZE);
    imshow("Detection", centroids);

    namedWindow("Image", WINDOW_AUTOSIZE);
    imshow("Image", img);

    imwrite(patternImg, img);
    imwrite(centroidImg, centroids);
    waitKey(0);

    return 0;

}
