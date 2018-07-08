#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include "camera.hpp"
#include "projector.hpp"
#include "defines.hpp"

using namespace std;

Projector projector;
cv::VideoCapture cap(0);

void capture_calibration_image() {
    cout << "calibration capture Start" << endl;
        
    cv::Mat frame, black(cv::Size(CAPTURE_WIDTH, CAPTURE_HEIGHT), CV_8UC3, cv::Scalar(0, 0, 0));
    int n = 0;
    char key;
    while (1) {
        cap >> frame;
        imshow("frame", frame);
        imshow("projector", black);
        key = cv::waitKey(1);

        if (key == 'c') {
            imwrite("../calibration/" + to_string(n++) + ".jpg", frame);
        }
        else if (key == 'q') {
            break;
        }
    }
}

int main() {
    cap.set(CV_CAP_PROP_FRAME_WIDTH, CAPTURE_WIDTH);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, CAPTURE_HEIGHT);
    
    vector<vector<cv::Point3f>> object_points;
    vector<vector<cv::Point2f>> image_points;
    vector<cv::Point3f> obj;
    for (int i = 0; i < CHESSBOARD_WIDTH * CHESSBOARD_HEIGHT; ++i) {
        obj.push_back(cv::Point3f(i / CHESSBOARD_WIDTH, i % CHESSBOARD_WIDTH, 0.0f));
    }

    int n = 0;
    while (1) {
        cv::Mat frame = cv::imread("../calibration/" + to_string(n) + ".jpg");
        if (frame.empty()) {
            if (n == 0) {
                cerr << "calibration image is not found" << endl;
                capture_calibration_image();
                continue;
            }
            break;
        }
        
        cv::Mat gray;
        cvtColor(frame, gray, CV_BGR2GRAY);
        vector<cv::Point2f> centers;
        bool found = findChessboardCorners(gray, cv::Size(CHESSBOARD_WIDTH, CHESSBOARD_HEIGHT), centers);
        
        if (found) {
            cornerSubPix(gray, centers, cv::Size(11,11), cv::Size(-1,-1), cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.1));
            object_points.push_back(obj);
            image_points.push_back(centers);
            
            cout << n << ".jpg chessboard found" << endl;
        }
        else {
            cout << n << ".jpg chessboard not found" << endl;
        }
        n++;
    }

    vector<cv::Mat> rvecs, tvecs;
    cv::Mat mtx(3, 3, CV_64F);
    cv::Mat dist(8, 1, CV_64F);
    calibrateCamera(object_points, image_points, cv::Size(CAPTURE_WIDTH, CAPTURE_HEIGHT), mtx, dist, rvecs, tvecs);

    cv::FileStorage fs("../calibration/calibration.yml", cv::FileStorage::WRITE);
    fs << "mtx" << mtx;
    fs << "dist" << dist;
    fs.release();

    return 0;
}
