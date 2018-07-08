#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include <opencv2/opencv.hpp>

class Camera {
public:
    cv::VideoCapture cap;
    cv::Mat mtx;
    cv::Mat dist;

    Camera();

    cv::Mat capture();
};

#endif
