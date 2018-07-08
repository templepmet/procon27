#include "point.hpp"
#include "defines.hpp"
#include <cmath>
#include <algorithm>

using namespace std;

Point::Point() {
    this->x = 0;
    this->y = 0;
}

Point::Point(double x, double y) {
    this->x = x;
    this->y = y;
}

Point Point::operator+(Point p) {
    return Point(x + p.x, y + p.y);
}

Point Point::operator-(Point p) {
    return Point(x - p.x, y - p.y);
}

Point& Point::operator+=(const Point& p) {
    x += p.x;
    y += p.y;
    return *this;
}

Point& Point::operator-=(const Point& p) {
    x -= p.x;
    y -= p.y;
    return *this;
}

double Point::norm() {
    return x * x + y * y;
}

double Point::abs() {
    return sqrt(norm());
}

double Point::dot(Point p) {
    return x * p.x + y * p.y;
}

double Point::cross(Point p) {
    return x * p.y - y * p.x;
}

double Point::distanceVertex(Point p) {
    return sqrt((x - p.x)*(x - p.x) + (y - p.y)*(y - p.y));
}

double Point::distanceVector(Point a, Point b) {
    Vector ab = b - a;
    Vector ap = *this - a;
    return std::abs(ab.cross(ap)) / a.distanceVertex(b);
}

double Point::angle(Point prev, Point next) {
    Point a = prev - *this;
    Point b = next - *this;
    return atan2(a.x*b.y - a.y*b.x, a.x*b.x + a.y*b.y);
}

bool Point::isContain(vector<Point> g) {
    int n = g.size();
    bool contain = false;
    for (int i = 0; i < n; ++i) {
        Point a = g[i] - *this;
        Point b = g[(i + 1) % n] - *this;
        if (a.y > b.y) {
            swap(a, b);
        }
        if (a.y < CONTAIN_EPS && CONTAIN_EPS < b.y && a.cross(b) > CONTAIN_EPS) {
            contain = !contain;
        }
    }
    return contain;
}

bool Point::isIntersect(Point b, Point c, Point d) {
    double ta = (c.x - d.x) * (y - c.y) + (c.y - d.y) * (c.x - x);
    double tb = (c.x - d.x) * (b.y - c.y) + (c.y - d.y) * (c.x - b.x);
    double tc = (x - b.x) * (c.y - y) + (y - b.y) * (x - c.x);
    double td = (x - b.x) * (d.y - y) + (y - b.y) * (x - d.x);

    return (ta * tb < 0 && tc * td < 0);
}

bool Point::isAdjacent(Point b, Point c, Point d) {
    double ret1 = this->distanceVector(c, d);
    double ret2 = b.distanceVector(c, d);
    if (ret1 <= ADJACENT_EPS && ret2 <= ADJACENT_EPS) {
        return true;
    }
    
    ret1 = c.distanceVector(*this, b);
    ret2 = d.distanceVector(*this, b);
    if (ret1 <= ADJACENT_EPS && ret2 <= ADJACENT_EPS) {
        return true;
    }
    
    return false;
}

cv::Point Point::convert_cvPoint() {
    return cv::Point(x, y);
}
