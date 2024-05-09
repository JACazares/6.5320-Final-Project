#include "point.h"
#include <vector>
#include <algorithm>
#include <cmath>
#include <unordered_map>
#include <iostream>

#define pb push_back

using std::vector;
using std::pair;
using std::unordered_map;
using std::cout;

class KDNode
{
    public:
        Point lower_bound;
        Point upper_bound;
        KDNode* left_child = nullptr;
        KDNode* right_child = nullptr;

        Point point;
        double split_coordinate;

        KDNode()
        {
            lower_bound = Point();
            for(int i = 0; i < num_dimensions; i++)
                lower_bound.coordinates[i] = -INF;

            upper_bound = Point();
            for(int i = 0; i < num_dimensions; i++)
                upper_bound.coordinates[i] = INF;
        }

        KDNode(KDNode* parent)
        {
            lower_bound = parent -> lower_bound;
            upper_bound = parent -> upper_bound;
        }

};

void build_kd_tree(vector<vector<pair<Point, int>>> points_sorted, int depth, KDNode* root)
{
    int N = (int)points_sorted[0].size();
    // cout << N << "\n";
    // for(auto i : points_sorted_x)
    //     cout << "(" << i.first << ", " << i.second << ")" << " ";
    // cout << "\n";
    if(N == 1)
    {
        root -> point = points_sorted[0][0].first;
        root -> left_child = nullptr;
        root -> right_child = nullptr;
        return;
    }

    vector<vector<pair<Point, int>>> left_child_points_sorted(num_dimensions, vector<pair<Point, int>>());
    vector<vector<pair<Point, int>>> right_child_points_sorted(num_dimensions, vector<pair<Point, int>>());

    unordered_map<int, bool> index_in_left;

    root -> left_child = new KDNode(root);
    root -> right_child = new KDNode(root);

    int current_coordinate_index = depth % num_dimensions;

    root -> split_coordinate = points_sorted[current_coordinate_index][N/2].first.coordinates[current_coordinate_index];
    
    for(int i = 0; i < N/2; i++)
    {
        index_in_left[points_sorted[current_coordinate_index][i].second] = 1;
        left_child_points_sorted[current_coordinate_index].pb(points_sorted[current_coordinate_index][i]);
    }
    for(int i = N/2; i < N; i++)
        right_child_points_sorted[current_coordinate_index].pb(points_sorted[current_coordinate_index][i]);
    
    for(int d = 0; d < num_dimensions; d++)
    {
        if(d == current_coordinate_index)
            continue;
        for(int j = 0; j < N; j++)
        {
            if(index_in_left[points_sorted[d][j].second])
                left_child_points_sorted[d].pb(points_sorted[d][j]);
            else
                right_child_points_sorted[d].pb(points_sorted[d][j]);
        }

        root -> left_child -> upper_bound.coordinates[d] = root -> split_coordinate;
        root -> right_child -> lower_bound.coordinates[d] = root -> split_coordinate;

        for(int i = 0; i < N/2; i++)
            index_in_left[points_sorted[d][i].second] = 0;
    }
    
    build_kd_tree(left_child_points_sorted, depth + 1, root -> left_child);
    build_kd_tree(right_child_points_sorted, depth + 1, root -> right_child);

    vector<vector<pair<Point, int>>>().swap(left_child_points_sorted);
    vector<vector<pair<Point, int>>>().swap(right_child_points_sorted);
    unordered_map<int, bool>().swap(index_in_left);
}

int kd_iterations = 0;

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

vector<Point> search_kd_tree(const Range search_range, KDNode* root)
{
    kd_iterations++;
    if(root == nullptr)
        return vector<Point>();
    
    if(root -> left_child == nullptr && root -> right_child == nullptr)
    {
        if(totally_contained(Range(root -> point, root -> point), search_range))
            return vector<Point>({Point(root -> point)});
        else
            return vector<Point>();
    }

    if(totally_outside(Range(root -> lower_bound, root -> upper_bound), search_range))
        return vector<Point>();

    vector<Point> left_child_points = search_kd_tree(search_range, root -> left_child);
    vector<Point> right_child_points = search_kd_tree(search_range, root -> right_child);
    left_child_points.insert(left_child_points.end(), right_child_points.begin(), right_child_points.end());
    return left_child_points;
}