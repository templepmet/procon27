#include <iostream>
#include <fstream>
#include "defines.hpp"
#include "camera.hpp"
#include "projector.hpp"
#include <opencv2/opencv.hpp>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;
int kbhit(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

int main() {
    ifstream ifs("../calibration/vertices.txt");

    if (!ifs) {
        cerr << "Failed to open vertices.txt" << endl;
        exit(1);
    }

    cv::Point2f before[4], after[4];
    for (int i = 0; i < 4; ++i) {
        ifs >> before[i].x >> before[i].y;
    }
    ifs.close();
    after[0] = cv::Point2f(0, 0);
    after[1] = cv::Point2f(CAPTURE_WIDTH, 0);
    after[2] = cv::Point2f(0, CAPTURE_HEIGHT);
    after[3] = cv::Point2f(CAPTURE_WIDTH, CAPTURE_HEIGHT);

    Camera camera;
    Projector projector;
    int number;
    char ch;
    cv::Mat src, dst, homography;
    while (1) {
        cout << "\nPlease Select adjustment Point number:";
        cin >> number;
        if (number < 0) {
            break;
        }
        while (1) {
            if (kbhit()) {
                ch = getchar();
                if (ch == 'w') {
                    before[number].y++;
                }
                else if (ch == 'd') {
                    before[number].x--;
                }
                else if (ch == 's') {
                    before[number].y--;
                }
                else if (ch == 'a') {
                    before[number].x++;
                }
                else if (ch == 'q') {
                    break;
                }
                
                homography = getPerspectiveTransform(before, after);
                src = camera.capture();
                warpPerspective(src, dst, homography, cv::Size(CAPTURE_WIDTH, CAPTURE_HEIGHT));
                imshow("projector", dst);
                cv::waitKey(1);
            }
        }
    }

    ofstream ofs("../calibration/vertices.txt");
    for (int i = 0; i < 4; ++i) {
        ofs << before[i].x << " " << before[i].y << endl;
    }
    
    return 0;
}
