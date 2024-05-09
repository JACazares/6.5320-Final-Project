#include "point.h"
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>
#include <iostream>

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
        double lower_bound = -INF;
        double upper_bound = INF;

        Point point;
        double split_coordinate;

        RTNode() {}
        RTNode(RTNode* parent)
        {
            this -> lower_bound = parent -> lower_bound;
            this -> upper_bound = parent -> upper_bound;
        }
        RTNode(double lower_bound, double upper_bound)
        {
            this -> lower_bound = lower_bound;
            this -> upper_bound = upper_bound;
        }
};

void build_range_tree(vector<Point> points, int recursion_depth, RTNode* root)
{
    int N = (int)points.size();
    if(N == 1)
    {
        // cout << recursion_depth << " " << points[0] << "\n";
        if(recursion_depth == 1)
        {
            root -> point = points[0];
            root -> is_leaf = true;
            root -> split_coordinate = points[0].y;
            return;
        }
        else
        {
            root -> split_coordinate = points[0].x;
            vector<Point> points_sorted_y = points;
            std::sort(points_sorted_y.begin(), points_sorted_y.end(), [](Point a, Point b) { return (a.y < b.y ? 1 : (a.y == b.y ? a.x < b.x : 0)); });
            root -> associated_node = new RTNode(points.front().y, points.back().y);
            build_range_tree(points_sorted_y, recursion_depth + 1, root -> associated_node);
            return;
        }
    }

    vector<Point> left_points;
    vector<Point> right_points;
    for(int i = 0; i < N / 2; i++)
        left_points.pb(points[i]);
    for(int i = N / 2; i < N; i++)
        right_points.pb(points[i]);

    root -> left_child = new RTNode(root);
    root -> right_child = new RTNode(root);

    if(recursion_depth == 0)
    {
        root -> split_coordinate = right_points.front().x;
        root -> left_child -> upper_bound = left_points.back().x;
        root -> right_child -> lower_bound = right_points.front().x;
    }
    else if (recursion_depth == 1)
    {
        root -> split_coordinate = right_points.front().y;
        root -> left_child -> upper_bound = left_points.back().y;
        root -> right_child -> lower_bound = right_points.front().y;
    }
    
    // cout << recursion_depth << ": " << root -> lower_bound << " " << root -> upper_bound << "\n";
    // for(auto i : points)
    //     cout << i << " ";
    // cout << "\n";
    // cout << root -> left_child << " " << root -> right_child << "\n";

    build_range_tree(left_points, recursion_depth, root -> left_child);
    build_range_tree(right_points, recursion_depth, root -> right_child);

    vector<Point>().swap(left_points);
    vector<Point>().swap(right_points);

    if(recursion_depth < 1)
    {
        // NOTE: O(n lg^2 n)
        vector<Point> points_sorted_y = points;
        std::sort(points_sorted_y.begin(), points_sorted_y.end(), [](Point a, Point b) { return (a.y < b.y ? 1 : (a.y == b.y ? a.x < b.x : 0)); });
        root -> associated_node = new RTNode(points.front().y, points.back().y);
        build_range_tree(points_sorted_y, recursion_depth + 1, root -> associated_node);
    }

}

int rt_iterations = 0;

vector<Point> search_range_tree(Range search_range, int recursion_depth, RTNode* root)
{
    if(root == nullptr)
        return vector<Point>();

    // cout << recursion_depth << " " << root -> lower_bound << " " << root -> upper_bound << "\n";
    rt_iterations++;
    if(recursion_depth == 0)
    {
        if(root -> upper_bound < search_range.lower_bound.x
            || search_range.upper_bound.x < root -> lower_bound)
            return vector<Point>();

        if(search_range.lower_bound.x <= root -> lower_bound
            && root -> upper_bound <= search_range.upper_bound.x)
        {
            // cout << "inside\n";
            return search_range_tree(search_range, recursion_depth + 1, root -> associated_node);
        }
        // cout << root -> left_child << " " << root -> right_child << "\n";

        vector<Point> left_child_points = search_range_tree(search_range, recursion_depth, root -> left_child);
        vector<Point> right_child_points = search_range_tree(search_range, recursion_depth, root -> right_child);

        left_child_points.insert(left_child_points.end(), right_child_points.begin(), right_child_points.end());
        return left_child_points;
    }
    else if(recursion_depth == 1)
    {
        if(root -> is_leaf)
        {
            if(search_range.lower_bound.y <= root -> point.y
                && root -> point.y <= search_range.upper_bound.y)
                return {root -> point};
            else
                return vector<Point>();
        }

        if(root -> upper_bound < search_range.lower_bound.y
            || search_range.upper_bound.y < root -> lower_bound)
            return vector<Point>();

        vector<Point> left_child_points = search_range_tree(search_range, recursion_depth, root -> left_child);
        vector<Point> right_child_points = search_range_tree(search_range, recursion_depth, root -> right_child);

        left_child_points.insert(left_child_points.end(), right_child_points.begin(), right_child_points.end());
        return left_child_points;
    }

    return vector<Point>();
}