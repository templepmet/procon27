#ifndef __PIECE_HPP__
#define __PIECE_HPP__

#include "point.hpp"

class Piece {
public:
    std::vector<Point> points;
    std::vector<double> length;
    std::vector<double> angle;
    double area;
    bool clockwise;
    int src_id;

    Piece();
    Piece(std::vector<cv::Point> points);
    Piece(std::vector<Point> points);

    bool operator<(const Piece& right) const;
    
    void init(bool isPiece, int id);
    void move(Vector v);
    //void rotate(Point& center, double angle);
    void rotate(Piece& frame, int frame_center, int piece_center, bool clockwise_set);
    void reverse();
    bool isOverlap(Piece& piece);
    void PieceAdd(Piece& piece, int frame_center, int piece_center, bool clockwise_set);
    std::vector<cv::Point> convert_cvPoint();
};

#endif
