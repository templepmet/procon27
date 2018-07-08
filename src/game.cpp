#include "game.hpp"
#include "utils.hpp"
#include "defines.hpp"
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void Game::run() {
    input();
    solve();
    output();
    //projectionMapping();
}

void Game::input() {
    cv::Mat binary;
    Camera camera;
    src = camera.capture();
    timer.restart();
    
    cvtColor(src, binary, CV_BGR2GRAY);
    threshold(binary, binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    
    vector<vector<cv::Point> > contours;
    vector<cv::Vec4i> hierarkey;
    findContours(binary, contours, hierarkey, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);

    auto compy = [](const cv::Point2f& left, const cv::Point2f& right){ return left.y < right.y; };
    auto compx = [](const cv::Point2f& left, const cv::Point2f& right){ return left.x < right.x; };
    
    for (int i = 0, n = contours.size(); i < n; ++i) {
        if (contourArea(contours[i]) < AREA_LIMIT) {
            continue;
        }

        vector<cv::Point> approx;
        approxPolyDP(cv::Mat(contours[i]), approx, APPROX_FACTOR * arcLength(contours[i], true), true);

        polylines(src, approx, true, cv::Scalar(0, 255, 0), 2);
        for (int j = 0, m = approx.size(); j < m; ++j) {
            circle(src, approx[j], 3, cv::Scalar(0, 0, 255), -1);
        }
        //printPiece(Piece(approx));
        
        if (hierarkey[i][2] > 0) { //枠の輪郭
            if (approx.size() == 4) {
                for (int j = 0; j < 4; ++j) {
                    outframe[j] = approx[j];
                }
                sort(outframe, outframe + 4, compy);
                sort(outframe, outframe + 2, compx);
                sort(outframe + 2, outframe + 4, compx);
            }
        }
        else if (hierarkey[i][3] > 0) { //枠の内側
            frame.push_back(Piece(approx));
        }
        else { //ピース
            src_piece.push_back(Piece(approx));
        }
    }

    answer.resize(src_piece.size());
    for (int i = 0, n = src_piece.size(); i < n; ++i) {
        src_piece[i].init(true, i);
    }
    for (int i = 0, n = frame.size(); i < n; ++i) {
        frame[i].init(false, i);
    }
    piece = src_piece;
    
    //printMat(src);
}

void Game::solve() {
    for (int i = 0, pieces = piece.size(); i < pieces; ++i) {
        //for (int b = 0; b < 2; ++b) {
            for (int j = 0, frames = frame.size(); j < frames; ++j) {
                if (piece[i].area > frame[j].area) {
                    continue;
                }
                for (int k = 0, frame_vertices = frame[j].points.size(); k < frame_vertices; ++k) {
                    for (int l = 0, piece_vertices = piece[i].points.size(); l < piece_vertices; ++l) {
                        piece[i].move(frame[j].points[k] - piece[i].points[l]);
                        //for (int clockwise_set = 0; clockwise_set < 2; ++clockwise_set) {
                        piece[i].rotate(frame[j], k, l, /*clockwise_set*/1);
                            if (!frame[j].isOverlap(piece[i])) {
                                answer[i] = piece[i];
                                printPiece(frame[0]);
                                frame[j].PieceAdd(piece[i], k, l, /*!clockwise_set*/1);
                                goto FIN;
                            }
                            //}
                    }
                }
            }
            //   piece[i].reverse();
            //}
    FIN:;
    }
}

void Game::output() {
    int ans = 0;
    cv::Point2f after[4] = { {0, 0}, {PERSPECTIVE_WIDTH, 0}, {0, PERSPECTIVE_HEIGHT}, {PERSPECTIVE_WIDTH, PERSPECTIVE_HEIGHT} };
    cv::Mat homography = getPerspectiveTransform(outframe, after);
    cv::Mat dst = src.clone();

    for (int i = 0, n = answer.size(); i < n; ++i) {
        for (int j = 0, m = answer[i].points.size(); j < m; ++j) {
            if (j == 0) {
                fillConvexPoly(dst, answer[i].convert_cvPoint(), cv::Scalar(255, 0, 0));
                polylines(dst, answer[i].convert_cvPoint(), true, cv::Scalar(0, 255, 0), 2);
                ans++;
            }
            circle(dst, cv::Point(answer[i].points[j].x, answer[i].points[j].y), 2, cv::Scalar(0, 0, 255), -1);
        }
    }
    
    warpPerspective(dst, dst, homography, cv::Size(PERSPECTIVE_WIDTH, PERSPECTIVE_HEIGHT));

    timer.print();
    cout << "Piece:" << piece.size() << endl;
    cout << "Frame:" << frame.size() << endl;
    cout << "Answer:" << ans << endl;
    printMat(dst);
}

void Game::projectionMapping() {
    Projector projector;
    cv::Mat dst(cv::Size(CAPTURE_WIDTH, CAPTURE_HEIGHT), CV_8UC3, cv::Scalar(0, 0, 0));
    char key;

    sort(answer.begin(), answer.end());
    
    for (int i = 0, n = piece.size(); i < n; ++i) {
        if (answer[i].points.size() == 0) {
            break;
        }
        
        fillConvexPoly(dst, answer[i].convert_cvPoint(), cv::Scalar(0, 255, 0));
        fillConvexPoly(dst, src_piece[answer[i].src_id].convert_cvPoint(), cv::Scalar(0, 0, 255));
        polylines(dst, answer[i].convert_cvPoint(), true, cv::Scalar(0, 0, 255), 2);
        while (1) {
            key = projector.projection(dst);
            if (key == 'n') {
                break;
            }
        }
        fillConvexPoly(dst, src_piece[answer[i].src_id].convert_cvPoint(), cv::Scalar(0, 0, 0));
    }
}
