#include "piece.hpp"
#include "point.hpp"
#include "utils.hpp"
#include "defines.hpp"
#include <vector>
#include <algorithm>
#include <cmath>
#include <opencv2/opencv.hpp>

using namespace std;

Piece::Piece() {}

Piece::Piece(vector<cv::Point> points) {
    int n = points.size();
    this->points.resize(n);
    for (int i = 0; i < n; ++i) {
        this->points[i].x = points[i].x;
        this->points[i].y = points[i].y;
    }
}

Piece::Piece(vector<Point> points) {
    int n = points.size();
    this->points.resize(n);
    for (int i = 0; i < n; ++i) {
        this->points[i].x = points[i].x;
        this->points[i].y = points[i].y;
    }
}

bool Piece::operator<(const Piece& right) const {
    int n = points.size();
    int m = right.points.size();
    if (n == 0) {
        return 0;
    }
    if (m == 0) {
        return 1;
    }

    double ly = INF, ry = INF;
    for (int i = 0; i < n; ++i) {
        ly = min(ly, points[i].y);
    }
    for (int i = 0; i < m; ++i) {
        ry = min(ry, right.points[i].y);
    }
    return ly < ry;
}

void Piece::init(bool isPiece, int id) {
    int n = points.size();
    length.resize(n);
    angle.resize(n);
    for (int i = 0; i < n; ++i) {
        length[i] = points[i].distanceVertex(points[(i + 1) % n]);
        angle[i] = points[i].angle(points[(i + n - 1) % n], points[(i + 1) % n]);
    }
    
    area = contourArea(convert_cvPoint());
    if (isPiece) {
        clockwise = false;
    }

    src_id = id;
}

void Piece::move(Vector v) {
    int n = points.size();
    for (int i = 0; i < n; ++i) {
        points[i] += v;
    }
}

void Piece::rotate(Piece& frame, int frame_center, int piece_center, bool clockwise_set) {
    Point center = frame.points[frame_center];
    int frame_vertices = frame.points.size();
    int piece_vertices = points.size();
    double angle;
    if (clockwise) {
        if (clockwise_set) {
            angle = center.angle(points[(piece_center+piece_vertices-1) % piece_vertices], frame.points[(frame_center+frame_vertices-1) % frame_vertices]);
        }
        else {
            angle = center.angle(points[(piece_center+1) % piece_vertices], frame.points[(frame_center+1) % frame_vertices]);
        }
    }
    else {
        if (clockwise_set) {
            angle = center.angle(points[(piece_center+1) % piece_vertices], frame.points[(frame_center+frame_vertices-1) % frame_vertices]);
        }
        else {
            angle = center.angle(points[(piece_center+piece_vertices-1) % piece_vertices], frame.points[(frame_center+1) % frame_vertices]);
        }
    }

    double sinr = sin(angle);
    double cosr = cos(angle);
    for (int i = 0; i < piece_vertices; ++i) {
        Point temp = points[i] - center;
        points[i].x = temp.x*cosr - temp.y*sinr + center.x;
        points[i].y = temp.x*sinr + temp.y*cosr + center.y;
    }
}

void Piece::reverse() {
    clockwise = !clockwise;
    for (int i = 0, n = points.size(); i < n; ++i) {
        points[i].x = CAPTURE_WIDTH + (CAPTURE_WIDTH - points[i].x);
    }
}

bool Piece::isOverlap(Piece& piece) {
    int n = piece.points.size();
    int m = points.size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; j++) {
            if (points[j].isIntersect(points[(j + 1) % m], piece.points[i], piece.points[(i + 1) % n])) {
                return true;
            }
        }
    }
    
    return false;
}
/*
void Piece::PieceAdd(Piece& piece, int frame_center, int piece_center, bool clockwise_set) {
    vector<Point> adder;
    int n = piece.points.size();
    
    if (piece.clockwise) {
        for (int i = piece_center - 1; i >= 0; --i) {
            adder.push_back(piece.points[i]);
        }
        for (int i = n - 1; i > piece_center; --i) {
            adder.push_back(piece.points[i]);
        }
    }
    else {
        for (int i = piece_center + 1; i < n; ++i) {
            adder.push_back(piece.points[i]);
        }
        for (int i = 0; i < piece_center; ++i) {
            adder.push_back(piece.points[i]);
        }
    }
    
    points.insert(points.begin() + frame_center + clockwise_set, adder.begin(), adder.end());
    area -= piece.area;
}
*/

void Piece::PieceAdd(Piece& piece, int frame_center, int piece_center, bool clockwise_set) {
    vector<Point> adder;
    int piece_vertices = piece.points.size();
    int frame_vertices = points.size();
    bool next_flag = true, prev_flag = true;
    int n = 0, p = 0, begin, end;
    
    if (piece.clockwise) {
        /*
        for (int i = piece_center - 1; i >= 0; --i) {
            adder.push_back(piece.points[i]);
        }
        for (int i = n - 1; i > piece_center; --i) {
            adder.push_back(piece.points[i]);
        }
        */
    }
    else {
        for (int i = 0; i < piece_vertices / 2 + 1; ++i) {
            if (next_flag) {
                if (piece.points[(piece_center+n) % piece_vertices].isAdjacent(piece.points[(piece_center+n+1) % piece_vertices], points[(frame_center+frame_vertices-n) % frame_vertices], points[(frame_center+frame_vertices-n-1) % frame_vertices])) {
                    n++;
                }
                else {
                    next_flag = false;
                }
            }

            if (prev_flag) {
                if (piece.points[(piece_center+piece_vertices-i) % piece_vertices].isAdjacent(piece.points[(piece_center+piece_vertices-i-1) % piece_vertices], points[(frame_center+i) % frame_vertices], points[(frame_center+i+1) % frame_vertices])) {
                    p++;   
                }
                else {
                    prev_flag = false;
                }
            }
        }
        
        for (int i = piece_center; i < piece_vertices; ++i) {
            adder.push_back(piece.points[i]);
        }
        for (int i = 0; i < piece_center; ++i) {
            adder.push_back(piece.points[i]);
        }
        
        begin = frame_center;
        if (!(n == 0 && p == 0) && (piece_center + n) % piece_vertices == (piece_center + piece_vertices - p) % piece_vertices) {
            cout << 0 << endl;
            points.erase(points.begin(), points.end());
            return;
        }
        else if (n > 1 || p > 1 || (n > 0 && p > 0)){
            begin = (frame_center+frame_vertices - (n > 0 ? n - 1 : n)) % frame_vertices;
            end = (frame_center + (p > 0 ? p - 1 : p)) % frame_vertices;
            
            if (n == 0 && p > 0) {
                cout << 0 << endl;
                begin = (begin - 1) % frame_vertices;
            }
            else if (n > 0 && p == 0) {
                cout << 1 << endl;
                begin = (begin + 1) % frame_vertices;
            }
            
            if (begin > end) {
                cout << 2 << endl;
                points.erase(points.begin()+begin, points.end());
                points.erase(points.begin(), points.begin()+end+1);
            }
            else {
                cout << 3 << endl;
                points.erase(points.begin()+begin, points.begin()+end+1);
            }
        }
    }
    points.insert(points.begin() + begin, adder.begin() + n, adder.end() - (p > 0 ? p - 1 : p));

    area-=piece.area;
}

vector<cv::Point> Piece::convert_cvPoint() {
    vector<cv::Point> points(this->points.size());
    int n = points.size();
    for (int i = 0; i < n; ++i) {
        points[i] = cv::Point(this->points[i].x, this->points[i].y);
    }
    return points;
}
