# Opencv Calibration Sample Code Customization

Now, we will customize OpenCV's original [calibration.cpp](https://github.com/the-other-mariana/vision/blob/master/calibration/opencv-sources-samples-cpp/calibration.cpp) file so that we can adjust to the method and pattern we will use. This file can be found at: `C:\OpenCV-4.2.0\opencv\sources\samples\cpp`, while all the example input images and files are found at: `C:\OpenCV-4.2.0\opencv\sources\samples\data`.

Source: https://github.com/the-other-mariana/vision/blob/master/calibration/calibration/calibration/calibration.cpp

## Error Scatter Plot

By running the calibration sample with the opencv input images, we get a view:

![img](https://github.com/the-other-mariana/vision/blob/master/calibration/res/view.png?raw=true)

And if we plot the error scatter with a scale factor of 1000:

![img](https://github.com/the-other-mariana/vision/blob/master/calibration/res/plot.png?raw=true)