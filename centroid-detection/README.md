# Centroid Detection for Camera Calibration

## Pattern Generation

The first step is to generate a circle-based pattern for the camera to process. It contains one black circle and on top of it another white circle, 40% smaller. This has the purpose of being more accurate: the centroid detection will have two matrices of centroids: one for all the black circles' centers and the other for the white ones'. In this way, the final centroid would be an average of the two centroid matrices, and thus taking on account two circles per centroid instead of just one.

Source: https://github.com/the-other-mariana/vision/blob/master/centroid-detection/circle-pattern/circle-pattern/circle-pattern.cpp

![img](https://github.com/the-other-mariana/vision/blob/master/centroid-detection/circle-pattern/test.png)

## Centroid Detection

1. This step is to apply a Hough Transform to photographs of the printed pattern, in order to detect circles with a certain big range of radius (black circles), and those with the smaller range of radius (white circles). Then, their centroids are marked on a separate image to visualize the differences. This step works fine with perfect circles, but with the tilted photos, the circles were now ellipses, and where not detected appropiately.

    - Source (1): https://github.com/the-other-mariana/vision/blob/master/centroid-detection/circle-centroid-detection/circle-centroid-detection/circle-centroid-detection.cpp


2. The second option is to use the concept of Blobs, which is similar to Region Detection, and through an area threshold applied to the contours found on the photo, mark it as either an outer centroid or an inner centroid. This process seemed to work nice.

    - Source (2): https://github.com/the-other-mariana/vision/blob/master/centroid-detection/circle-centroid-detection-regions/circle-centroid-detection-regions/circle-centroid-detection-regions.cpp

    - What is a **blob**? A blob is a group of connected pixels that share a property, like the grayscale value. 

    - What is the **centroid** of a shape? It is the mean (average) of all the points, in this case pixels, in a shape.

    ![\Large c=\frac{1}{n}\sum_{i=1}^{n}x_{i}](https://latex.codecogs.com/svg.latex?\Large&space;c=\frac{1}{n}\sum_{i=1}^{n}x_{i})

3. The third option was to try and fit an ellipse to the found contours that once again fell in the estimated areas for outer and inner circles, and then mark the centers of both. This was similar to the previous step, which also works nice.

    - Source (3): https://github.com/the-other-mariana/vision/blob/master/centroid-detection/centroid-detection-ellipse/centroid-detection-ellipse/centroid-detection-ellipse.cpp

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