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
        double split_coordinate;

        RTNode() {}
};

void build_range_tree(vector<Point> points, int recursion_depth, RTNode* root)
{
    if(points.size() == 1 && recursion_depth == 1)
    {
        root -> point = points[0];
        root -> is_leaf = true;
        root -> split_coordinate = points[0].y;
        return;
    }

    int N = (int)points.size();
    if(N > 1)
    {
        vector<Point> left_points;
        vector<Point> right_points;
        for(int i = 0; i < N / 2; i++)
            left_points.pb(points[i]);
        for(int i = N / 2; i < N; i++)
            right_points.pb(points[i]);

        if(recursion_depth == 0)
            root -> split_coordinate = right_points.front().x;
        else if (recursion_depth == 1)
            root -> split_coordinate = right_points.front().y;
        
        root -> left_child = new RTNode();
        root -> right_child = new RTNode();
        build_range_tree(left_points, recursion_depth, root -> left_child);
        build_range_tree(right_points, recursion_depth, root -> right_child);

        vector<Point>().swap(left_points);
        vector<Point>().swap(right_points);
    }
    else
        root -> split_coordinate = points[0].x;

    if(recursion_depth < 1)
    {
        root -> associated_node = new RTNode();
        // NOTE: O(n lg^2 n)
        sort(points.begin(), points.end(), [](Point a, Point b) { return (a.y < b.y ? 1 : (a.y == b.y ? a.x < b.x : 0)); });
        build_range_tree(points, recursion_depth + 1, root -> associated_node);
    }

}

vector<Point> search_range_tree(Range search_range, int recursion_depth, RTNode* root)
{
    vector<Point> ans;
    if(root -> is_leaf)
    {
        if(search_range.lower_bound.y <= root -> point.y && root -> point.y <= search_range.upper_bound.y)
            ans.pb(root -> point);
        return ans;
    }

    if(root -> left_child != nullptr && search_range.lower_bound.x <= root -> left_child -> point)
    {
        vector<Point> left_ans = search_range_tree(search_range, root -> left_child);
        for(auto i : left_ans)
            ans.pb(i);
    }

    if(root -> right_child != nullptr && root -> right_child -> point <= search_range.upper_bound)
    {
        vector<Point> right_ans = search_range_tree(search_range, root -> right_child);
        for(auto i : right_ans)
            ans.pb(i);
    }

    return ans;
}