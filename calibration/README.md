# Opencv Calibration Sample Code Customization

Now, we will customize OpenCV's original [calibration.cpp](https://github.com/the-other-mariana/vision/blob/master/calibration/opencv-sources-samples-cpp/calibration.cpp) file so that we can adjust to the method and pattern we will use. This file can be found at: `C:\OpenCV-4.2.0\opencv\sources\samples\cpp`, while all the example input images and files are found at: `C:\OpenCV-4.2.0\opencv\sources\samples\data`.

Source: https://github.com/the-other-mariana/vision/blob/master/calibration/calibration/calibration/calibration.cpp

### Run the Code

1. Open a C++ Visual Studio project.

2. Right click over the solution file and go to Project Properties.

3. Select Debugging.

4. Type in Command Arguments text box what would de the cmd command line of the .exe:

```
$ -w=6 -h=8 -s=0.025 -o=camera.yml -op -oe data/calibration.yml
```

## Error Scatter Plot

By running the calibration sample with the opencv input images, we get a view:

![img](https://github.com/the-other-mariana/vision/blob/master/calibration/res/view.png?raw=true)

And if we plot the error scatter with a scale factor of 1000:

![img](https://github.com/the-other-mariana/vision/blob/master/calibration/res/plot.png?raw=true)

## How does OpenCV Calibrates

### Hierarchical Clustering

![img](res/hc.png)

circlesgrid.cpp (line 125):

```c++
//the largest cluster can have more than pn points -- we need to filter out such situations
if(clusters[patternClusterIdx].size() != static_cast<size_t>(patternSz.area()))
{
    return;
}
```

Then in the function:

```c++
void CirclesGridClusterFinder::getSortedCorners()
```

it seems that the sorting is based on distances from the centers to the corners found (squares' corners) and then they use that distance to know the orientation of the lines (clockwise / counterclock wise) and they find the `firstCorner` (line 329). They search for this `firstCorner` inside the convex hull of all the points, they start a loop from that index and over the convex hull sorted points, and they push them to the final sortedCorners vector (line 339).

To our pattern, it seems like also this function is useful:

```c++
bool CirclesGridFinder::findHoles()
```

## Customizing OpenCV

1. Create a Visual Studio Project and configure it for OpenCV. [[Tutorial Here]](https://github.com/the-other-mariana/image-processing-algorithms#usage)

2. Go to OpenCV source folder and grab the function `findCirclesGrid()` from `calibinit.cpp` at `C:\OpenCV-4.2.0\opencv\sources\modules\calib3d\src\calibinit.cpp` that is at line 2183:

```c++
bool findCirclesGrid( InputArray _image, Size patternSize,
                          OutputArray _centers, int flags, const Ptr<FeatureDetector> &blobDetector,
                          const CirclesGridFinderParameters& parameters_)
{
    CV_INSTRUMENT_REGION();

    CirclesGridFinderParameters parameters = parameters_; // parameters.gridType is amended below

    bool isAsymmetricGrid = (flags & CALIB_CB_ASYMMETRIC_GRID) ? true : false;
    bool isSymmetricGrid  = (flags & CALIB_CB_SYMMETRIC_GRID ) ? true : false;
    CV_Assert(isAsymmetricGrid ^ isSymmetricGrid);

    Mat image = _image.getMat();
    std::vector<Point2f> centers;

    std::vector<KeyPoint> keypoints;
    blobDetector->detect(image, keypoints);
    std::vector<Point2f> points;
    for (size_t i = 0; i < keypoints.size(); i++)
    {
      points.push_back (keypoints[i].pt);
    }

    if(flags & CALIB_CB_ASYMMETRIC_GRID)
      parameters.gridType = CirclesGridFinderParameters::ASYMMETRIC_GRID;
    if(flags & CALIB_CB_SYMMETRIC_GRID)
      parameters.gridType = CirclesGridFinderParameters::SYMMETRIC_GRID;

    if(flags & CALIB_CB_CLUSTERING)
    {
      CirclesGridClusterFinder circlesGridClusterFinder(parameters);
      circlesGridClusterFinder.findGrid(points, patternSize, centers);
      Mat(centers).copyTo(_centers);
      return !centers.empty();
    }

    const int attempts = 2;
    const size_t minHomographyPoints = 4;
    Mat H;
    for (int i = 0; i < attempts; i++)
    {
      centers.clear();
      CirclesGridFinder boxFinder(patternSize, points, parameters);
      bool isFound = false;
#define BE_QUIET 1
#if BE_QUIET
      void* oldCbkData;
      ErrorCallback oldCbk = redirectError(quiet_error, 0, &oldCbkData); // FIXIT not thread safe
#endif
      try
      {
        isFound = boxFinder.findHoles();
      }
      catch (const cv::Exception &)
      {

      }
#if BE_QUIET
      redirectError(oldCbk, oldCbkData);
#endif
      if (isFound)
      {
        switch(parameters.gridType)
        {
          case CirclesGridFinderParameters::SYMMETRIC_GRID:
            boxFinder.getHoles(centers);
            break;
          case CirclesGridFinderParameters::ASYMMETRIC_GRID:
        boxFinder.getAsymmetricHoles(centers);
        break;
          default:
            CV_Error(Error::StsBadArg, "Unknown pattern type");
        }

        if (i != 0)
        {
          Mat orgPointsMat;
          transform(centers, orgPointsMat, H.inv());
          convertPointsFromHomogeneous(orgPointsMat, centers);
        }
        Mat(centers).copyTo(_centers);
        return true;
      }

      boxFinder.getHoles(centers);
      if (i != attempts - 1)
      {
        if (centers.size() < minHomographyPoints)
          break;
        H = CirclesGridFinder::rectifyGrid(boxFinder.getDetectedGridSize(), centers, points, points);
      }
    }
    Mat(centers).copyTo(_centers);
    return false;
}
```

As well as the headers, 

```c++
#include "precomp.hpp"
#include "circlesgrid.hpp"
#include <opencv2/core/utils/logger.defines.hpp>
//#undef CV_LOG_STRIP_LEVEL
//#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#include <opencv2/core/utils/logger.hpp>

#include <stack>

// ADD THIS ONE:
using namespace cv;
```

Plus the function definition for `quiet_error` used in `findCirclesGrid`:

```c++
static int quiet_error(int /*status*/, const char* /*func_name*/,
    const char* /*err_msg*/, const char* /*file_name*/,
    int /*line*/, void* /*userdata*/)
{
    return 0;
}
```

3. Paste these 2 functions plus headers from `calibinit.cpp` into a Source.cpp from the Source Files folder of your project.

4. Copy the following files from `C:\OpenCV-4.2.0\opencv\sources\modules\calib3d\src`:

    - `precomp.hpp`

    - `circlesgrid.hpp`

5. Paste the files in a custom folder in your machine. For example I pasted them in a folder called `include` I created in the same folder where Source.cpp is at `C:\Users\mariana\Documents\github-mariana\vision\calibration\calibration\calibration\include`

6. Right click on your project > Project Properties > C/C++ > Additional Include Directories and paste the location of this `include` folder, for example, `C:\Users\mariana\Documents\github-mariana\vision\calibration\calibration\calibration\include`, where the .hpp files are.

7. Click OK.

8. Change the name of `findCirclesGrid()` function **at Source.cpp** to `findCirclesGridNew()`.

9. From this [link](https://github.com/opencv/opencv/blob/bf4c5bef7fa254e996f31d3e3d7f81ce63716c13/modules/core/include/opencv2/core/private.hpp#L538) copy the whole section:

```c++
///// General instrumentation
// General OpenCV region instrumentation macro
#define CV_INSTRUMENT_REGION()              CV_INSTRUMENT_REGION_META(cv::instr::TYPE_GENERAL, cv::instr::IMPL_PLAIN)
// Parallel OpenCV region instrumentation macro
#define CV_INSTRUMENT_REGION_MT()           CV_INSTRUMENT_REGION_MT_META(cv::instr::TYPE_GENERAL, cv::instr::IMPL_PLAIN)

///// IPP instrumentation
// Wrapper region instrumentation macro
#define CV_INSTRUMENT_REGION_IPP()          CV_INSTRUMENT_REGION_META(::cv::instr::TYPE_WRAPPER, ::cv::instr::IMPL_IPP)
// Function instrumentation macro
#define CV_INSTRUMENT_FUN_IPP(FUN, ...)     CV_INSTRUMENT_FUN_RT_META(::cv::instr::TYPE_FUN, ::cv::instr::IMPL_IPP, status < 0, FUN, __VA_ARGS__)
// Diagnostic markers
#define CV_INSTRUMENT_MARK_IPP(NAME)        CV_INSTRUMENT_MARK_META(::cv::instr::IMPL_IPP, NAME)

///// OpenCL instrumentation
// Wrapper region instrumentation macro
#define CV_INSTRUMENT_REGION_OPENCL()              CV_INSTRUMENT_REGION_META(::cv::instr::TYPE_WRAPPER, ::cv::instr::IMPL_OPENCL)
// Function instrumentation macro
#define CV_INSTRUMENT_FUN_OPENCL_KERNEL(FUN, ...)  CV_INSTRUMENT_FUN_RT_META(::cv::instr::TYPE_FUN, ::cv::instr::IMPL_OPENCL, status == 0, FUN, __VA_ARGS__)
// Diagnostic markers
#define CV_INSTRUMENT_MARK_OPENCL(NAME)            CV_INSTRUMENT_MARK_META(::cv::instr::IMPL_OPENCL, NAME)

#define CV_INSTRUMENT_REGION()
#define CV_INSTRUMENT_REGION_MT()

#define CV_INSTRUMENT_REGION_IPP()
#define CV_INSTRUMENT_FUN_IPP(FUN, ...) ((FUN)(__VA_ARGS__))
#define CV_INSTRUMENT_MARK_IPP(NAME)

#define CV_INSTRUMENT_REGION_OPENCL()
#define CV_INSTRUMENT_FUN_OPENCL_KERNEL(FUN, ...) ((FUN)(__VA_ARGS__))
#define CV_INSTRUMENT_MARK_OPENCL(NAME)
```

And paste it in Source.cpp below the headers, so that `CV_INSTRUMENT_REGION();` used in `findCirclesGridNew()` is defined.