#include <vector>
#include <algorithm>
#include <cmath>
#include <map>
#include <iostream>
#include "point.h"

#define pb push_back

using std::vector;
using std::pair;
using std::unordered_map;
using std::cout;

class RTNode
{
    public:
        RTNode* associated_node = nullptr;
        RTNode* left_child = nullptr;
        RTNode* right_child = nullptr;
        bool is_leaf = false;
        Point point;

        RTNode() {}
};

void build_range_tree(vector<Point> points, int recursion_depth, RTNode* root)
{
    if(points.size() == 1 && recursion_depth == 1)
    {
        root -> point = points[0];
        root -> is_leaf = true;
        return;
    }

    if(recursion_depth < 1)
    {
        root -> associated_node = new RTNode();
        build_range_tree(points, recursion_depth + 1, root -> associated_node);
    }

    if(points.size() == 1)
        return;

    int N = (int)points.size();
    vector<Point> left_points;
    vector<Point> right_points;
    for(int i = 0; i < N / 2; i++)
        left_points.pb(points[i]);
    for(int i = N / 2; i < N; i++)
        right_points.pb(points[i]);

    root -> left_child = new RTNode();
    root -> right_child = new RTNode();
    build_range_tree(left_points, recursion_depth, root -> left_child);
    build_range_tree(right_points, recursion_depth, root -> right_child);

    vector<Point>().swap(left_points);
    vector<Point>().swap(right_points);
}