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

// struct ArrayHasher
// {
//     size_t operator()(const array<int, 8> &a) const
//     {
//         size_t h = 0;
//         for (auto e : a)
//         {
//             h ^= hash<int>{}(e) + 0x9e3779b9 + (h << 6) + (h >> 2);
//         }
//         return h;
//     }
// };
// array<int, 8> ans[100005];
// unordered_map<array<int, 8>, int, ArrayHasher> path_hash;
typedef long long ll;
const ll mod = 1152921504606846883;
int main()
{
    ll x = mod + ((ll)7 << 60);
    cout << x << endl;
    return 0;
}