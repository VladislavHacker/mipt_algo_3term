#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdint>
#include <unordered_set>
#include <cmath>
#include <iomanip>

const long double EPSILON = 0.000001;

template<typename T>
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

    Point(T x, T y) : x_(x), y_(y) { }

    /* >> overloaded operator */
    friend std::istream& operator >> (
        std::istream& in,
        Point& point
    ) {
        in >> point.x_ >> point.y_;
        return in;
    }

    T getX() const {
        return x_;
    }

    T getY() const {
        return y_;
    }

    Point& operator *= (T a) {
        x_ *= a;
        y_ *= a;
        return *this;
    }

    Point& operator * (T a) {
        Point res = *this;
        return (res *= a);
    }

    Point& operator - () {
        return *this * -1;
    }

    Point& operator += (const Point& other) {
        x_ += other.x_;
        y_ += other.y_;
        return *this;
    }

    Point& operator -= (const Point& other) {
        x_ -= other.x_;
        y_ -= other.y_;
        return *this;
    }

    Point operator + (const Point& other) {
        Point res = *this;
        return (res += other);
    }

    Point operator - (const Point& other) {
        Point res = *this;
        return (res -= other);
    }

    bool operator < (const Point& other) const {
        return x_ < other.x_ || (std::abs(x_ - other.x_) < EPSILON && y_ < other.y_);
    }

    bool operator > (const Point& other) const {
        return other < *this;
    }

    bool operator <= (const Point& other) const {
        return !(*this > other);
    }

    bool operator >= (const Point& other) const {
        return !(*this < other);
    }

    bool operator == (const Point& other) const {
        return !(*this < other) && !(*this > other);
    }

    bool operator != (const Point& other) const {
        return !(*this == other);
    }

    bool isLessPolarAngle(const Point& other) const {
        return x_ * other.y_ - y_ * other.x_ > 0;
    }

protected:

    T x_;
    T y_;

};

template<typename T>
class Figure {
public:

    /* default constructor */
    Figure() = default;

    /* destructor */
    ~Figure() = default;

    /* copy constructor */
    Figure(const Figure& other) {
        points_ = other.points_;
    }

    /* move constructor */
    Figure(Figure&& other) noexcept {
        points_ = other.points_;
        other.points_ = std::vector<Point<T>>();
    }

    /* copy assignment operator */
    Figure& operator = (const Figure& other) = default;

    /* move assignment operator */
    Figure& operator = (Figure&& other)  noexcept {
        if (&other != this) {
            points_ = other.points_;
            other.points_ = std::vector<Point<T>>();
        }
        return *this;
    }

    /* >> overloaded operator */
    friend std::istream& operator >> (
            std::istream& in,
            Figure& figure
    ) {
        size_t n;
        in >> n;
        figure.points_.resize(n);
        for (size_t i = 0; i < n; ++i) {
            in >> figure.points_[i];
        }
        figure.makeOrder();
        return in;
    }

    /* Minkovski sum */
    Figure& operator += (const Figure& other) {

        auto first = points_;
        auto second = other.points_;

        first.push_back(first[0]);
        first.push_back(first[1]);
        second.push_back(second[0]);
        second.push_back(second[1]);

        std::vector<Point<T>> result;

        size_t i = 0;
        size_t j = 0;

        while (i < first.size() - 1 && j < second.size() - 1) {
            result.push_back(first[i] + second[j]);
            Point t1 = first[i + 1] - first[i];
            Point t2 = second[j + 1] - second[j];
            if (t1.isLessPolarAngle(second[j + 1] - second[j])) {
                ++i;
            } else if (t2.isLessPolarAngle( first[i + 1] - first[i])) {
                ++j;
            } else {
                ++i;
                ++j;
            }
        }

        points_ = result;

        makeOrder();

        return *this;

    }

    /* Minkovski sum */
    Figure operator + (const Figure& other) const {
        Figure res = *this;
        return (res += other);
    }

    long double getArea() const {
        long double result = 0;
        for (size_t i = 0; i < points_.size(); ++i) {
            const Point<T>& p1 = points_[i];
            const Point<T>& p2 = points_[(i + 1) % points_.size()];
            result += det(p1.getX(), p1.getY(), p2.getX(), p2.getY());
        }
        return std::abs(result) / 2.0;
    }

protected:

    std::vector<Point<int64_t>> points_;

private:

    static T det(T x1, T y1, T x2, T y2) {
        return x1 * y2 - y1 * x2;
    }

    void makeOrder() {

        size_t min = 0;
        for (size_t i = 1; i < points_.size(); ++i) {
            if (points_[min] > points_[i]) {
                min = i;
            }
        }

        std::vector<Point<int64_t>> new_order;
        new_order.reserve(points_.size());
        for (size_t i = 0; i < points_.size(); ++i) {
            new_order.push_back(points_[(i + min) % points_.size()]);
        }

        points_ = new_order;

    }

};

void getInput(
    std::istream& in,
    Figure<int64_t>& firstFigure,
    Figure<int64_t>& secondFigure
) {
    in >> firstFigure;
    in >> secondFigure;
}

void setOutput(
    std::ostream& out,
    long double area
) {
    out << std::fixed << std::setprecision(6) << area;
}

long double getS(const Figure<int64_t>& firstFigure, const Figure<int64_t>& secondFigure) {
    long double s1 = firstFigure.getArea();
    long double s2 = secondFigure.getArea();
    Figure sum = firstFigure + secondFigure;
    long double s3 = sum.getArea();
    return (s3 - s1 - s2) / 2;
}


int main() {

    Figure<int64_t> firstFigure;
    Figure<int64_t> secondFigure;
    getInput(std::cin, firstFigure, secondFigure);

    setOutput(std::cout, getS(firstFigure, secondFigure));
    return 0;

}
