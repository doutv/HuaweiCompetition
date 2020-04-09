#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <unordered_map>
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
    size_t operator()(const array<int, 8> &a) const noexcept
    {
        size_t h = 0;
        for (auto e : a)
        {
            h ^= hash<int>{}(e) + INF + (h << 6) + (h >> 2);
        }
        return h;
    }
};
array<int, 8> ans[100005];
unordered_map<array<int, 8>, int, ArrayHasher> path_hash;

void read_data()
{
    memset(first, -1, sizeof(first));
    char input_path[] = "test_data.txt";
    freopen(input_path, "r", stdin);
    string s;
    int u, v;
    edge_size = 0;
    while (cin >> s)
    {
        // cout << s << endl;
        ++edge_size;
        int first_comma = s.find(',');
        int second_comma = s.find(',', first_comma + 1);
        u = stoi(s.substr(0, first_comma));
        v = stoi(s.substr(first_comma + 1, second_comma - first_comma - 1));
        node[edge_size] = u;
        edge[edge_size].u = u;
        edge[edge_size].v = v;
        edge[edge_size].nxt = first[u];
        first[u] = edge_size;
        // cout << edge[top].u << " " << edge[top].v << endl;
    }
    // fclose(stdin);
}

bool cmp(array<int, 8> &x, array<int, 8> &y)
{
    return x[0] < y[0];
}

void dfs(int now, int depth, array<int, 8> &path)
{
    if (visited[now] && depth >= 4)
    {
        path[0] = depth;
        array<int, 8> path_copy = path;
        sort(next(path_copy.begin(), 1), path_copy.end());
        if (path_hash.find(path_copy) == path_hash.end())
        {
            path_hash.insert({path_copy, 1});
            ans[++ans_size] = path_copy;
        }
        return;
    }
    if (depth >= 8)
    {
        return;
    }
    path[depth] = now;
    visited[now] = 1;
    for (int i = first[now]; i != -1; i = edge[i].nxt)
    {
        dfs(edge[i].v, depth + 1, path);
        visited[edge[i].v] = 0;
    }
}
void work()
{
    array<int, 8> path;
    for (int i = 1; i <= edge_size; i++)
    {
        dfs(node[i], 1, path);
    }
}
void output_data()
{
    sort(ans + 1, ans + ans_size + 1, cmp);
    for (int i = 1; i <= ans_size; i++)
    {
        printf("%d\n", ans[i][0]);
        // start from the first element
        // sort(next(ans[i].begin(), 1), ans[i].end());
        for (int j = 1; j < ans[i].size(); j++)
        {
            printf("%d ", ans[i][j]);
        }
        printf("\n");
    }
    return;
}
int main()
{
    read_data();
    work();
    output_data();
    return 0;
}