#include "camera.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;

int main() {
    Camera camera;
    cv::namedWindow("preview", 0);
    cv::setWindowProperty("preview", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);

    char key;
    while (1) {
        imshow("preview", camera.capture());
        key = cv::waitKey(1);
        if (key == 'q') {
            break;
        }
    }
    
    return 0;
}
