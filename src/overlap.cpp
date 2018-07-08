#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <vector>
#include "camera.hpp"
#include "projector.hpp"
#include "defines.hpp"

using namespace std;

int main() {
    Camera camera;
    Projector projector;
    cv::Mat frame, dst;
    vector<vector<cv::Point> > contours;
    
    while (1) {
        frame = camera.capture();
        imwrite("/home/owata/projector.jpg", frame);
        cvtColor(frame, dst, CV_BGR2GRAY);
        threshold(dst, dst, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
        findContours(dst, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
        for (int i = 0; i < contours.size(); ++i) {
            fillConvexPoly(frame, contours[i], cv::Scalar(0, 0, 255));
        }
        projector.projection(frame);
    }
    
    return 0;
}
