#ifndef __POINT_HPP__
#define __POINT_HPP__

#include <vector>
#include <opencv2/opencv.hpp>

class Point {
public:
    double x;
    double y;

    Point ();
    Point (double x, double y);
    Point operator+(Point p);
    Point operator-(Point p);
    Point& operator+=(const Point& p);
    Point& operator-=(const Point& p);
    
    double norm();
    double abs();
    double dot(Point p);
    double cross(Point p);
    double distanceVertex(Point p);
    double distanceVector(Point a, Point b);
    double angle(Point prev, Point next);
    bool isContain(std::vector<Point> g);
    bool isIntersect(Point b, Point c, Point d);
    bool isAdjacent(Point b, Point c, Point d);
    cv::Point convert_cvPoint();
};

typedef Point Vector;

#endif
