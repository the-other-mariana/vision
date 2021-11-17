#include "precomp.hpp"
#include "circlesgrid.hpp"
#include <opencv2/core/utils/logger.defines.hpp>
#include <math.h>
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

float euclideanDistance(Point2f p1, Point2f p2) {
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}

std::vector<KeyPoint> detectNew(const Ptr<FeatureDetector>& blobDetector, Mat image, std::vector<KeyPoint> keypoints) {
    const int w = image.cols;
    const int h = image.rows;
    const int hCircles = 8;
    const int vCircles = 6;
    
    int limit = 1600;
    
    Mat centroids(h, w, CV_8UC3, Scalar(255, 255, 255));
    Mat img_gray;
    Mat img_gray_inv;
    Mat binary;
    cvtColor(image, img_gray, COLOR_BGR2GRAY);
    GaussianBlur(img_gray, img_gray, Size(9, 9), 2, 2);
    threshold(img_gray, binary, 150, 255, THRESH_BINARY);

    // inner circles: blue
    SimpleBlobDetector::Params params1;
    params1.filterByColor = false;
    params1.minThreshold = 50;
    params1.maxThreshold = 255;
    params1.filterByArea = true;
    params1.maxArea = limit;
    params1.filterByCircularity = false;
    params1.filterByConvexity = false;
    params1.filterByInertia = false;
    std::vector<KeyPoint> pts_inner;
    Ptr<SimpleBlobDetector> detector1 = SimpleBlobDetector::create(params1);
    Point2f ip[vCircles * hCircles];
    Point2f all_centers[vCircles * hCircles * 2];

    bitwise_not(img_gray, img_gray_inv);
    detector1->detect(img_gray, pts_inner);
    for (int i = 0; i < pts_inner.size(); i++) {
        KeyPoint k = pts_inner[i];
        ip[i] = k.pt;
        all_centers[i] = k.pt;
        // comment this on center
        //circle(img_gray_inv, ip[i], k.size * 0.70, color, FILLED, 8, 0);
    }

    // outer circles: magenta
    SimpleBlobDetector::Params params2;
    params2.filterByColor = false;
    params2.minThreshold = 80;
    params2.maxThreshold = 255;
    params2.filterByArea = true;
    params2.minArea = limit;
    //params2.maxArea = 1500;
    params2.filterByCircularity = false;
    params2.filterByConvexity = false;
    params2.filterByInertia = false;
    std::vector<KeyPoint> pts_outer;
    Ptr<SimpleBlobDetector> detector2 = SimpleBlobDetector::create(params2);
    Point2f op[vCircles * hCircles];

    detector2->detect(img_gray_inv, pts_outer);
    for (int i = 0; i < pts_outer.size(); i++) {
        KeyPoint k = pts_outer[i];
        op[i] = k.pt;
        all_centers[(vCircles* hCircles) + i] = k.pt;
    }

    int eps = 10;
    for (int i = 0; i < (vCircles * hCircles * 2); i++) {
        Point2f c = all_centers[i];
        for (int j = 0; j < (vCircles * hCircles * 2); j++) {
            if (i == j) {
                continue;
            }
            float dist = euclideanDistance(c, all_centers[i]);
            if (dist < eps) {
                float avgX = (c.x + all_centers[i].x) / 2.0;
                float avgY = (c.y + all_centers[i].y) / 2.0;
                Point2f avg_center = Point2f(avgX, avgY);
                keypoints.push_back(KeyPoint(avg_center, 1.f));
            }
        }
    }
    return keypoints;
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
    // original line
    //blobDetector->detect(image, keypoints);
    // added line
    keypoints = detectNew(blobDetector, image, keypoints);
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