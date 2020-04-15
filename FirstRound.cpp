#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <unordered_map>
#include <cstring>
#include <array>
#include <functional>
#include <chrono>
#include <cmath>
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
int found_numbers=0;
double real_size=0;

array<int, 8> ans[100005];
array<double, 2> found[100005];

// bool hash_f(array<int, 8> path)
// {
//     double feature = 0;
//     for (int i=1; i<=path[0]; i++)
//     {
//         int j;
//         j = i + 1;
//         if (j > path[0])
//         {
//             j = 1;
//         }
//         double out=path[j];
//         double in=path[i];
//         double double_feature = pow(2, 2*in + out);
//         feature += double_feature;
//     }
//     for (int i=0; i<100005; i++)
//     {
//         if (found[i][0] == 0)
//         {
//             found[i][0] = path[0];
//             found[i][1] = feature;
//             return true;
//         }
//         if (found[i][0] == path[0] and found[i][1] == feature)
//         {
//             return false;
//         }
//     }
//     return true;
// }

array<int, 8> sort_path(array<int, 8> path)
{
    int smallest=path[1];
    int smallest_position=1;
    for (int i=1; i<=path[0]; i++)
    {
        if (path[i] < smallest)
        {
            smallest = path[i];
            smallest_position = i;
        }
    }
    array<int, 8> sorted_path;
    sorted_path[0] = path[0];
    for (int i=1; i <= path[0]; i++)
    {
        if (i >= smallest_position)
        {
            sorted_path[i-smallest_position+1] = path[i];
        } else
        {
            sorted_path[i+path[0]-smallest_position+1] = path[i];
        }
    }
    return sorted_path;
}

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

void dfs(int now, int depth, array<int, 8> &path, int target)
{
    if (now == target and depth)
    {
        if (depth >= 3)
        {
            path[0] = depth;
            path[depth] = now;
            array<int, 8> path_copy;
            path_copy = sort_path(path);
            ans[++ans_size] = path_copy;
            real_size += 1 / depth;
            // int hash_value = hash_f(path_copy);
            // if (hash_value)
            // {
            //     ans[++ans_size] = path_copy;
            //     for (int i=0; i<8; i++)
            //     {
            //         cout << path_copy[i] << ' ';
            //     }
            //     cout << '\n';
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
        dfs(edge[i].v, depth + 1, path, target);
        visited[edge[i].v] = 0;
    }
}
void work()
{
    array<int, 8> path;
    for (int i = 1; i <= edge_size; i++)
    {
        dfs(node[i], 0, path, node[i]);
        visited[node[i]] = 0;
    }
}
void output_data()
{
    sort(ans + 1, ans + ans_size + 1, cmp);
    int r_ans_size = static_cast<int>(real_size);
    printf("%d\n", (int)real_size);
    for (int i = 1; i <= ans_size; i = i + ans[i][0])
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