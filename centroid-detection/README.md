# Centroid Detection for Camera Calibration

## Pattern Generation

The first step is to generate a circle-based pattern for the camera to process. It contains one black circle and on top of it another white circle, 40% smaller. This has the purpose of being more accurate: the centroid detection will have two matrices of centroids: one for all the black circles' centers and the other for the white ones'. In this way, the final centroid would be an average of the two centroid matrices, and thus taking on account two circles per centroid instead of just one.

Source: https://github.com/the-other-mariana/vision/blob/master/centroid-detection/circle-pattern/circle-pattern/circle-pattern.cpp

![img](https://github.com/the-other-mariana/vision/blob/master/centroid-detection/circle-pattern/test.png)

## Handy Links

- [OpenCV Flags](https://stackoverflow.com/questions/22547416/open-cv-flags-dont-work)

- [OpenCV Flags 3.0](https://stackoverflow.com/questions/10344246/how-can-i-convert-a-cvmat-to-a-gray-scale-in-opencv)

- [Hough Transform Doc](https://docs.opencv.org/4.5.1/dd/d1a/group__imgproc__feature.html#ga47849c3be0d0406ad3ca45db65a25d2d)

- [Common Aspect Ratios](https://en.wikipedia.org/wiki/List_of_common_resolutions)

- [Hough Example](https://stackoverflow.com/questions/39630077/using-opencv-and-hough-transform-circle-to-detect-circles-subscript-error)