#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <iostream>

#include <ctime>
#pragma warning(disable : 4996)// tells c++ that ctime is safe

using namespace cv;
using namespace std;

string getFilename() {
    time_t now = time(0);
    char* dt = ctime(&now);
    string ts(dt);
    ts.replace(ts.find(":"), sizeof(":") - 1, "_");
    ts.replace(ts.find(":"), sizeof(":") - 1, "_");
    string filename = "tests/" + ts + ".png";
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
    

    Size size(w, h);// aspect ratio 3:4
    resize(img, img, size);//resize image
    Mat img_gray;
    cvtColor(img, img_gray, COLOR_BGR2GRAY);
    //Canny(img, img, 50, 150, 3);
    GaussianBlur(img_gray, img_gray, Size(9, 9), 2, 2);

    vector<Vec2f> mtx1[hCircles][vCircles];
    vector<Vec3f> circles; // in std
    HoughCircles(img_gray, circles, HOUGH_GRADIENT, 1.5, 10, 100, 75, 30, 50);

    for (int i = 0; i < circles.size(); i++)
    {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        // circle center
        circle(img, center, 3, Scalar(0, 255, 0), -1, 8, 0);
        // circle outline
        circle(img, center, radius, Scalar(255, 0, 255), 3, 8, 0);
    }
    cout << circles.size() << endl;

    // timestamp for test img storing
    string filename = getFilename();
    cout << filename << endl;

    namedWindow("Display Image", WINDOW_AUTOSIZE);
    imshow("Display Image", img);
    imwrite(filename, img);
    waitKey(0);

    return 0;
}