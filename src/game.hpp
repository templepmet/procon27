#ifndef __GAME_HPP__
#define __GAME_HPP__

#include "piece.hpp"
#include "timer.hpp"
#include "camera.hpp"
#include "projector.hpp"
#include <vector>
#include <opencv2/opencv.hpp>

class Game {
public:
    cv::Mat src;
    std::vector<Piece> src_piece;
    std::vector<Piece> piece;
    std::vector<Piece> frame;
    std::vector<Piece> answer;
    cv::Point2f outframe[4];
    Timer timer;
    
    void run();
    void input();
    void solve();
    void output();
    void projectionMapping();
};

#endif
