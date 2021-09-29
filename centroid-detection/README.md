# Centroid Detection for Camera Calibration

## Pattern Generation

The first step is to generate a circle-based pattern for the camera to process. It contains one black circle and on top of it another white circle, 40% smaller. This has the purpose of being more accurate: the centroid detection will have two matrices of centroids: one for all the black circles' centers and the other for the white ones'. In this way, the final centroid would be an average of the two centroid matrices, and thus taking on account two circles per centroid instead of just one.

Source:

![img]()