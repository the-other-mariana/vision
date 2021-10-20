# Centroid Detection for Camera Calibration

## Pattern Generation

The first step is to generate a circle-based pattern for the camera to process. It contains one black circle and on top of it another white circle, 40% smaller. This has the purpose of being more accurate: the centroid detection will have two matrices of centroids: one for all the black circles' centers and the other for the white ones'. In this way, the final centroid would be an average of the two centroid matrices, and thus taking on account two circles per centroid instead of just one.

Source: https://github.com/the-other-mariana/vision/blob/master/centroid-detection/circle-pattern/circle-pattern/circle-pattern.cpp

![img](https://github.com/the-other-mariana/vision/blob/master/centroid-detection/circle-pattern/test.png)

## Centroid Detection

1. This step is to apply a Hough Transform to photographs of the printed pattern, in order to detect circles with a certain big range of radius (black circles), and those with the smaller range of radius (white circles). Then, their centroids are marked on a separate image to visualize the differences. This step works fine with perfect circles, but with the tilted photos, the circles were now ellipses, and where not detected appropiately.

    - Source (1): https://github.com/the-other-mariana/vision/blob/master/centroid-detection/circle-centroid-detection/circle-centroid-detection/circle-centroid-detection.cpp

Status: Not Considered

2. The second option is to use the concept of Blobs, and for that opencv has a structure called SimpleBlobDetector, to which you adjust the parameters and then get the key points (centers) of such detected blobs. In this case, an area filter is done for the blobs.

- Source (2): https://github.com/the-other-mariana/vision/blob/master/centroid-detection/circle-centroid-detection-blobs/circle-centroid-detection-blobs/circle-centroid-detection-blobs.cpp

- What is a **blob**? A blob is a group of connected pixels that share a property, like the grayscale value, an specific area, etc.

Status: Considered

3. The third option is to use Region Detection, and through an area threshold applied to the contours found on the photo, mark it as either an outer centroid or an inner centroid. This process seemed to work nice.

    - Source (3): https://github.com/the-other-mariana/vision/blob/master/centroid-detection/circle-centroid-detection-regions/circle-centroid-detection-regions/circle-centroid-detection-regions.cpp

    - What is the **centroid** of a shape? It is the mean (average) of all the points, in this case pixels, in a shape.

    ![\Large c=\frac{1}{n}\sum_{i=1}^{n}x_{i}](https://latex.codecogs.com/svg.latex?\Large&space;c=\frac{1}{n}\sum_{i=1}^{n}x_{i})

    We find the center of a detected blob using **moments** in opencv:

    ![\Large c_{x}=\frac{M_{10}}{M_{00}}](https://latex.codecogs.com/svg.latex?\Large&space;c_{x}=\frac{M_{10}}{M_{00}})

    ![\Large c_{y}=\frac{M_{01}}{M_{00}}](https://latex.codecogs.com/svg.latex?\Large&space;c_{y}=\frac{M_{01}}{M_{00}})

    Where the c_x and c_y are the x and y coordinates.

    The M_10 refers to the calculation that, for every white pixel, the accumulated value is j^1 * i^0 * (pixel / 255). Corresponding subindex refer to the power to which j and i are elevated.

Status: Considered

4. The fourth option was to try and fit an ellipse to the found contours that once again fell in the estimated areas for outer and inner circles, and then mark the centers of both. This was similar to the previous step, which also works nice.

    - Source (4): https://github.com/the-other-mariana/vision/blob/master/centroid-detection/centroid-detection-ellipse/centroid-detection-ellipse/centroid-detection-ellipse.cpp

Status: Considered

### Detected Centroids (2, 3)
----

![img](https://github.com/the-other-mariana/vision/blob/master/centroid-detection/patterns.png?raw=true)

### Marked Centroids (2, 3)
----

![img](https://github.com/the-other-mariana/vision/blob/master/centroid-detection/detections.png?raw=true)

## Handy Links

- [OpenCV Flags](https://stackoverflow.com/questions/22547416/open-cv-flags-dont-work)

- [OpenCV Flags 3.0](https://stackoverflow.com/questions/10344246/how-can-i-convert-a-cvmat-to-a-gray-scale-in-opencv)

- [Hough Transform Doc](https://docs.opencv.org/4.5.1/dd/d1a/group__imgproc__feature.html#ga47849c3be0d0406ad3ca45db65a25d2d)

- [Common Aspect Ratios](https://en.wikipedia.org/wiki/List_of_common_resolutions)

- [Hough Example](https://stackoverflow.com/questions/39630077/using-opencv-and-hough-transform-circle-to-detect-circles-subscript-error)

- [Blobs](https://learnopencv.com/find-center-of-blob-centroid-using-opencv-cpp-python/)

- [Contours](https://stackoverflow.com/questions/10262600/how-to-detect-region-of-large-of-white-pixels-using-opencv)

- [Fit Ellipse](https://stackoverflow.com/questions/28835644/ambiguity-in-ellipse-detection-in-opencv-c)

- [Rotated Rect](https://vovkos.github.io/doxyrest-showcase/opencv/sphinx_rtd_theme/class_cv_RotatedRect.html)

- [MD Eq](https://www.codecogs.com/latex/eqneditor.php)

- [Contours Func](https://docs.opencv.org/4.5.3/dd/d49/tutorial_py_contour_features.html)

- [Find Contours](https://docs.opencv.org/3.4.15/d3/dc0/group__imgproc__shape.html#ga17ed9f5d79ae97bd4c7cf18403e1689a)

- [moments()](https://docs.opencv.org/4.5.3/d3/dc0/group__imgproc__shape.html#ga556a180f43cab22649c23ada36a8a139)

- [Moments Structure](https://docs.opencv.org/4.5.3/d8/d23/classcv_1_1Moments.html#a8b1b4917d1123abc3a3c16b007a7319b)

- [Ellipse Detection Opencv Python](https://stackoverflow.com/questions/42206042/ellipse-detection-in-opencv-python)

- [skimage Hough Ellipse](https://www.kite.com/python/docs/skimage.transform.hough_ellipse)

- [Blobs with Detector](https://learnopencv.com/blob-detection-using-opencv-python-c/)

- [Detector Docs](https://docs.opencv.org/4.5.3/d0/d7a/classcv_1_1SimpleBlobDetector.html)