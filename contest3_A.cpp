#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdint>
#include <unordered_set>
#include <cmath>

const double EPSILON = 1E-8;

class Point {
public:

    /* default constructor */
    Point() : Point(0, 0) { }

    /* destructor */
    ~Point() = default;

    /* copy constructor */
    Point(const Point& other) {
        x_ = other.x_;
        y_ = other.y_;
    }

    /* move constructor */
    Point(Point&& other)  noexcept {
        x_ = other.x_;
        y_ = other.y_;
        other.x_ = 0;
        other.x_ = 0;
    }

    /* copy assignment operator */
    Point& operator = (const Point& other) = default;

    /* move assignment operator */
    Point& operator = (Point&& other)  noexcept {
        if (&other != this) {
            x_ = other.x_;
            y_ = other.y_;
            other.x_ = 0;
            other.x_ = 0;
        }
        return *this;
    }

    Point(double x, double y) : x_(x), y_(y) { }

    /* >> overloaded operator */
    friend std::istream& operator >> (
            std::istream& in,
            Point& segment
    ) {
        in >> segment.x_ >> segment.y_;
        return in;
    }

    double getX() {
        return x_;
    }

    double getY() {
        return y_;
    }

protected:

    double x_;
    double y_;

};

class LineSegment {
public:

    LineSegment() : LineSegment(0, 0, 0, 0) {}
    LineSegment(double x1, double y1, double x2, double y2)
            : start_(x1, y1), end_(x2, y2)  {
        if (start_.getX() > end_.getX()) {
            std::swap(start_, end_);
        }
        if (start_.getX() == end_.getX()) {
            if (start_.getY() > end_.getY()) {
                std::swap(start_, end_);
            }
        }
    }

    bool isIntersect(LineSegment& other, double& x) {
        return intersect(start_, end_, other.start_, other.end_, x);
    }

    bool isHorizontal() {
        return end_.getY() == start_.getY();
    }

    bool isVertical() {
        return end_.getX() == start_.getX();
    }

    friend std::istream& operator >> (
            std::istream& in,
            LineSegment& segment
    ) {
        in >> segment.start_ >> segment.end_;
        return in;
    }

private:

    static double max(double x, double y){
        return x > y ? x : y;
    }

    static double min(double x, double y){
        return x < y ? x : y;
    }

    static double det(double a, double b, double c, double d) {
        return a * d - b * c;
    }

    static bool between(double a, double b, double c) {
        return min(a, b) <= c + EPSILON && c <= max(a, b) + EPSILON;
    }

    static bool intersectParallels(double a, double b, double c, double d) {
        if (a > b)  {
            std::swap (a, b);
        }
        if (c > d) {
            std::swap(c, d);
        }
        return max(a, c) <= min(b, d);
    }

    static bool intersect(Point &a, Point& b, Point& c, Point& d, double& x) {
        double A1 = a.getY() - b.getY();
        double B1 = b.getX() - a.getX();
        double C1 = -A1 * a.getX() - B1 * a.getY();
        double A2 = c.getY() - d.getY();
        double B2 = d.getX() - c.getX();
        double C2 = -A2 * c.getX() - B2 * c.getY();
        double delta = det (A1, B1, A2, B2);
        if (delta >= EPSILON || delta <= EPSILON) {
            x = - det(C1, B1, C2, B2) * 1.0 / delta;
            double y = - det(A1, C1, A2, C2) * 1.0 / delta;
            return between(a.getX(), b.getX(), x) && between(a.getY(), b.getY(), y)
                   && between(c.getX(), d.getX(), x) && between (c.getY(), d.getY(), y);
        } else {
            return (det(A1, C1, A2, C2) == 0 && det(B1, C1, B2, C2) == 0)
                   && (intersectParallels(a.getX(), b.getX(), c.getX(), d.getX())
                   && intersectParallels(a.getY(), b.getY(), c.getY(), d.getY()));
        }
    }

    Point start_;
    Point end_;

};

void getInput(
    std::istream& in,
    LineSegment& road,
    std::vector<LineSegment>& rivers
) {
    in >> road;
    int32_t n;
    in >> n;
    rivers.resize(n);
    for (int32_t i = 0; i < n; ++i) {
        in >> rivers[i];
    }
}

void setOutput(
        std::ostream& out,
        int64_t count
) {
    out << count;
}


int64_t findCount(
        LineSegment& road,
        std::vector<LineSegment>& rivers
) {
    std::vector<double> intersections;
    for (auto& river : rivers) {
        double x;
        if (road.isIntersect(river, x)) {
            intersections.push_back(x);
        }
    }

    return intersections.size();

}

int main() {
    LineSegment road;
    std::vector<LineSegment> rivers;
    getInput(std::cin, road, rivers);
    setOutput(std::cout, findCount(road, rivers));
    return 0;
}
