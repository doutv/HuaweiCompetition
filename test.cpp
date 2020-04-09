#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <unordered_map>
#include <cstring>
#include <array>
#include <functional>
using namespace std;

// const int INF = 280005;
// struct Edge
// {
//     int u, v, nxt;
// } edge[INF];
// int edge_size;
// int first[INF];
// bool visited[INF];
// int node[INF];
// int ans_size;

struct ArrayHasher
{
    size_t operator()(const array<int, 8> &a) const
    {
        size_t h = 0;
        for (auto e : a)
        {
            h ^= hash<int>{}(e) + 0x9e3779b9 + (h << 6) + (h >> 2);
        }
        return h;
    }
};
array<int, 8> ans[100005];
unordered_map<array<int, 8>, int, ArrayHasher> path_hash;

int main()
{
    array<int, 4> a{1, 2, 3, 4};
    array<int, 4> a_copy;
    copy(a.begin(), next(a.begin(), 3), a_copy.begin());
    for (auto each : a_copy)
    {
        cout << each << " ";
    }
    return 0;
}