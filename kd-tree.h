#include <vector>
#include <algorithm>
#include <cmath>
#include <unordered_map>
#include <iostream>
#include "point.h"

#define pb push_back

using std::vector;
using std::pair;
using std::unordered_map;
using std::cout;

const double INF = 1e9;

class KDNode
{
    public:
        Point lower_bound = Point(-INF, -INF);
        Point upper_bound = Point(INF, INF);
        KDNode* left_child = nullptr;
        KDNode* right_child = nullptr;

        Point point;
        double split_coordinate;

        KDNode()
        {
            lower_bound = Point(-INF, -INF);
            upper_bound = Point(INF, INF);
        }

        KDNode(KDNode* parent)
        {
            lower_bound = parent -> lower_bound;
            upper_bound = parent -> upper_bound;
        }

};

class Range
{
    public:
        Point lower_bound;
        Point upper_bound;

        Range(Point lower_bound, Point upper_bound) : lower_bound(lower_bound), upper_bound(upper_bound) {}
};

void build_kd_tree(vector<pair<Point, int>> points_sorted_x, vector<pair<Point, int>> points_storted_y, int depth, KDNode* root)
{
    int N = (int)points_sorted_x.size();
    // cout << N << "\n";
    // for(auto i : points_sorted_x)
    //     cout << "(" << i.first << ", " << i.second << ")" << " ";
    // cout << "\n";
    if(N == 1)
    {
        root -> point = points_sorted_x[0].first;
        root -> left_child = nullptr;
        root -> right_child = nullptr;
        return;
    }

    vector<pair<Point, int>> left_child_points_sorted_x;
    vector<pair<Point, int>> right_child_points_sorted_x;
    vector<pair<Point, int>> left_child_points_sorted_y;
    vector<pair<Point, int>> right_child_points_sorted_y;
    unordered_map<int, bool> index_in_left;

    root -> left_child = new KDNode(root);
    root -> right_child = new KDNode(root);

    if(depth % 2 == 0)
    {
        root -> split_coordinate = points_sorted_x[N/2].first.x;

        for(int i = 0; i < N/2; i++)
        {
            index_in_left[points_sorted_x[i].second] = 1;
            left_child_points_sorted_x.pb(points_sorted_x[i]);
        }
        for(int i = N/2; i < N; i++)
            right_child_points_sorted_x.pb(points_sorted_x[i]);
        
        for(int j = 0; j < N; j++)
        {
            if(index_in_left[points_storted_y[j].second])
                left_child_points_sorted_y.pb(points_storted_y[j]);
            else
                right_child_points_sorted_y.pb(points_storted_y[j]);
        }

        root -> left_child -> upper_bound.x = root -> split_coordinate;
        root -> right_child -> lower_bound.x = root -> split_coordinate;

        for(int i = 0; i < N/2; i++)
            index_in_left[points_sorted_x[i].second] = 0;
    }
    else
    {
        root -> split_coordinate = points_storted_y[N/2].first.y;

        for(int j = 0; j < N/2; j++)
        {
            index_in_left[points_storted_y[j].second] = 1;
            left_child_points_sorted_y.pb(points_storted_y[j]);
        }
        for(int j = N/2; j < N; j++)
            right_child_points_sorted_y.pb(points_storted_y[j]);
        
        for(int i = 0; i < N; i++)
        {
            if(index_in_left[points_sorted_x[i].second])
                left_child_points_sorted_x.pb(points_sorted_x[i]);
            else
                right_child_points_sorted_x.pb(points_sorted_x[i]);
        }

        root -> left_child -> upper_bound.y = root -> split_coordinate;
        root -> right_child -> lower_bound.y = root -> split_coordinate;
        
        for(int j = 0; j < N/2; j++)
            index_in_left[points_storted_y[j].second] = 0;
    }

    #pragma omp parallel
    {
        build_kd_tree(left_child_points_sorted_x, left_child_points_sorted_y, depth + 1, root -> left_child);
        build_kd_tree(right_child_points_sorted_x, right_child_points_sorted_y, depth + 1, root -> right_child);
    }

    vector<pair<Point, int>>().swap(left_child_points_sorted_x);
    vector<pair<Point, int>>().swap(right_child_points_sorted_x);
    vector<pair<Point, int>>().swap(left_child_points_sorted_y);
    vector<pair<Point, int>>().swap(right_child_points_sorted_y); 
    unordered_map<int, bool>().swap(index_in_left);
}

int kd_iterations = 0;

vector<Point> search_kd_tree(const Range search_range, KDNode* root)
{
    kd_iterations++;
    if(root == nullptr)
        return vector<Point>();
    
    if(root -> left_child == nullptr && root -> right_child == nullptr)
    {
        
        if(search_range.lower_bound.x <= root -> point.x
            && root -> point.x <= search_range.upper_bound.x
            && search_range.lower_bound.y <= root -> point.y
            && root -> point.y <= search_range.upper_bound.y)
            return vector<Point>({Point(root -> point)});
        else
            return vector<Point>();
    }

    if((root -> upper_bound).x <= search_range.lower_bound.x || (root->upper_bound).y <= search_range.lower_bound.y
        || (root -> lower_bound).x > search_range.upper_bound.x || (root -> lower_bound).y > search_range.upper_bound.y)
        return vector<Point>();

    vector<Point> left_child_points = search_kd_tree(search_range, root -> left_child);
    vector<Point> right_child_points = search_kd_tree(search_range, root -> right_child);
    left_child_points.insert(left_child_points.end(), right_child_points.begin(), right_child_points.end());
    return left_child_points;
}