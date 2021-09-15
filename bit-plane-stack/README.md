# Coded Structured Light

## Analyzing 8 Images

This code reads 8 images in order to analyze the code that corresponds to each point (pixel) in another resulting image.

This works as follows:

Each image is linked to a bit of an 8-bit byte. The 8 images need to be black and white, and so to ensure this, we apply a binary threshold. Thus, in order to know the code that corresponds to each point (pixel) a resulting image, we need to analyze which color appears on that point inside each of the 8 images. 

For example, if we are analyzing the point (x, y), and that point contains:

- Black in the 1st image: 0

- White in the 2nd image: 1

- White in the 3rd image: 1

- Black in the 4th image: 0

- White in the 5th image: 1

- Black in the 6th image: 0

- White in the 7th image: 1

- White in the 8th image: 1

The code in binary that would correspond is 01101011. The result can be saved as integer in an output image. The output image contains in each pixel their corresponding codes, they can be integers instead of uchar.

To make the test, we use the images created at the last operation located in the folder https://github.com/the-other-mariana/vision/tree/master/gray-code-pattern/src, and according to the theory, the image output in this particular case would be a black and white gradient in the horizontal axis.

### Solution

```c++
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
    } 

    Mat result(h, w, CV_8UC1, Scalar(255));

    for (int i = 0; i < result.rows; i++) {
        for (int j = 0; j < result.cols; j++) {
            string byte = "";
            for (int k = 0; k < size; k++) {
                if (imgs[k].at<uchar>(i, j) == 255) byte += "1";
                if (imgs[k].at<uchar>(i, j) == 0) byte += "0";
            }
            uchar pixel = static_cast<uchar>(stoi(byte, 0, 2));
            result.at<uchar>(i, j) = pixel;
        }
    }

    namedWindow("result", WINDOW_AUTOSIZE);
    imshow("result", result);
    imwrite("../result.png", result);

    waitKey(0);

    return 0;
}
```

## Analyzing 10 images

If we want to analyze more than 8 images, indeed we will have more that 1 byte (8 bits), which will mean that we need to store the code of a pixel into a bigger structure than uchar (1 byte). For this, we will use now a matrix of integers. But in order to save and show the image in the code, the matrix must be of uchar type. For this, we will adjust the value with a remap function.

![img](https://github.com/the-other-mariana/vision/blob/master/bit-plane-stack/remap-diagram.png?raw=true)

## Output (Both Cases)

Source code: https://github.com/the-other-mariana/vision/blob/master/bit-plane-stack/src/src/src.cpp

![img](https://github.com/the-other-mariana/vision/blob/master/bit-plane-stack/src/result.png?raw=true)

## Findings

- The structure of the data type a `Mat` object must have follows the rules below:

    - CV_[number of bits][(U)nsigned/(S)igned/(F)loat][(C)hannels][1/2/3/4 channels]: CV_32UC1, for a 32-bit, Unsigned, 1-channel matrix.

- The difference between `uchar` and `char` type is that the last bit of information is taken as a bit for the number's sign (in the case of `char`) or as part of the number itself (in case of `uchar`).

## Handy Links

- https://try2explore.com/questions/12110895

- https://stackoverflow.com/questions/26502912/opencv-mat-for-integer-types-only