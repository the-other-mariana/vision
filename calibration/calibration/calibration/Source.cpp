#include "precomp.hpp"
#include "circlesgrid.hpp"
#include <opencv2/core/utils/logger.defines.hpp>
//#undef CV_LOG_STRIP_LEVEL
//#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#include <opencv2/core/utils/logger.hpp>

#include <stack>

using namespace cv;

// https://github.com/opencv/opencv/blob/bf4c5bef7fa254e996f31d3e3d7f81ce63716c13/modules/core/include/opencv2/core/private.hpp#L538
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

static int quiet_error(int /*status*/, const char* /*func_name*/,
    const char* /*err_msg*/, const char* /*file_name*/,
    int /*line*/, void* /*userdata*/)
{
    return 0;
}

bool findCirclesGridNew(InputArray _image, Size patternSize,
    OutputArray _centers, int flags, const Ptr<FeatureDetector>& blobDetector,
    const CirclesGridFinderParameters& parameters_)
{
    CV_INSTRUMENT_REGION();

    CirclesGridFinderParameters parameters = parameters_; // parameters.gridType is amended below

    bool isAsymmetricGrid = (flags & CALIB_CB_ASYMMETRIC_GRID) ? true : false;
    bool isSymmetricGrid = (flags & CALIB_CB_SYMMETRIC_GRID) ? true : false;
    CV_Assert(isAsymmetricGrid ^ isSymmetricGrid);

    Mat image = _image.getMat();
    std::vector<Point2f> centers;

    std::vector<KeyPoint> keypoints;
    // here
    blobDetector->detect(image, keypoints);
    std::vector<Point2f> points;
    for (size_t i = 0; i < keypoints.size(); i++)
    {
        points.push_back(keypoints[i].pt);
    }

    if (flags & CALIB_CB_ASYMMETRIC_GRID)
        parameters.gridType = CirclesGridFinderParameters::ASYMMETRIC_GRID;
    if (flags & CALIB_CB_SYMMETRIC_GRID)
        parameters.gridType = CirclesGridFinderParameters::SYMMETRIC_GRID;

    if (flags & CALIB_CB_CLUSTERING)
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
        catch (const cv::Exception&)
        {

        }
#if BE_QUIET
        redirectError(oldCbk, oldCbkData);
#endif
        if (isFound)
        {
            switch (parameters.gridType)
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