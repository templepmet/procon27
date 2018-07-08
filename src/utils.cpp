#include "utils.hpp"
#include "defines.hpp"
#include <cstdio>
#include <algorithm>

using namespace std;

void printMat(cv::Mat src) {
    cv::namedWindow("window", 0);
    cv::moveWindow("window", CAPTURE_WIDTH, 0);
    cv::resizeWindow("window", CAPTURE_WIDTH, CAPTURE_HEIGHT);
    
    imshow("window", src);
    cv::waitKey(0);
}

void printPiece(Piece piece) {
    cv::Mat dst(cv::Size(CAPTURE_WIDTH, CAPTURE_HEIGHT), CV_8UC3, cv::Scalar(255, 255, 255));
    polylines(dst, piece.convert_cvPoint(), true, cv::Scalar(0, 255, 0), 2);
    for (int i = 0, n = piece.points.size(); i < n; ++i) {
        circle(dst, piece.convert_cvPoint()[i], 3, cv::Scalar(0, 0, 255), -1);
        //putText(dst, to_string(i), piece.convert_cvPoint()[i], cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 0, 255), 1, CV_AA);
    }
    printMat(dst);
}
