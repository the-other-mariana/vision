# Computer Vision Algorithms

Algorithms to perform Computer Vision related operations.

## Specifications

OpenCV Version: `4.2.0`

## Set Up

- Download OpenCV from: https://opencv.org/releases/

- Follow the guide at: https://towardsdatascience.com/install-and-configure-opencv-4-2-0-in-windows-10-vc-d132c52063a1 or at my repo: https://github.com/the-other-mariana/image-processing-algorithms for Visual Studio 2019 set up for an opencv development environment.

- Try this basic code to see if it works:

```c++
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
using namespace cv;
int main()
{
    std::string image_path = samples::findFile("starry-night.jpg");
    Mat img = imread(image_path, IMREAD_COLOR);
    if(img.empty())
    {
        std::cout << "Could not read the image: " << image_path << std::endl;
        return 1;
    }
    imshow("Display window", img);
    int k = waitKey(0); // Wait for a keystroke in the window
    if(k == 's')
    {
        imwrite("starry_night.png", img);
    }
    return 0;
}
```

*Note: make sure you have an image named starry-night.jpg in the same folder as the .cpp file.*

## Content

1. [Gray Code Pattern Generation](https://github.com/the-other-mariana/vision/tree/master/gray-code-pattern)

![img](https://github.com/the-other-mariana/vision/blob/master/gray-code-pattern/out.png?raw=true)

2. [8-bit & 10-bit Stack](https://github.com/the-other-mariana/vision/tree/master/bit-plane-stack)

![img](https://github.com/the-other-mariana/vision/blob/master/bit-plane-stack/src/result.png?raw=true)

3. [Projector Calibration Papers](https://github.com/the-other-mariana/vision/tree/master/projector-calib-papers)

4. [Sinusoidal Pattern Generation](https://github.com/the-other-mariana/vision/tree/master/phase-shifting-sinusoidal-code)

![img](https://github.com/the-other-mariana/vision/blob/master/phase-shifting-sinusoidal-code/pattern/zero.png?raw=true)

5. [Pattern Detection for Camera Calibration](https://github.com/the-other-mariana/vision/tree/master/centroid-detection)

![img](https://github.com/the-other-mariana/vision/blob/master/centroid-detection/patterns.png?raw=true)

![img](https://github.com/the-other-mariana/vision/blob/master/centroid-detection/detections.png?raw=true)

5. [Pattern Detection Algorithm Evaluation](https://github.com/the-other-mariana/vision/tree/master/algorithm-evaluation)

![img](https://github.com/the-other-mariana/vision/blob/master/algorithm-evaluation/histograms/out/condensed-center.png?raw=true)

![img](https://github.com/the-other-mariana/vision/blob/master/algorithm-evaluation/histograms/out/condensed-front-tilt.png?raw=true)

![img](https://github.com/the-other-mariana/vision/blob/master/algorithm-evaluation/histograms/out/condensed-side-tilt.png?raw=true)

5. [Custom Calibration](https://github.com/the-other-mariana/vision/tree/master/calibration)

![img](https://github.com/the-other-mariana/vision/blob/master/calibration/res/plot.png?raw=true)