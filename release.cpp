#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cstring>
#include <array>
#include <chrono>
#include <cmath>
#include <map>
#include <unordered_map>
#include <unordered_set>
using namespace std;

char input_path[] = "/data/test_data.txt";
char output_path[] = "/projects/student/result.txt";

const int INF = 280005;
typedef long long ll;

// 出边
vector<int> GUV[INF];
// 入边
vector<int> GVU[INF];
int edge_size;

bool visited[INF];
int flag[INF];
int node_size;
int node[INF * 2];
unordered_map<int, int> node_hashmap;

typedef array<int, 8> ans_t;
int ans_size;
ans_t ans[4000005];

int u_arr[INF];
int v_arr[INF];

void read_data()
{
    freopen(input_path, "r", stdin);
    int u, v;
    int ch;
    while (1)
    {
        ch = getchar();
        if (ch == EOF)
            break;
        u = 0;
        while ('0' <= ch && ch <= '9')
        {
            u = (u << 3) + (u << 1) - '0' + ch;
            ch = getchar();
        }
        v = 0;
        while ('0' <= (ch = getchar()) && ch <= '9')
            v = (v << 3) + (v << 1) - '0' + ch;
        ch = getchar();
        while ('0' <= ch && ch <= '9')
            ch = getchar();
        node[++node_size] = u;
        node[++node_size] = v;
        ++edge_size;
        u_arr[edge_size] = u;
        v_arr[edge_size] = v;
    }
    //离散化
    sort(node + 1, node + node_size + 1);
    node_size = unique(node + 1, node + node_size + 1) - node - 1;
    for (int i = 1; i <= node_size; i++)
    {
        node_hashmap[node[i]] = i;
    }
    for (int i = 1; i <= edge_size; i++)
    {
        int u = node_hashmap[u_arr[i]];
        int v = node_hashmap[v_arr[i]];
        GUV[u].push_back(v_arr[i]);
        GVU[v].push_back(u_arr[i]);
    }
}
bool cmp(ans_t &x, ans_t &y)
{
    int now = 0;
    while (x[now] == y[now])
        ++now;
    return x[now] < y[now];
}
void flag_traverse_dfs(int u, int depth, int target)
{
    if (depth <= 2)
    {
        for (int i = 0; i < GUV[u].size(); i++)
        {
            int v = node_hashmap[GUV[u][i]];
            if (!visited[v] && GUV[u][i] > target)
            {
                visited[v] = 1;
                flag[v] = target;
                flag_traverse_dfs(v, depth + 1, target);
                visited[v] = 0;
            }
        }
    }
}
void flag_reverse_dfs(int u, int depth, int target)
{
    if (depth <= 2)
    {
        for (int i = 0; i < GVU[u].size(); i++)
        {
            int v = node_hashmap[GVU[u][i]];
            if (!visited[v] && GVU[u][i] > target)
            {
                visited[v] = 1;
                flag[v] = target;
                flag_reverse_dfs(v, depth + 1, target);
                visited[v] = 0;
            }
        }
    }
}
void dfs(int u, int depth, ans_t &path, int target)
{
    for (int i = 0; i < GUV[u].size(); i++)
    {
        int v = node_hashmap[GUV[u][i]];
        if (GUV[u][i] < target)
            continue;
        if (flag[v] == -2 && visited[v] == 0)
        {
            if (depth >= 2)
            {
                path[0] = depth + 1;
                path[depth + 1] = GUV[u][i];
                ans[++ans_size] = path;
            }
        }
        if (GUV[u][i] == target)
            continue;
        if (flag[v] != target && flag[v] != -2)
            continue;
        if (!visited[v] && depth <= 5)
        {
            visited[v] = 1;
            path[depth + 1] = GUV[u][i];
            dfs(v, depth + 1, path, target);
            visited[v] = 0;
        }
    }
}
void work()
{
    memset(flag, -1, node_size + 5);
    ans_t path;
    for (int i = 1; i <= node_size; i++)
    {
        int target = node[i];
        flag_traverse_dfs(i, 0, target);
        flag_reverse_dfs(i, 0, target);
        for (int j = 0; j < GVU[i].size(); j++)
        {
            int v = node_hashmap[GVU[i][j]];
            // 特殊标记倒走一步的点
            flag[v] = -2;
        }
        path[1] = target;
        dfs(i, 1, path, target);
        for (int j = 0; j < GVU[i].size(); j++)
        {
            int v = node_hashmap[GVU[i][j]];
            flag[v] = target;
        }
    }
}
void out(int x)
{
    if (x > 9)
        out(x / 10);
    putchar(x % 10 + '0');
}

void output_data()
{
    freopen(output_path, "w", stdout);
    sort(ans + 1, ans + ans_size + 1, cmp);
    printf("%d\n", ans_size);
    for (int i = 1; i <= ans_size; i++)
    {
        int len = ans[i][0];
        for (int j = 1; j < len; j++)
        {
            out(ans[i][j]);
            putchar(',');
        }
        out(ans[i][len]);
        putchar('\n');
    }
    return;
}
int main()
{
    auto time_start = chrono::steady_clock::now();
    read_data();
    work();
    output_data();
    fclose(stdin);
    fclose(stdout);
    return 0;
}