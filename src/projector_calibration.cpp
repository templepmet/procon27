#include "camera.hpp"
#include "projector.hpp"
#include "defines.hpp"
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

int main(int argc, char* argv[]) {
    Camera camera;
    Projector projector;
    cv::Mat frame, binary;
    cv::Mat white(cv::Size(CAPTURE_WIDTH, CAPTURE_HEIGHT), CV_8UC3, cv::Scalar(255, 255, 255));
    vector<cv::Point> vertices(4);
    
    for (int i = 0; i < 10; ++i) {
        imshow("projector", white);
        cv::waitKey(1);
        frame = camera.capture();
        
        cvtColor(frame, binary, CV_BGR2GRAY);
        threshold(binary, binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
        
        vector<vector<cv::Point>> contours, approx;
        int maxs = -1, n;
        double maxarea = 0, area;
        findContours(binary, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
        n = contours.size();
        approx.resize(n);
        for (int j = 0; j < n; ++j) {
            approxPolyDP(cv::Mat(contours[j]), approx[j], APPROX_FACTOR * arcLength(contours[j], true), true);
            if (approx[j].size() == 4 && (area = contourArea(approx[j])) > maxarea && area > CAPTURE_WIDTH * CAPTURE_HEIGHT / 2) {
                maxarea = area;
                maxs = j;
            }
        }

        if (maxs != -1) {
            vertices = approx[maxs];
            break;
        }
    }

    auto compy = [](const cv::Point& left, const cv::Point& right){ return left.y < right.y; };
    auto compx = [](const cv::Point& left, const cv::Point& right){ return left.x < right.x; };
    sort(vertices.begin(), vertices.end(), compy);
    sort(vertices.begin(), vertices.begin() + 2, compx);
    sort(vertices.begin() + 2, vertices.end(), compx);

    ofstream ofs("../calibration/vertices.txt");
    for (auto v : vertices) {
        ofs << v.x << " " << v.y << endl;
        cout << v.x << " " << v.y << endl;
    }
    
    return 0;
}
