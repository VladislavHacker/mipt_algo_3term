#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdint>
#include <unordered_set>
#include <iomanip>
#include <set>

enum {
    start = 1,
    end = -1
};

const long double EPSILON = 1E-9;

template<typename T = long double>
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


template<typename T = long double>
class LineSegment {
public:

    LineSegment() : LineSegment(0, 0, 0, 0) {}

    LineSegment(T x1, T y1, T x2, T y2)
            : start_(x1, y1), end_(x2, y2), id_(0)  {
        normalize();
    }

    void normalize() {
        if (start_.getX() > end_.getX()) {
            std::swap(start_, end_);
        }
        if (start_.getX() == end_.getX()) {
            if (start_.getY() > end_.getY()) {
                std::swap(start_, end_);
            }
        }
    }

    bool isIntersect(const LineSegment& other) const {
        return intersect(start_, end_, other.start_, other.end_);
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
        segment.normalize();
        return in;
    }

    bool operator < (const LineSegment<T>& other) const {
        T x = std::max(
            std::min(start_.getX(), end_.getX()), std::min(other.start_.getX(), other.end_.getX())
        );
        long double diff = getY(x) - other.getY(x);
        if (std::abs(diff) < EPSILON) {
            return getId() < other.getId();
        }
        return diff < 0;
    }

    const Point<T>& getA() const {
        return start_;
    }

    const Point<T>& getB() const {
        return end_;
    }


    long double getY(long double x) const {
        if (start_.getX() == end_.getX()) {
            return start_.getY();
        }
        return start_.getY() + (end_.getY() - start_.getY()) * (x - start_.getX()) / ((long double)end_.getX() - start_.getX());
    }

    void setId(size_t id) {
        id_ = id;
    }

    size_t getId() const {
        return id_;
    }

private:

    static T max(T x, T y){
        return x > y ? x : y;
    }

    static T min(T x, T y){
        return x < y ? x : y;
    }

    static T det(T a, T b, T c, T d) {
        return a * d - b * c;
    }

    static bool between(T a, T b, T c) {
        return min(a, b) <= c + EPSILON && c <= max(a, b) + EPSILON;
    }

    static bool intersectParallels(T a, T b, T c, T d) {
        if (a > b)  {
            std::swap(a, b);
        }
        if (c > d) {
            std::swap(c, d);
        }
        return max(a, c) <= min(b, d);
    }

    static bool intersect(const Point<T> &a, const Point<T>& b, const Point<T>& c, const Point<T>& d) {
        T A1 = a.getY() - b.getY();
        T B1 = b.getX() - a.getX();
        T C1 = -A1 * a.getX() - B1 * a.getY();
        T A2 = c.getY() - d.getY();
        T B2 = d.getX() - c.getX();
        T C2 = -A2 * c.getX() - B2 * c.getY();
        T delta = det (A1, B1, A2, B2);
        if (delta >= EPSILON || delta <= -EPSILON) {
            long double x = - det(C1, B1, C2, B2) * 1.0 / delta;
            long double y = - det(A1, C1, A2, C2) * 1.0 / delta;
            return between(a.getX(), b.getX(), x) && between(a.getY(), b.getY(), y)
                   && between(c.getX(), d.getX(), x) && between (c.getY(), d.getY(), y);
        } else {
            return (det(A1, C1, A2, C2) == 0 && det(B1, C1, B2, C2) == 0)
                   && (intersectParallels(a.getX(), b.getX(), c.getX(), d.getX())
                       && intersectParallels(a.getY(), b.getY(), c.getY(), d.getY()));
        }
    }

    Point<T> start_;
    Point<T> end_;

    size_t id_;

};

class Event {
public:

    Event() : x_(0), id_(0), type_(0) {}
    Event (long double x, size_t id, char type) : x_(x), id_(id), type_(type) { }

    bool operator < (const Event & other) const {
        if (std::abs(x_ - other.x_) > EPSILON) {
            return x_ < other.x_;
        } else if (type_ == start && other.type_ == end) {
            return true;
        }
        return id_ < other.id_;
    }

    long double getX() {
        return id_;
    }

    size_t getId() {
        return id_;
    }

    char getType() {
        return type_;
    }

private:

    long double x_;
    size_t id_;
    char type_;

};

std::set<LineSegment<long double>>::iterator getPrev(
    std::set<LineSegment<long double>> &s,
    std::set<LineSegment<long double>>::iterator it
) {
    return it == s.begin() ? s.end() : --it;
}

std::set<LineSegment<long double>>::iterator getNext(
    std::set<LineSegment<long double>> &s,
    std::set<LineSegment<long double>>::iterator it
) {
    return ++it;
}

bool findIntersection(
    const std::vector<LineSegment<long double>>& segments,
    std::pair<size_t, size_t>& intersection
) {

    std::set<LineSegment<long double>> s;
    std::vector<std::set<LineSegment<long double>>::iterator> where;

    std::vector<Event> events;
    size_t i = 0;
    events.reserve(2 * segments.size());
    for (auto& seg : segments) {
        events.emplace_back(std::min(seg.getA().getX(), seg.getB().getX()), i, start);
        events.emplace_back(std::max(seg.getA().getX(), seg.getB().getX()), i, end);
        ++i;
    }

    std::sort(events.begin(), events.end());

    where.resize(segments.size());

    for (auto& evt : events) {

        if (evt.getType() == start) {
            auto next = s.lower_bound(segments[evt.getId()]);
            auto prev = getPrev(s, next);
            if (next != s.end() && next->isIntersect(segments[evt.getId()])) {
                intersection = std::make_pair(next->getId(), evt.getId());
                return true;
            }
            if (prev != s.end() && prev->isIntersect(segments[evt.getId()])) {
                intersection = std::make_pair(prev->getId(), evt.getId());
                return true;
            }
            where[evt.getId()] = s.insert(next, segments[evt.getId()]);
        } else {
            auto next = getNext(s, where[evt.getId()]);
            auto prev = getPrev(s, where[evt.getId()]);
            if (prev != s.end() && next != s.end() && next->isIntersect(*prev)) {
                intersection = std::make_pair(prev->getId(), next->getId());
                return true;
            }
            s.erase(where[evt.getId()]);
        }

    }

    return false;
}

void getInput(
    std::istream& in,
    std::vector<LineSegment<long double>> &segments
) {
    size_t n;
    in >> n;
    segments.resize(n);
    for (size_t i = 0; i < n; ++i) {
        in >> segments[i];
        segments[i].setId(i);
    }
}

int main() {

    std::vector<LineSegment<long double>> segments;
    getInput(std::cin, segments);
    std::pair<size_t, size_t > result_ids(0, 0);

    if (findIntersection(segments, result_ids)) {
        std::cout << "YES" << std::endl;
        if (result_ids.first < result_ids.second) {
            std::cout << result_ids.first + 1 << " " << result_ids.second + 1;
        } else {
            std::cout << result_ids.second + 1 << " " << result_ids.first + 1;
        }
    } else {
        std::cout << "NO" << std::endl;
    }

    return 0;
}
