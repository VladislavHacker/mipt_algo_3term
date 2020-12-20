#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>
#include <functional>

long double const INF = std::numeric_limits<long double>::max();
long double const ANGLE = 0.01;

class Point {
public:

    Point* prev;
    Point* next;

    long long id;
    long double x, y, z;

    explicit Point(long double x, long double y, long double z, long long id) :
        x(x), y(y), z(z), id(id), next(nullptr), prev(nullptr) {
    }

    Point() : Point(0, 0, 0, -1) {}

    bool operator<(const Point &other) const {
        return x < other.x;
    }

    bool operator>(const Point& other) const {
        return other < *this;
    }

    bool operator<=(const Point& other) const {
        return !(*this > other);
    }

    bool operator>=(const Point& other) const {
        return !(*this < other);
    }

    bool operator==(const Point& other) const {
        return !(*this < other) && !(*this > other);
    }

    bool operator!=(const Point& other) const {
        return !(*this == other);
    }

    friend std::istream& operator>>(std::istream& in, Point &p) {
        std::cin >> p.x >> p.y >> p.z;
        p.rotate(ANGLE);  // important !
        return in;
    }

    bool performEvent() {
        if (prev->next != this) {
            prev->next = this;
            next->prev = this;
            return true;
        }
        prev->next = next;
        next->prev = prev;
        return false;
    }

private:

    void rotate(long double angle) {

        long double new_z = -x * std::sin(angle) + z * std::cos(angle);
        long double new_x = x * std::cos(angle) + z * std::sin(angle);

        long double new_y;

        z = new_z;
        x = new_x;

        new_z = z * std::cos(angle) + y * std::sin(angle);
        new_y = -z * std::sin(angle) + y * std::cos(angle);
        y = new_y;
        z = new_z;

        new_x = x * std::cos(angle) + y * std::sin(angle);
        new_y = -x * std::sin(angle) + y * std::cos(angle);
        x = new_x;
        y = new_y;

    }

};


class Face {
public:

    std::vector<long long> points;

    Face(long long first, long long second, long long third) {
        points.push_back(first);
        points.push_back(second);
        points.push_back(third);
    }

    bool operator<(const Face& other) const {
        for (size_t i = 0; i < points.size() - 1; ++i) {
            if (points[i] != other.points[i]) {
                return points[i] < other.points[i];
            }
        }
        return (points[points.size() - 1] < other.points[points.size() - 1]);
    }

    bool operator>(const Face& other) const {
        return other < *this;
    }

    bool operator<=(const Face& other) const {
        return !(*this > other);
    }

    bool operator>=(const Face& other) const {
        return !(*this < other);
    }

    bool operator==(const Face& other) const {
        return !(*this < other) && !(*this > other);
    }

    bool operator!=(const Face& other) const {
        return !(*this == other);
    }

    void fixFaceOrder() {
        if (points[1] < points[0] && points[1] < points[2]) {
            std::swap(points[0], points[1]);
            std::swap(points[1], points[2]);
            return;
        }
        if (points[2] < points[0] && points[2] < points[1]) {
            std::swap(points[1], points[2]);
            std::swap(points[0], points[1]);
        }
    }

};

long double sign(const Point* a, const Point* b, const Point* c) {
    return ((a == nullptr || b == nullptr || c == nullptr) ?
                INF :
                (b->x - a->x) * (c->y - b->y) - (b->y - a->y) * (c->x - b->x)
    );
}


long double calcTime(const Point* a, const Point* b, const Point* c) {
    return ((a == nullptr || b == nullptr || c == nullptr) ?
                INF :
                (((b->x - a->x) * (c->z - b->z) - (b->z - a->z) * (c->x - b->x)) / sign(a, b, c))
    );
}

std::pair<long long, long double> findMin(std::vector<long double>& next_time, long double current_time) {
    long long min_index = -1;
    long double min_time = INF;

    for (int i = 0; i < 6; i++) {
        if ((next_time[i] > current_time) && (next_time[i] < min_time)) {
            min_time = next_time[i];
            min_index = i;
        }
    }
    return std::make_pair(min_index, min_time);
}

bool handleCases(
    std::pair<long long, long double>& min_p,
    Point*& left_p,
    Point*& right_p,
    Point*& u,
    Point*& v,
    size_t& first_id,
    size_t& second_id,
    std::vector<Point*>& result
) {

    long long min_index = min_p.first;
    long double min_time = min_p.second;

    if (min_index == -1 || min_time >= INF) {
        return false;
    }

    switch (min_index) {
        case 0:
            if (left_p->x < u->x) {
                result.push_back(left_p);
            }
            left_p->performEvent();
            ++first_id;
            break;
        case 1:
            if (right_p->x > v->x) {
                result.push_back(right_p);
            }
            right_p->performEvent();
            ++second_id;
            break;
        case 2:
            result.push_back(v);
            v = v->next;
            break;
        case 3:
            v = v->prev;
            result.push_back(v);
            break;
        case 4:
            result.push_back(u);
            u = u->prev;
            break;
        case 5:
            u = u->next;
            result.push_back(u);
            break;
        default:
            break;
    }

    return true;
}

