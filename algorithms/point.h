#include <cmath>
#include <iostream>
#include <vector>
#include <cassert>
#define assertm(exp, msg) assert(((void)msg, exp))
#pragma once

using std::vector;

const double INF = 1e9;
const int num_dimensions = 2;

struct Point
{
    vector<double> coordinates = vector<double>(num_dimensions, 0);

    Point() {}
    Point(vector<double> coordinates)
    {
        assertm(coordinates.size() == num_dimensions, "Invalid number of dimensions");
        this -> coordinates = coordinates;
    }

    bool operator==(const Point &p) const
    {
        for(int i = 0; i < num_dimensions; i++)
            if(coordinates[i] != p.coordinates[i])
                return false;
        return true;
    }

    friend std::ostream& operator<<(std::ostream& os, Point p)
    {
		os << "(";
        for(int i = 0; i < num_dimensions; i++)
        {
            os << p.coordinates[i];
            if(i != num_dimensions - 1)
                os << ", ";
        }
        os << ")";
        return os;
    }
};

class Range
{
    public:
        Point lower_bound;
        Point upper_bound;

        Range() : lower_bound(Point()), upper_bound(Point()) {}
        Range(Point lower_bound, Point upper_bound) : lower_bound(lower_bound), upper_bound(upper_bound) {}
};

bool totally_contained(const Range r1, const Range r2)
{
    for(int i = 0; i < num_dimensions; i++)
        if(r1.upper_bound.coordinates[i] < r2.lower_bound.coordinates[i]
            || r2.upper_bound.coordinates[i] < r1.lower_bound.coordinates[i])
            return false;
    return true;
}

bool totally_outside(const Range r1, const Range r2)
{
    for(int i = 0; i < num_dimensions; i++)
        if(r1.upper_bound.coordinates[i] <= r2.lower_bound.coordinates[i]
            || r1.lower_bound.coordinates[i] > r2.upper_bound.coordinates[i])
            return true;
    return false;
}