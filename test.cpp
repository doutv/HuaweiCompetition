#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <unordered_set>
#include <cstring>
#include <array>
#include <functional>
using namespace std;

const int INF = 280005;
struct Edge
{
    int u, v, nxt;
} edge[INF];
int edge_size;
int first[INF];
bool visited[INF];
int node[INF];
int ans_size;

struct ArrayHasher
{
    size_t operator()(const array<int, 8> &a)
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
unordered_set<array<int, 8>, int, ArrayHasher> path_hash;