std::vector<Point*> BuildHull(std::vector<Point> &points, size_t left, size_t right) {

    if (right == 1 + left) {
        return std::vector<Point*> ();
    }

    size_t mid = left + (right - left) / 2;

    size_t first_id = 0;
    size_t second_id = 0;

    std::vector<std::vector<Point*>> events;
    events.push_back(BuildHull(points, left, mid));
    events.push_back(BuildHull(points, mid, right));

    std::vector<Point*> result;

    Point *v = &points[mid];
    Point *u = &points[mid - 1];

    bool end_iter = false;
    while (!end_iter) {
        if (sign(u, v, v->next) < 0) {
            v = v->next;
        } else if (sign(u->prev, u, v) < 0) {
            u = u->prev;
        } else {
            end_iter = true;
        }
    }

    long double current_t = -INF;

    while (true) {

        Point *left_p = nullptr;
        Point *right_p = nullptr;

        std::vector<long double> next_time(6, INF);

        if (first_id < events[0].size()) {
            left_p = events[0][first_id];
            next_time[0] = calcTime(left_p->prev, left_p, left_p->next);
        }

        if (second_id < events[1].size()) {
            right_p = events[1][second_id];
            next_time[1] = calcTime(right_p->prev, right_p, right_p->next);
        }

        next_time[2] = calcTime(u, v, v->next);
        next_time[3] = calcTime(u, v->prev, v);
        next_time[4] = calcTime(u->prev, u, v);
        next_time[5] = calcTime(u, u->next, v);

        auto min_p = findMin(next_time, current_t);

        if (!handleCases(min_p, left_p, right_p, u, v, first_id, second_id, result)) {
            break;
        }

        current_t = min_p.second;

    }

    u->next = v;
    v->prev = u;

    for (auto it = result.rbegin(); it != result.rend(); ++it) {
        Point* current = *it;
        if (current->x < v->x && current->x > u->x) {
            v->prev = current;
            u->next = v->prev;
            current->prev = u;
            current->next = v;
            if (current->x <= points[mid - 1].x) {
                u = current;
            } else {
                v = current;
            }
        } else {
            current->performEvent();
            if (current == u) {
                u = u->prev;
            }
            if (current == v) {
                v = v->next;
            }
        }
    }

    return result;

}


std::vector<Face> build3DConvexHull(std::vector<Point> points) {

    size_t n = points.size();
    std::vector<Face> result;

    std::sort(points.begin(), points.end());

    std::vector<Point*> events = BuildHull(points, 0, n);

    for (auto& event : events) {
        Face current(event->prev->id, event->id, event->next->id);
        if (!event->performEvent()) {
            std::swap(current.points[0], current.points[1]);
        }
        result.emplace_back(current);
    }

    for (auto& p : points) {
        p.z = -p.z;
        p.next = nullptr;
        p.prev = nullptr;
    }

    events = BuildHull(points, 0, n);

    for (auto& event : events) {

        Face current(event->prev->id, event->id, event->next->id);

        if (event->performEvent()) {
            std::swap(current.points[1], current.points[0]);
        }

        result.push_back(current);

    }

    for (auto& current : result) {
        current.fixFaceOrder();
    }

    std::sort(result.begin(), result.end());

    return result;

}

void fastReadWorkFromStream(
    std::istream& in,
    size_t n,
    const std::function<void(std::vector<Point>&)> &workerCallback
) {
    for (size_t i = 0; i < n; ++i) {
        size_t m;
        std::vector<Point> points;
        in >> m;
        points.resize(m);
        for (size_t j = 0; j < m; ++j) {
            points[j].id = j;
            in >> points[j];
        }
        workerCallback(points);
    }
}

int main() {

    /* cin/cout optimization */
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    size_t n;
    std::cin >> n;
    /* for good performance */
    fastReadWorkFromStream(std::cin, n, [](std::vector<Point>& points) {
        std::vector<Face> hull = build3DConvexHull(points);
        std::cout << hull.size() << std::endl;
        for (Face& f : hull) {
            std::cout << 3 << " " << f.points[0] << " " << f.points[1] << " " << f.points[2] << std::endl;
        }
    });

    return 0;
}
