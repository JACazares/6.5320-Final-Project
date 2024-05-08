#include <cmath>
#include <iostream>
#pragma once

struct Point
{
    double x, y;
    Point() {}
    Point(double x, double y) : x(x), y(y) {}
    double distance(const Point &p) const
    {
        return sqrt((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y));
    }

    bool operator==(const Point &p) const
    {
        return x == p.x && y == p.y;
    }

    bool operator<(const Point &p) const
    {
        return x < p.x || (x == p.x && y < p.y);
    }

    bool operator>(const Point &p) const
    {
        return x > p.x || (x == p.x && y > p.y);
    }

    bool operator<=(const Point &p) const
    {
        return x < p.x || (x == p.x && y <= p.y);
    }

    bool operator>=(const Point &p) const
    {
        return x > p.x || (x == p.x && y >= p.y);
    }

    friend std::ostream& operator<<(std::ostream& os, Point p)
    {
		return os << "(" << p.x << "," << p.y << ")";
    }
};

class Range
{
    public:
        Point lower_bound;
        Point upper_bound;

        Range(Point lower_bound, Point upper_bound) : lower_bound(lower_bound), upper_bound(upper_bound) {}
};