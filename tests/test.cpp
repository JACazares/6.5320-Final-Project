#include "../algorithms/kd-tree.h"
#include "../algorithms/range-tree.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <cassert>
#include <fstream>

using std::cout;
using std::vector;
using std::sort;
using std::pair;

int N;
std::string prefix;

vector<Point> points;
vector<pair<Point, int>> points_with_index;

vector<long long> brute_force_query(int& brute_force, const Range search_range)
{
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    for(auto p : points)
        if(totally_contained(Range(p, p), search_range))
            brute_force++;

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    auto brute_force_time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

    return {N, brute_force_time};
}

vector<long long> kd_tree_build(kd_tree::KDNode* root)
{
    vector<vector<pair<Point, int>>> points_sorted(num_dimensions, points_with_index);

    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;

    begin = std::chrono::steady_clock::now();
    for(int d = 0; d < num_dimensions; d++)
    {
        sort(points_sorted[d].begin(), points_sorted[d].end(), [d](pair<Point, int> a, pair<Point, int> b) -> bool
        {
            return a.first.coordinates[d] < b.first.coordinates[d];
        });
    }
    end = std::chrono::steady_clock::now();
    auto kd_sorting_time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

    begin = std::chrono::steady_clock::now();
    kd_tree::build_kd_tree(points_sorted, 0, root);
    end = std::chrono::steady_clock::now();
    auto kd_build_time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

    return {kd_sorting_time, kd_build_time};
}

vector<long long> kd_tree_query(const int brute_force, const Range search_range, const kd_tree::KDNode* root)
{
    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;

    begin = std::chrono::steady_clock::now();
    kd_tree::kd_iterations = 0;
    auto ans = kd_tree::search_kd_tree(search_range, root);
    end = std::chrono::steady_clock::now();
    auto kd_query_time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

    // cout << (int)ans.size() << " " << brute_force << "\n";

    assert((int)ans.size() == brute_force);
    return {kd_tree::kd_iterations, kd_query_time};
}

vector<long long> range_tree_build(range_tree::RTNode* root)
{
    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;

    vector<Point> points_sorted = points;
    begin = std::chrono::steady_clock::now();
    sort(points_sorted.begin(), points_sorted.end(), [](Point a, Point b) -> bool
    {
        return a.coordinates[0] < b.coordinates[0];
    });
    end = std::chrono::steady_clock::now();
    auto rt_sorting_time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

    root -> lower_bound = points_sorted.front().coordinates[0];
    root -> upper_bound = points_sorted.back().coordinates[0];

    begin = std::chrono::steady_clock::now();
    build_range_tree(points_sorted, 0, root);
    end = std::chrono::steady_clock::now();
    auto rt_build_time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

    return {rt_sorting_time, rt_build_time};
}

vector<long long> range_tree_query(const int brute_force, const Range search_range, const range_tree::RTNode* root)
{
    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;

    begin = std::chrono::steady_clock::now();
    range_tree::rt_iterations = 0;
    auto ans = search_range_tree(search_range, 0, root);
    end = std::chrono::steady_clock::now();

    auto rt_query_time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    assert((int)ans.size() == brute_force);

    return {range_tree::rt_iterations, rt_query_time};
}

void build(kd_tree::KDNode*& kd_root, range_tree::RTNode*& rt_root)
{
    kd_root = new kd_tree::KDNode();
    auto kd_tree_build_results = kd_tree_build(kd_root);
    cout << "KD-tree build: " << kd_tree_build_results[0] << "us sorting, " << kd_tree_build_results[1] << "us building\n";

    rt_root = new range_tree::RTNode();
    auto range_tree_build_results = range_tree_build(rt_root);
    cout << "Range tree build: " << range_tree_build_results[0] << "us sorting, " << range_tree_build_results[1] << "us building\n";

    // Write results to build_times.csv
    std::string filename = "build_times.csv";
    std::ofstream file(prefix + filename, std::ios_base::app);
    if (file.is_open())
    {
        file << N << ",Brute force,0,0\n";
        file << N << ",KD-tree," << kd_tree_build_results[0] << "," << kd_tree_build_results[1] << "\n";
        file << N << ",Range tree," << range_tree_build_results[0] << "," << range_tree_build_results[1] << "\n";
        file.close();
        cout << "Results written to build_times.csv\n";
    }
    else
        cout << "Failed to open build_times.csv\n";
}

