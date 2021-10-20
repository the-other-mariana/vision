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

    int outMax = 15000;
    int outMin = 1300;
    int inMax = 1300;
    int inMin = 12;

    string pics[5] = { "center", "front45", "front60", "side4502", "side60" };
    string option = pics[3];
    Mat img = imread("test-renders/" + option + ".png", 1);
    Mat centroids(h, w, CV_8UC3, Scalar(255, 255, 255));

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
        if (contourArea(contours[i]) < outMax && contourArea(contours[i]) > outMin && i % 2 == 1) {
            outerMoments[i] = moments(contours[i], false);
            //cout << "Outer: " << contourArea(contours[i]) << endl;
        }
        if (contourArea(contours[i]) < inMax && contourArea(contours[i]) > inMin && i % 2 == 1) {
            innerMoments[i] = moments(contours[i], false);
            //cout << "Inner: " << contourArea(contours[i]) << endl;
        }
    }
    cout << outerMoments.size() << " " << innerMoments.size() << endl;
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
    
    cout << contours.size() << endl;
    for (int i = 0; i < contours.size(); i++)
    {
        if (contourArea(contours[i]) < outMax && contourArea(contours[i]) > outMin && i % 2 == 1) {
            cout << "\tOuter: " << contourArea(contours[i]) << endl;
            Scalar color = Scalar(255, 0, 255);
            op.push_back(oc[i]);
            drawContours(img, contours, i, color, 2, 8, hierarchy, 0, Point());
            drawMarker(img, oc[i], Scalar(255, 0, 255), MARKER_TILTED_CROSS, 10, 2, 8);
            drawMarker(centroids, oc[i], Scalar(255, 0, 255), MARKER_TILTED_CROSS, 10, 2, 8);
            //putText(img, //target image
            //   to_string(int(contourArea(contours[i]))), //text
            //    Point(int(oc[i].x) - i - (rand() % 30), int(oc[i].y) - i - (rand() % 30)), //top-left position
            //    FONT_HERSHEY_DUPLEX,
            //    0.5,
            //    color, //font color
            //    2);
            
        }
        else if (contourArea(contours[i]) < inMax && contourArea(contours[i]) > inMin && i % 2 == 1) {
            cout << "Inner: " << contourArea(contours[i]) << endl;
            Scalar color = Scalar(255, 0, 0);
            ip.push_back(ic[i]);
            drawContours(img, contours, i, color, 2, 8, hierarchy, 0, Point());
            drawMarker(img, ic[i], Scalar(255, 0, 0), MARKER_CROSS, 10, 2, 8);
            drawMarker(centroids, ic[i], Scalar(255, 0, 0), MARKER_CROSS, 10, 2, 8);
            //putText(img, //target image
            //    to_string(int(contourArea(contours[i]))), //text
            //    Point(int(ic[i].x)-i - (rand()%30), int(ic[i].y)-i - (rand() % 30)), //top-left position
            //    FONT_HERSHEY_DUPLEX,
            //    0.5,
            //    CV_RGB(118, 185, 0), //font color
            //    2);
        }
        else {
            if (i % 2 == 0) {
                cout << "\tNone: " << contourArea(contours[i]) << endl;
            }
            
        }

    }
    cout << op.size() << " " << ip.size() << endl;

    ofstream out("regions" + option + ".txt");
    cout << "op:" << op.size() << endl;
    cout << "ip:" << ip.size() << endl;
    for (int i = 0; i < op.size(); i++) {
        cout << i << op[i].x << ", " << op[i].y << endl;
        string os = to_string(op[i].x) + ";" + to_string(op[i].y) + "\n";
        out << os;
    }
    for (int i = 0; i < ip.size(); i++) {
        cout << i << ip[i].x << ", " << ip[i].y << endl;
        string is = to_string(ip[i].x) + ";" + to_string(ip[i].y) + "\n";
        out << is;
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