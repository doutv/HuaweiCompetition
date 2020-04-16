#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cstring>
#include <array>
#include <chrono>
#include <cmath>
#include <map>
#include <unordered_set>
using namespace std;
const int INF = 280005;
typedef long long ll;
struct Edge
{
    int u, v, nxt;
} edge[INF];
int edge_size;
int first[INF];
bool visited[INF];
int node_size;
int node[INF * 2];

typedef array<int, 8> ans_t;
int ans_size;
ans_t ans[100005];

unordered_set<ll> ans_hashmap;
// 2**61-1是质数
const ll mod = 2305843009213693951;

void read_data()
{
    memset(first, -1, sizeof(first));
    char test_input_path[] = "./data/38252/test_data.txt";
    char input_path[] = "/data/test_data.txt";
    freopen(test_input_path, "r", stdin);
    string s;
    int u, v;
    int u_arr[INF];
    while (cin >> s)
    {
        int first_comma = s.find(',');
        int second_comma = s.find(',', first_comma + 1);
        u = stoi(s.substr(0, first_comma));
        v = stoi(s.substr(first_comma + 1, second_comma - first_comma - 1));
        node[++node_size] = u;
        node[++node_size] = v;
        ++edge_size;
        u_arr[edge_size] = u;
        edge[edge_size].u = u;
        edge[edge_size].v = v;
    }
    //离散化
    sort(node + 1, node + node_size + 1);
    node_size = unique(node + 1, node + node_size + 1) - node - 1;
    for (int i = 1; i <= edge_size; i++)
    {
        int u = lower_bound(node + 1, node + node_size + 1, u_arr[i]) - node;
        edge[i].nxt = first[u];
        first[u] = i;
    }
}
bool cmp(ans_t &x, ans_t &y)
{
    int now = 0;
    while (x[now] == y[now])
        ++now;
    return x[now] < y[now];
}
void add_ans(ans_t path)
{
    ans_t path_copy = path;
    pair<int, int> minx{INF, INF};
    int len = path_copy[0];
    for (int i = 1; i <= len; i++)
    {
        if (path_copy[i] < minx.second)
        {
            minx.first = i;
            minx.second = path_copy[i];
        }
    }
    path_copy[1] = path[minx.first];
    int hash_id = lower_bound(node + 1, node + node_size + 1, path[minx.first]) - node;
    ll hash_key = (ll)(hash_id);
    // 有向图，一定是右边
    int cnt = 1;
    int ri = minx.first == len ? 1 : minx.first + 1;
    while (ri != minx.first)
    {
        ll hash_id = lower_bound(node + 1, node + node_size + 1, path[ri]) - node;
        hash_key *= hash_id + (1 << cnt);
        hash_key %= mod;
        ++cnt;
        path_copy[cnt] = path[ri];
        ri = ri == len ? 1 : ri + 1;
    }
    hash_key += (ll)len << 61;
    if (ans_hashmap.find(hash_key) == ans_hashmap.end())
    {
        ans_hashmap.insert(hash_key);
        ans[++ans_size] = path_copy;
    }
}
void dfs(int now, int depth, ans_t &path, int target)
{
    for (int i = first[now]; i != -1; i = edge[i].nxt)
    {
        if (edge[i].v == target)
        {
            // 1->2->1
            if (depth >= 2)
            {
                path[depth + 1] = target;
                path[0] = depth + 1;
                add_ans(path);
            }
            continue;
        }
        // edge[i].v==target时，不能继续dfs
        int v = lower_bound(node + 1, node + node_size + 1, edge[i].v) - node;
        if (!visited[v] && depth <= 5)
        {
            visited[v] = 1;
            path[depth + 1] = edge[i].v;
            dfs(v, depth + 1, path, target);
            visited[v] = 0;
        }
    }
}
void work()
{
    for (int i = 1; i <= node_size; i++)
    {
        // cout << i << " " << node[i] << endl;
        ans_t path;
        dfs(i, 0, path, node[i]);
    }
}
void output_data()
{
    char test_output_path[] = "./output.txt";
    char output_path[] = "/projects/student/result.txt";
    freopen(test_output_path, "w", stdout);
    sort(ans + 1, ans + ans_size + 1, cmp);
    printf("%d\n", ans_size);
    for (int i = 1; i <= ans_size; i++)
    {
        int len = ans[i][0];
        for (int j = 1; j < len; j++)
        {
            printf("%d,", ans[i][j]);
        }
        printf("%d\n", ans[i][len]);
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
    // if test
    cout << "The program's speed: " << chrono::duration<double, milli>(diff).count() << "ms" << endl;
    fclose(stdin);
    fclose(stdout);
    return 0;
}