void query(Range search_range, const kd_tree::KDNode* kd_root, const range_tree::RTNode* rt_root)
{
    int brute_force = 0;
    auto brute_force_results = brute_force_query(brute_force, search_range);
    auto kd_tree_query_results = kd_tree_query(brute_force, search_range, kd_root);
    auto range_tree_query_results = range_tree_query(brute_force, search_range, rt_root);

    cout << "Found " << brute_force << " points\n";
    cout << "Brute force query: " << brute_force_results[0] << " iterations in " << brute_force_results[1] << "us\n";
    cout << "KD-tree query: " << kd_tree_query_results[0] << " iterations in " << kd_tree_query_results[1] << "us\n";
    cout << "Range tree query: " << range_tree_query_results[0] << " iterations in " << range_tree_query_results[1] << "us\n";
    
    // Append results to query_times.csv
    std::string filename = "query_times.csv";
    std::ofstream file(prefix + filename, std::ios_base::app);
    if (file.is_open())
    {
        file << N << ",Brute force," << brute_force << "," << brute_force_results[0] << "," << brute_force_results[1] << "\n";
        file << N << ",KD-tree," << brute_force << "," << kd_tree_query_results[0] << "," << kd_tree_query_results[1] << "\n";
        file << N << ",Range tree," << brute_force << "," << range_tree_query_results[0] << "," << range_tree_query_results[1] << "\n";
        file.close();
        cout << "Results appended to query_times.csv\n";
    }
    else
        cout << "Failed to open query_times.csv\n";
}


void full_test()
{
    prefix = "../results/benchmarks d=2/";
    std::random_device rd;
    std::mt19937 g(rd());

    // Write header to query_times.csv
    std::string query_filename = "query_times.csv";
    std::ofstream query_file(prefix + query_filename);
    if (query_file.is_open())
    {
        query_file << "N,Algorithm,Points,Iterations,Time (us)\n";
        query_file.close();
        cout << "Header written to query_times.csv\n";
    }
    else
        cout << "Failed to open query_times.csv\n";


    // Write header to build_times.csv
    std::string build_filename = "build_times.csv";
    std::ofstream build_file(prefix + build_filename);
    if (build_file.is_open())
    {
        build_file << "N,Algorithm,Sorting Time (us),Build Time (us)\n";
        build_file.close();
        cout << "Header written to build_times.csv\n";
    }
    else
        cout << "Failed to open build_times.csv\n";

    N = 1e3;
    int lb = 2e2;
    int ub = 4e2;

    while(N < (int)1e6)
    {
        vector<vector<double>> coordinates_available(num_dimensions, vector<double>(N, 0));
        for(int d = 0; d < num_dimensions; d++)
        {
            for(int i = 0; i < N; i++)
                coordinates_available[d][i] = i;

            std::shuffle(coordinates_available[d].begin(), coordinates_available[d].end(), g);
        }

        points.clear();
        points_with_index.clear();
        for(int i = 0; i < N; i++)
        {
            vector<double> coordinates(num_dimensions, 0);
            for(int d = 0; d < num_dimensions; d++)
                coordinates[d] = coordinates_available[d][i];

            points.push_back(Point(coordinates));
            points_with_index.push_back({Point(coordinates), i});
        }

        kd_tree::KDNode* kd_root = nullptr;
        range_tree::RTNode* rt_root = nullptr;
        build(kd_root, rt_root);

        Range search_range;
        search_range = Range(Point(vector<double>(num_dimensions, lb)), Point(vector<double>(num_dimensions, ub)));
        query(search_range, kd_root, rt_root);

        search_range = Range(Point(vector<double>(num_dimensions, 2*lb)), Point(vector<double>(num_dimensions, (int)(1.5*ub))));
        query(search_range, kd_root, rt_root);

        N *= 3;
        lb *= 3;
        ub *= 3;
    }
}

int main()
{
    full_test();
    return 0;
}