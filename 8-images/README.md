# Analyzing 8 Images

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

To make the test, we use the images created at the last operation located in the folder https://github.com/the-other-mariana/vision/tree/master/phase-shifting/src, and according to the theory, the image output in this particular case would be a black and white gradient in the horizontal axis.

## Output

Source code: https://github.com/the-other-mariana/vision/blob/master/8-images/src/src/src.cpp

![img](https://github.com/the-other-mariana/vision/blob/master/8-images/src/result.png?raw=true)

## Handy Links

- https://try2explore.com/questions/12110895

- https://stackoverflow.com/questions/26502912/opencv-mat-for-integer-types-only