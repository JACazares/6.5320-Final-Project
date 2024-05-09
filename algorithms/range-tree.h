#include "point.h"
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>
#include <iostream>

#define pb push_back

namespace range_tree
{
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
            if(recursion_depth == num_dimensions - 1)
            {
                root -> point = points[0];
                root -> is_leaf = true;
                root -> split_coordinate = points[0].coordinates[recursion_depth];
                return;
            }
            else
            {
                root -> split_coordinate = points[0].coordinates[recursion_depth];
                vector<Point> points_sorted = points;
                std::sort(points_sorted.begin(), points_sorted.end(), [recursion_depth](Point a, Point b) -> bool {
                    return (a.coordinates[recursion_depth + 1] < b.coordinates[recursion_depth + 1]);
                });
                root -> associated_node = new RTNode(points_sorted.front().coordinates[recursion_depth + 1], points_sorted.back().coordinates[recursion_depth + 1]);
                build_range_tree(points_sorted, recursion_depth + 1, root -> associated_node);
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

        root -> split_coordinate = right_points.front().coordinates[recursion_depth];
        root -> left_child -> upper_bound = left_points.back().coordinates[recursion_depth];
        root -> right_child -> lower_bound = right_points.front().coordinates[recursion_depth];

        build_range_tree(left_points, recursion_depth, root -> left_child);
        build_range_tree(right_points, recursion_depth, root -> right_child);

        vector<Point>().swap(left_points);
        vector<Point>().swap(right_points);

        if(recursion_depth < num_dimensions - 1)
        {
            // NOTE: O(n lg^d n)
            root -> split_coordinate = points[0].coordinates[recursion_depth];
            vector<Point> points_sorted = points;
            std::sort(points_sorted.begin(), points_sorted.end(), [recursion_depth](Point a, Point b) -> bool {
                return (a.coordinates[recursion_depth + 1] < b.coordinates[recursion_depth + 1]);
            });
            root -> associated_node = new RTNode(points_sorted.front().coordinates[recursion_depth + 1], points_sorted.back().coordinates[recursion_depth + 1]);
            build_range_tree(points_sorted, recursion_depth + 1, root -> associated_node);
            return;
        }
    }

    int rt_iterations = 0;

    vector<Point> search_range_tree(const Range search_range, int recursion_depth, const RTNode* root)
    {
        rt_iterations++;
        if(root == nullptr)
            return vector<Point>();

        if(root -> is_leaf)
        {
            // cout << "l " << root -> point << "\n";
            if(search_range.lower_bound.coordinates[recursion_depth] <= root -> point.coordinates[recursion_depth]
                && root -> point.coordinates[recursion_depth] <= search_range.upper_bound.coordinates[recursion_depth])
                return {root -> point};
            else
                return vector<Point>();
        }
        // cout << recursion_depth << ", " << root -> lower_bound << " " << root -> upper_bound << "\n";

        if(root -> upper_bound < search_range.lower_bound.coordinates[recursion_depth]
            || search_range.upper_bound.coordinates[recursion_depth] < root -> lower_bound)
            return vector<Point>();

        if(recursion_depth < num_dimensions - 1)
        {
            if(search_range.lower_bound.coordinates[recursion_depth] <= root -> lower_bound
                && root -> upper_bound <= search_range.upper_bound.coordinates[recursion_depth])
            {
                // cout << "inside\n";
                return search_range_tree(search_range, recursion_depth + 1, root -> associated_node);
            }
        }

        vector<Point> left_child_points = search_range_tree(search_range, recursion_depth, root -> left_child);
        vector<Point> right_child_points = search_range_tree(search_range, recursion_depth, root -> right_child);

        left_child_points.insert(left_child_points.end(), right_child_points.begin(), right_child_points.end());
        return left_child_points;
    }
}