#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>

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

    bool operator < (const Point& other) const {
        return getX() < other.getX() || getX() == other.getX() && getY() < other.getY();
    }

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

    double getX() const {
        return x_;
    }

    double getY() const {
        return y_;
    }

protected:

    double x_;
    double y_;

};

double calc(const Point& a, const Point& b, const Point& c) {
    return a.getX() * (b.getY() - c.getY()) + b.getX() * (c.getY() - a.getY()) + c.getX() * (a.getY() - b.getY());
}

bool lessZero(const Point& a, const Point& b, const Point& c) {
    return calc(a, b, c) < 0;
}

bool greaterZero(const Point& a, const Point& b, const Point& c) {
    return calc(a, b, c) > 0;
}

void convexHull(std::vector<Point> points, std::vector<Point>& result) {
    if (points.size() <= 1) {
        return;
    }
    std::sort(points.begin(), points.end());
    Point point1 = *points.begin();
    Point point2 = points.back();
    std::vector<Point> up;
    std::vector<Point> down;
    up.push_back(point1);
    down.push_back(point1);
    for (size_t i = 1; i < points.size(); ++i) {
        if (points.size() - 1 == i || lessZero(point1, points[i], point2)) {
            while (
                up.size() >= 2 &&
                !lessZero(up[up.size() - 2], up[up.size() - 1], points[i])
            ) {
                up.pop_back();
            }
            up.push_back(points[i]);
        }
        if (points.size() - 1 == i || greaterZero(point1, points[i], point2)) {
            while (
                down.size() >= 2 &&
                !greaterZero(down[down.size() - 2], down[down.size() - 1], points[i])
            ) {
                down.pop_back();
            }
            down.push_back(points[i]);
        }
    }
    for (auto & i : up) {
        result.push_back(i);
    }
    for (size_t i=down.size() - 2; i > 0; --i) {
        result.push_back(down[i]);
    }
}

void getInput(
    std::istream& in,
    std::vector<Point>& points
) {
    int32_t n;
    in >> n;
    points.resize(n);
    for (int32_t i = 0; i < n; ++i) {
        in >> points[i];
    }
}

double calcLength(std::vector<Point>& result) {
    double size = 0;
    for (int32_t i = 0; i < result.size(); ++i) {
        double a = result[i].getX() - result[(i + 1) % result.size()].getX();
        double b = result[i].getY() - result[(i + 1) % result.size()].getY();
        size += pow(a * a + b * b, 0.5);
    }
    return size;
}

void setOutput(
    std::ostream& out,
    double size
) {
    std::cout << std::setprecision(9) << size;
}

int main() {

    std::vector<Point> points;
    std::vector<Point> result;
    getInput(std::cin, points);
    convexHull(points, result);
    setOutput(std::cout, calcLength(result));

    return 0;
}
