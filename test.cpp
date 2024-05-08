#include "kd-tree.h"
#include <iostream>
#include <chrono>

using std::cout;

int main()
{
    vector<pair<Point, int>> points;
    int N = (int)2e7;
    for(int i = 0; i < N; i++)
        points.pb({Point(i, i), i});
    
    KDNode* root = new KDNode();

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    build_kd_tree(points, points, 0, root);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    auto kd_build_time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

    Range search_range = Range(Point(15675, 15675), Point(150675, 45675));

    begin = std::chrono::steady_clock::now();
    auto ans = search_kd_tree(search_range, root);
    end = std::chrono::steady_clock::now();

    auto kd_query_time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    cout << "KD size: " << ans.size() << "\n";

    int brute_force = 0;
    begin = std::chrono::steady_clock::now();
    for(auto i : points)
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
    return 0;
}