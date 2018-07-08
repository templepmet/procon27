#include "camera.hpp"
#include "defines.hpp"
#include <iostream>

using namespace std;

Camera::Camera() {
    cap.open(0);
    cap.set(CV_CAP_PROP_FRAME_WIDTH, CAPTURE_WIDTH);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, CAPTURE_HEIGHT);

    mtx = cv::Mat(3, 3, CV_64F);
    dist = cv::Mat(8, 1, CV_64F);
    cv::FileStorage fs("../calibration/calibration.yml", cv::FileStorage::READ);
    if (!fs.isOpened()) {
        cerr << "Failed to open calibration.yml" << endl;
        exit(1);
    }
    else {
        fs["mtx"] >> mtx;
        fs["dist"] >> dist;
        fs.release();
    }
    for (int i = 0; i < 2; ++i) {
        capture();
    }
}

cv::Mat Camera::capture() {
    cv::Mat frame, undistorted;
    cap >> frame;
    undistort(frame, undistorted, mtx, dist);
    return undistorted;
}
