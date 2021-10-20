#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

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
    const int w = 1920;
    const int h = 1080;
    const int hCircles = 8;
    const int vCircles = 6;
    Scalar colorOuter = Scalar(255, 0, 255);
    Scalar colorInner = Scalar(255, 0, 0);

    int limit = 1600;

    string pics[5] = { "center", "front45", "front60", "side4504", "side60" };
    string option = pics[0];
    Mat img = imread("test-renders/" + option + ".png", 1);
    Mat centroids(h, w, CV_8UC3, Scalar(255, 255, 255));

    //Size size(w, h);// aspect ratio 3:4
    //resize(img, img, size);//resize image


    Mat img_gray;
    Mat img_gray_inv;
    Mat binary;
    cvtColor(img, img_gray, COLOR_BGR2GRAY);
    GaussianBlur(img_gray, img_gray, Size(9, 9), 2, 2);
    threshold(img_gray, binary, 150, 255, THRESH_BINARY);

    // inner circles: blue
    SimpleBlobDetector::Params params1;
    params1.filterByColor = false;
    params1.minThreshold = 50;
    params1.maxThreshold = 255;
    params1.filterByArea = true;
    params1.maxArea = limit;
    params1.filterByCircularity = false;
    params1.filterByConvexity = false;
    params1.filterByInertia = false;
    vector<KeyPoint> pts_inner;
    Ptr<SimpleBlobDetector> detector1 = SimpleBlobDetector::create(params1);
    Point2f ip[48];

    bitwise_not(img_gray, img_gray_inv);
    detector1->detect(img_gray, pts_inner);
    for (int i = 0; i < pts_inner.size(); i++) {
        cout << pts_inner[i].size << endl;
        KeyPoint k = pts_inner[i];
        Point center = Point(k.pt.x, k.pt.y);
        ip[i] = k.pt;
        Scalar color = Scalar(250, 250, 250);
        circle(img, ip[i], k.size / 2.0, colorInner, 2, 8, 0);
        drawMarker(img, center, colorInner, MARKER_TILTED_CROSS, 10, 2, 8);
        drawMarker(centroids, center, colorInner, MARKER_TILTED_CROSS, 10, 2, 8);
        // comment this on center
        //circle(img_gray_inv, ip[i], k.size * 0.70, color, FILLED, 8, 0);
        cout << "Pt: " << ip[i].x << " " << ip[i].y << endl;
    }

    // outer circles: magenta
    SimpleBlobDetector::Params params2;
    params2.filterByColor = false;
    params2.minThreshold = 80;
    params2.maxThreshold = 255;
    params2.filterByArea = true;
    params2.minArea = limit;
    //params2.maxArea = 1500;
    params2.filterByCircularity = false;
    params2.filterByConvexity = false;
    params2.filterByInertia = false;
    vector<KeyPoint> pts_outer;
    Ptr<SimpleBlobDetector> detector2 = SimpleBlobDetector::create(params2);
    Point2f op[48];

    detector2->detect(img_gray_inv, pts_outer);
    for (int i = 0; i < pts_outer.size(); i++) {
        KeyPoint k = pts_outer[i];
        Point center = Point(k.pt.x, k.pt.y);
        op[i] = k.pt;
        circle(img, center, k.size / 2.0, colorOuter, 2, 8, 0);
        drawMarker(img, center, colorOuter, MARKER_CROSS, 10, 2, 8);
        drawMarker(centroids, center, colorOuter, MARKER_CROSS, 10, 2, 8);
    }
    
    ofstream out("blobs" + option + ".txt");

    for (int i = 0; i < 48; i++) {
        cout << i << " " << ip[i].x << ", " << ip[i].y << endl;
        cout << i << " " << op[i].x << ", " << op[i].y << endl;
        string is = to_string(ip[i].x) + ";" + to_string(ip[i].y) + "\n";
        string os = to_string(op[i].x) + ";" + to_string(op[i].y) + "\n";
        out << is;
        out << os;
    }
    out.close();
    
    // timestamp for test img storing
    string patternImg = getFilename("pattern");
    string centroidImg = getFilename("centroids");

    namedWindow("Detection", WINDOW_NORMAL);
    imshow("Detection", centroids);

    namedWindow("Binary", WINDOW_NORMAL);
    imshow("Binary", binary);

    namedWindow("Inverted", WINDOW_NORMAL);
    imshow("Inverted", img_gray_inv);

    namedWindow("Image", WINDOW_NORMAL);
    imshow("Image", img);

    imwrite(patternImg, img);
    imwrite(centroidImg, centroids);
    waitKey(0);

    return 0;

}
