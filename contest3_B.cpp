#include <algorithm>
#include <cmath>
#include <functional>
#include <vector>
#include <iostream>
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
    Point(Point&& other) noexcept {
        x_ = other.x_;
        y_ = other.y_;
        other.x_ = 0;
        other.x_ = 0;
    }

    /* copy assignment operator */
    Point& operator=(const Point& other) = default;

    bool operator<(const Point& other) const {
        return getX() < other.getX() || getX() == other.getX() && getY() < other.getY();
    }

    /* move assignment operator */
    Point& operator=(Point&& other)  noexcept {
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
    friend std::istream& operator>>(
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

void buildStep(
    const std::function<bool(const Point&, const Point&, const Point&)> &checker,
    const std::vector<Point> &points,
    size_t i,
    const Point& point1,
    const Point& point2,
    std::vector<Point>& result
) {
    if (points.size() - 1 == i || checker(point1, points[i], point2)) {
        while (
            result.size() >= 2 &&
            !checker(result[result.size() - 2], result[result.size() - 1], points[i])
        ) {
            result.pop_back();
        }
        result.push_back(points[i]);
    }
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
        buildStep(lessZero, points, i,point1, point2,up);
        buildStep(greaterZero, points, i, point1, point2, down);
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

double calculateDistance(const Point& first, const Point& second) {
    double a = first.getX() - second.getX();
    double b = first.getY() - second.getY();
    return pow(a * a + b * b, 0.5);
}

double calcLength(std::vector<Point>& result) {
    double size = 0;
    for (int32_t i = 0; i < result.size(); ++i) {
        size += calculateDistance(result[i], result[(i + 1) % result.size()]);
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
