#include "kd-tree.h"
#include "range-tree.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <cassert>

using std::cout;
using std::vector;
using std::sort;

int N = (int)5e4;
Range search_range = Range();

vector<Point> points;
vector<pair<Point, int>> points_with_index;

void kd_tree_test()
{
    vector<vector<pair<Point, int>>> points_sorted(num_dimensions, points_with_index);
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    for(int d = 0; d < num_dimensions; d++)
    {
        sort(points_sorted[d].begin(), points_sorted[d].end(), [d](pair<Point, int> a, pair<Point, int> b) -> bool
        {
            return a.first.coordinates[d] < b.first.coordinates[d];
        });
    }

    // for(int d = 0; d < num_dimensions; d++)
    // {
    //     for(auto p : points_sorted[d])
    //         cout << p.first << " ";
    //     cout << "\n";
    // }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    auto sorting_time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

    cout << "Sorting time: " << sorting_time << "us\n\n";

    KDNode* root = new KDNode();

    begin = std::chrono::steady_clock::now();
    build_kd_tree(points_sorted, 0, root);
    end = std::chrono::steady_clock::now();

    auto kd_build_time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

    begin = std::chrono::steady_clock::now();
    auto ans = search_kd_tree(search_range, root);
    end = std::chrono::steady_clock::now();

    auto kd_query_time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

    int brute_force = 0;
    begin = std::chrono::steady_clock::now();
    for(auto i : points_with_index)
        if(totally_contained(Range(i.first, i.first), search_range))
            brute_force++;

    end = std::chrono::steady_clock::now();
    auto brute_force_time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

    assert((int)ans.size() == brute_force);
    cout << "Points in range: " << brute_force << "\n\n";

    cout << "KD iterations: " << kd_iterations << "\n";
    cout << "Brute force iterations: " << N << "\n\n";

    cout << "KD build time: " << kd_build_time << "us\n";
    cout << "KD query time: " << kd_query_time << "us\n";
    cout << "Brute force time: " << brute_force_time << "us\n\n";
}

void range_tree_test()
{
    vector<Point> points_sorted = points;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    sort(points_sorted.begin(), points_sorted.end(), [](Point a, Point b) -> bool
    {
        return a.coordinates[0] < b.coordinates[0];
    });

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    auto sorting_time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

    cout << "Sorting time: " << sorting_time << "us\n\n";

    // for(auto p : points_sorted)
    //     cout << p << " ";
    // cout << "\n";

    RTNode* root = new RTNode(points_sorted.front().coordinates[0], points_sorted.back().coordinates[0]);

    begin = std::chrono::steady_clock::now();
    build_range_tree(points_sorted, 0, root);
    end = std::chrono::steady_clock::now();

    auto rt_build_time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

    begin = std::chrono::steady_clock::now();
    auto ans = search_range_tree(search_range, 0, root);
    end = std::chrono::steady_clock::now();

    auto rt_query_time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

    int brute_force = 0;
    begin = std::chrono::steady_clock::now();
    for(auto p : points)
    {
        if(totally_contained(Range(p, p), search_range))
            brute_force++;
    }

    end = std::chrono::steady_clock::now();
    auto brute_force_time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    assert((int)ans.size() == brute_force);
    cout << "Points in range: " << brute_force << "\n\n";

    cout << "RT iretations: " << rt_iterations << "\n";
    cout << "Brute force iterations: " << N << "\n\n";

    cout << "RT build time: " << rt_build_time << "us\n";
    cout << "RT query time: " << rt_query_time << "us\n";
    cout << "Brute force time: " << brute_force_time << "us\n\n";

}

int main()
{
    std::random_device rd;
    std::mt19937 g(rd());

    vector<vector<double>> coordinates_available(num_dimensions, vector<double>(N, 0));
    for(int d = 0; d < num_dimensions; d++)
    {
        for(int i = 0; i < N; i++)
            coordinates_available[d][i] = i;

        std::shuffle(coordinates_available[d].begin(), coordinates_available[d].end(), g);
    }

    for(int i = 0; i < N; i++)
    {
        vector<double> coordinates(num_dimensions, 0);
        for(int d = 0; d < num_dimensions; d++)
            coordinates[d] = coordinates_available[d][i];

        points.push_back(Point(coordinates));
        points_with_index.push_back({Point(coordinates), i});
    }

    // for(auto p : points)
    //     cout << p << " ";
    // cout << "\n";

    search_range = Range(Point(vector<double>(num_dimensions, 0)), Point(vector<double>(num_dimensions, 1e5)));
    
    // kd_tree_test();
    range_tree_test();
    return 0;
}