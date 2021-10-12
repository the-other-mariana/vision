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
    const int w = 1152;
    const int h = 864;
    const int hCircles = 8;
    const int vCircles = 6;

    int outMax = 7000;
    int outMin = 1450;
    int inMax = 1400;
    int inMin = 12;

    string pics[3] = { "1.jpg", "2.jpg", "3.jpg" };
    Mat img = imread("pattern-photos/" + pics[1], 1);
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

    // get the moments
    vector<Moments> outerMoments(contours.size());
    vector<Moments> innerMoments(contours.size());
    for (int i = 0; i < contours.size(); i++)
    {
        cout << "Area: " << contourArea(contours[i]) << endl;
        if (contourArea(contours[i]) < outMax && contourArea(contours[i]) > outMin) {
            outerMoments[i] = moments(contours[i], false);
            //cout << "Outer: " << contourArea(contours[i]) << endl;
        }
        if (contourArea(contours[i]) < inMax && contourArea(contours[i]) > inMin) {
            innerMoments[i] = moments(contours[i], false);
            //cout << "Inner: " << contourArea(contours[i]) << endl;
        }
    }

    // get the centroid of figures.
    vector<Point2f> oc(outerMoments.size());
    for (int i = 0; i < outerMoments.size(); i++)
    {
        oc[i] = Point2f(outerMoments[i].m10 / outerMoments[i].m00, outerMoments[i].m01 / outerMoments[i].m00);
    }
    vector<Point2f> ic(innerMoments.size());
    for (int i = 0; i < innerMoments.size(); i++)
    {
        ic[i] = Point2f(innerMoments[i].m10 / innerMoments[i].m00, innerMoments[i].m01 / innerMoments[i].m00);
    }

    // draw contours
    vector<Point2f> op;
    vector<Point2f> ip;
    Mat drawing(canny_output.size(), CV_8UC3, Scalar(255, 255, 255));
    for (int i = 0; i < contours.size(); i++)
    {
        if (contourArea(contours[i]) < outMax && contourArea(contours[i]) > outMin && i % 2 == 0) {
            Scalar color = Scalar(255, 0, 255);
            op.push_back(oc[i]);
            drawContours(img, contours, i, color, 2, 8, hierarchy, 0, Point());
            drawMarker(img, oc[i], Scalar(255, 0, 255), MARKER_TILTED_CROSS, 10, 2, 8);
            drawMarker(centroids, oc[i], Scalar(255, 0, 255), MARKER_TILTED_CROSS, 10, 2, 8);
        }
        if (contourArea(contours[i]) < inMax && contourArea(contours[i]) > inMin && i % 2 == 0) {
            Scalar color = Scalar(255, 0, 0);
            ip.push_back(ic[i]);
            drawContours(img, contours, i, color, 2, 8, hierarchy, 0, Point());
            drawMarker(img, ic[i], Scalar(255, 0, 0), MARKER_CROSS, 10, 2, 8);
            drawMarker(centroids, ic[i], Scalar(255, 0, 0), MARKER_CROSS, 10, 2, 8);
        }

    }

    ofstream out("regions.txt");
    for (int i = 0; i < op.size(); i++) {
        cout << i << op[i].x << ", " << op[i].y << endl;
        cout << i << ip[i].x << ", " << ip[i].y << endl;
        string is = to_string(ip[i].x) + ";" + to_string(ip[i].y) + "\n";
        string os = to_string(op[i].x) + ";" + to_string(op[i].y) + "\n";
        out << is;
        out << os;
    }
    out.close();
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