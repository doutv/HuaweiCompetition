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

#define TEST

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
unordered_map<int, int> node_hashmap;

typedef array<int, 8> ans_t;
int ans_size;
ans_t ans[100005];

unordered_set<ll> ans_hashmap;
// 2^60上一个质数
const ll mod = 1152921504606846883;
int bit_size;
void read_data()
{
    memset(first, -1, sizeof(first));
    char test_input_path[] = "./data/58284/test_data.txt";
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
    for (int i = 1; i <= node_size; i++)
    {
        node_hashmap[node[i]] = i;
    }
    bit_size = log(node_size) / log(2) + 1;
    for (int i = 1; i <= edge_size; i++)
    {
        int u = node_hashmap[u_arr[i]];
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
void add_ans(ans_t &path)
{
    ll len = path[0];
    ll hash_key = 1;
    for (ll i = 1; i <= len; i++)
    {
        ll hash_id = node_hashmap[path[i]];
        hash_key ^= hash_id << ((i - 1) * bit_size) % 60;
        hash_key %= mod;
    }
    hash_key += (ll)len << 60;
    if (ans_hashmap.find(hash_key) == ans_hashmap.end())
    {
        ans_hashmap.insert(hash_key);
        ans[++ans_size] = path;
    }
}
void dfs(int u, int depth, ans_t &path, int target)
{
    for (int i = first[u]; i != -1; i = edge[i].nxt)
    {
        int v = node_hashmap[edge[i].v];
        if (v < target)
            continue;
        if (v == target)
        {
            // 1->2->1
            if (depth >= 3)
            {
                path[0] = depth;
                add_ans(path);
            }
        }
        // 找到环后不能继续dfs
        else if (!visited[v] && depth <= 6)
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
    ans_t path;
    for (int i = 1; i <= node_size; i++)
    {
        visited[i] = 1;
        path[1] = node[i];
        dfs(i, 1, path, i);
        visited[i] = 0;
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
#ifdef TEST
    auto time_end = chrono::steady_clock::now();
    auto diff = time_end - time_start;
    cout << "The program's speed: " << chrono::duration<double, milli>(diff).count() / 1000 << "s" << endl;
#endif
    fclose(stdin);
    fclose(stdout);
    return 0;
}