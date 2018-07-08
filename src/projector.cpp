#include "projector.hpp"
#include "defines.hpp"
#include <iostream>
#include <fstream>

using namespace std;

Projector::Projector() {
    cv::namedWindow("projector", 0);
    cv::moveWindow("projector", 0, 0);
    cv::setWindowProperty("projector", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);

    ifstream ifs("../calibration/vertices.txt");
    if (!ifs) {
        cerr << "Failed to open vertices.txt" << endl;
        exit(1);
    }
    
    cv::Point2f before[4], after[4];
    for (int i = 0; i < 4; ++i) {
        ifs >> before[i].x >> before[i].y;
    }
    after[0] = cv::Point2f(0, 0);
    after[1] = cv::Point2f(CAPTURE_WIDTH, 0);
    after[2] = cv::Point2f(0, CAPTURE_HEIGHT);
    after[3] = cv::Point2f(CAPTURE_WIDTH, CAPTURE_HEIGHT);
    homography = getPerspectiveTransform(before, after);
}

char Projector::projection(cv::Mat src) {
    cv::Mat dst;
    warpPerspective(src, dst, homography, cv::Size(CAPTURE_WIDTH, CAPTURE_HEIGHT));
    imshow("projector", dst);
    return cv::waitKey(1);
}
