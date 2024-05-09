#include "kd-tree.h"
#include "range-tree.h"
#include <iostream>
#include <chrono>
#include <vector>

using std::cout;
using std::vector;

int N = (int)1e7;
Range search_range = Range(Point(12000, 1567), Point(46000, 27000));
vector<Point> points;
vector<pair<Point, int>> points_with_index;

void kd_test()
{
    KDNode* root = new KDNode();

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    build_kd_tree(points_with_index, points_with_index, 0, root);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    auto kd_build_time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

    begin = std::chrono::steady_clock::now();
    auto ans = search_kd_tree(search_range, root);
    end = std::chrono::steady_clock::now();

    auto kd_query_time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    cout << "KD size: " << ans.size() << "\n";

    int brute_force = 0;
    begin = std::chrono::steady_clock::now();
    for(auto i : points_with_index)
    {
        if(search_range.lower_bound.x <= i.first.x
            && i.first.x <= search_range.upper_bound.x
            && search_range.lower_bound.y <= i.first.y
            && i.first.y <= search_range.upper_bound.y)
            brute_force++;
    }
    end = std::chrono::steady_clock::now();
    auto brute_force_time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    cout << "Brute force size: " << brute_force << "\n";

    cout << kd_iterations << "\n";
    cout << N << "\n";

    cout << "KD build time: " << kd_build_time << "ms\n";
    cout << "KD query time: " << kd_query_time << "ms\n";
    cout << "Brute force time: " << brute_force_time << "ms\n";
}

void range_tree_test()
{
    RTNode* root = new RTNode(points.front().x, points.back().x);

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    build_range_tree(points, 0, root);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    auto rt_build_time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

    begin = std::chrono::steady_clock::now();
    auto ans = search_range_tree(search_range, 0, root);
    end = std::chrono::steady_clock::now();

    auto rt_query_time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    cout << "RT size: " << ans.size() << "\n";

    int brute_force = 0;
    begin = std::chrono::steady_clock::now();
    for(auto i : points)
    {
        if(search_range.lower_bound.x <= i.x
            && i.x <= search_range.upper_bound.x
            && search_range.lower_bound.y <= i.y
            && i.y <= search_range.upper_bound.y)
            brute_force++;
    }
    end = std::chrono::steady_clock::now();
    auto brute_force_time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    cout << "Brute force size: " << brute_force << "\n\n";

    cout << "RT iretations: " << rt_iterations << "\n";
    cout << "Brute force iterations: " << N << "\n\n";

    cout << "RT build time: " << rt_build_time << "ms\n";
    cout << "RT query time: " << rt_query_time << "ms\n";
    cout << "Brute force time: " << brute_force_time << "ms\n\n";

}

int main()
{
    for(int i = 0; i < N; i++)
    {
        points.pb(Point(i, i));
        points_with_index.pb({Point(i, i), i});
    }
    
    // kd_tree_test();
    range_tree_test();
    return 0;
}