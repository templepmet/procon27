#include <iostream>
#include "camera.hpp"
#include "projector.hpp"
#include <opencv2/opencv.hpp>
#include "defines.hpp"

using namespace std;

int main() {
    Camera camera;
    Projector projector;
    cv::Mat src, binary, line(cv::Size(CAPTURE_WIDTH, CAPTURE_HEIGHT), CV_8UC3, cv::Scalar(255, 255, 255)), vertices = line.clone();
    for (int i = 0; i < 10; ++i) {
        projector.projection(cv::Mat(cv::Size(CAPTURE_WIDTH, CAPTURE_HEIGHT), CV_8UC3, cv::Scalar(0, 0, 0)));
        src = camera.capture();
    }
    imwrite("/home/owata/src.jpg", src);
    cvtColor(src, binary, CV_BGR2GRAY);
    threshold(binary, binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    imwrite("/home/owata/binary.jpg", binary);

    vector<vector<cv::Point> > contours;
    vector<cv::Vec4i> hierarkey;
    findContours(binary, contours, hierarkey, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);
    
    for (int i = 0, n = contours.size(); i < n; ++i) {
        if (contourArea(contours[i]) < AREA_LIMIT) {
            continue;
        }

        vector<cv::Point> approx;
        approxPolyDP(cv::Mat(contours[i]), approx, APPROX_FACTOR * arcLength(contours[i], true), true);
        
        //描画
        polylines(line, approx, true, cv::Scalar(0, 255, 0), 5);
        polylines(vertices, approx, true, cv::Scalar(0, 255, 0), 5);
        //putText(src, to_string(i), contours[i][0], cv::FONT_HERSHEY_SIMPLEX, 1.2, cv::Scalar(0, 0, 255), 2, CV_AA);
        for (int j = 0, m = approx.size(); j < m; ++j) {
            circle(vertices, approx[j], 5, cv::Scalar(0, 0, 255), -1);
            //putText(src, to_string(j), approx[j], cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 0, 255), 1, CV_AA);
        }
    }
    imwrite("/home/owata/line.jpg", line);
    imwrite("/home/owata/vertices.jpg", vertices);

    return 0;
}
