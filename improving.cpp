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
// g++ -O3 -o backup backup.cpp
#define TEST
#define OUTPUT
// #define DEBUG

#ifdef TEST
string s("1004812");
#endif

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

int one_way[INF];

int u_arr[INF];
int v_arr[INF];
bool descending_cmp(int a, int b)
{
    return a > b;
}
void read_data()
{
#ifdef TEST
    string input_path = "./data/" + s + "/test_data.txt";
    freopen(input_path.c_str(), "r", stdin);
#else
    char input_path[] = "/data/test_data.txt";
    freopen(input_path, “r”, stdin);
#endif
    int u, v, c;
    while (scanf("%u,%u,%u", &u, &v, &c) != EOF)
    {
        node[++node_size] = u;
        node[++node_size] = v;
        ++edge_size;
        u_arr[edge_size] = u;
        v_arr[edge_size] = v;
    }
    //离散化
    sort(node + 1, node + node_size + 1);
    node_size = unique(node + 1, node + node_size + 1) - node - 1;
#ifdef TEST
    cout << node_size << endl;
#endif
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
    // initialize to 10, if no i/o, 0;yes, 1; not certain, 10.
    memset(one_way, 2, INF);
    for (int i = 1; i < INF; i++)
    {
        if (GUV[i].size() >= 1) {
            sort(GUV[i].begin(), GUV[i].end(), descending_cmp);
        } else {
            one_way[i] = 0;
        }
#ifdef DEBUG
        for (int &x: GUV[i]) {
            cout << x << ' ';
        }
        cout << '\n';
#endif
        if (GVU[i].size() >= 1) {
            sort(GVU[i].begin(), GVU[i].end(), descending_cmp);
        } else {
            one_way[i] = 0;
        }
#ifdef DEBUG
        for (int &x: GVU[i]) {
            cout << x << ' ';
        }
        cout << '\n';
#endif
    }
    // cout << GUV[144].size() << endl;
}
bool cmp(ans_t &x, ans_t &y)
{
    int now = 0;
    while (x[now] == y[now])
        ++now;
    return x[now] < y[now];
}
void flag_reverse_dfs(int u, int depth, int target)
{
    if (depth <= 2)
    {
        one_way[u] = 0;/*start here*/
        for (int i = 0; i < GVU[u].size(); i++)
        {
            int v = node_hashmap[GVU[u][i]];
            if (GVU[u][i] <= target) {
                return;
            } else if (!visited[v])
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
            return;
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
        if (flag[v] != target && flag[v] != -2 && depth >= 4)
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
#ifdef OUTPUT
        if (i % (node_size / 10) == 0)
            cout << i << endl;
#endif

        int target = node[i];
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
#ifdef TEST
    string output_path = "./data/" + s + "/result.txt";
    freopen(output_path.c_str(), "w", stdout);
#else
    char output_path[] = "/projects/student/result.txt";
    freopen(output_path, "w", stdout);
#endif
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
#ifdef TEST
    auto time_end = chrono::steady_clock::now();
    auto diff = time_end - time_start;
    freopen("CON", "w", stdout);
    printf("ans size is %d\n", ans_size);
    cout << "The program's speed: " << chrono::duration<double, milli>(diff).count() / 1000 << "s" << endl;
#endif
    fclose(stdin);
    fclose(stdout);
    return 0;
}