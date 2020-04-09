#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <unordered_map>
#include <cstring>
#include <array>
#include <functional>
#include <chrono>
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

array<int, 8> ans[100005];

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
    if (x[0] == y[0])
        return x[1] < y[1];
    return x[0] < y[0];
}

void dfs(int now, int depth, array<int, 8> &path)
{
    if (visited[now])
    {
        if (depth >= 3)
        {
            path[0] = depth;
            array<int, 8> path_copy;
            copy(path.begin(), next(path.begin(), depth + 1), path_copy.begin());
            sort(next(path_copy.begin(), 1), next(path_copy.begin(), depth + 1));
            ans[++ans_size] = path_copy;
            // if (path_hash.find(path_copy) == path_hash.end())
            // {
            //     path_hash.insert({path_copy, my_hash(path_copy)});
            //     ans[++ans_size] = path_copy;
            // }
        }
        return;
    }
    if (depth >= 7)
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
        dfs(node[i], 0, path);
        visited[node[i]] = 0;
    }
}
void output_data()
{
    sort(ans + 1, ans + ans_size + 1, cmp);
    printf("%d\n", ans_size);
    for (int i = 1; i <= ans_size; i++)
    {
        // start from the first element
        // sort(next(ans[i].begin(), 1), ans[i].end());
        for (int j = 1; j <= ans[i][0]; j++)
        {
            printf("%d ", ans[i][j]);
        }
        printf("\n");
    }
    return;
}
int main()
{
    auto time_start = chrono::steady_clock::now();
    read_data();
    work();
    output_data();
    auto time_end = chrono::steady_clock::now();
    auto diff = time_end - time_start;
    cout << "The program's speed: " << chrono::duration<double, milli>(diff).count() << "ms" << endl;
    return 0;
}