#ifndef __PROJECTOR_HPP__
#define __PROJECTOR_HPP__

#include <opencv2/opencv.hpp>

class Projector {
public:
    cv::Mat homography;
    
    Projector();

    char projection(cv::Mat frame);
};

#endif
