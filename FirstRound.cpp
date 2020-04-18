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

#ifdef TEST
string test_scale = "3738";
string test_input_path_s;
#endif

const int INF = 280005;
typedef long long ll;

// 出边
vector<int> GUV[INF];
// 入边
vector<int> GVU[INF];
int edge_size;

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

int u_arr[INF];
int v_arr[INF];
void read_data()
{
    test_input_path_s = "./data/" + test_scale + "/test_data.txt";
    char input_path[] = "/data/test_data.txt";
    freopen(test_input_path_s.c_str(), "r", stdin);
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
    for (int i = 1; i <= node_size; i++)
    {
        node_hashmap[node[i]] = i;
    }
    bit_size = log(node_size) / log(2) + 1;
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
void add_ans(ans_t path)
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
void dfs(int pu, int ru, int p_depth, int r_depth, ans_t &p_path, ans_t &r_path, int target)
{
    if (p_depth <= 3)
    {
        for (auto each : GUV[pu])
        {
            int v = node_hashmap[each];
            if (each < target)
                continue;
            if (v == ru)
            {
                if (p_depth + r_depth < 2)
                    continue;
                ans_t path;
                path[0] = p_depth + r_depth + 1;
                path[1] = target;
                int pos = 1;
                for (int i = 1; i <= p_depth; i++)
                    path[++pos] = p_path[i];
                for (int i = r_depth; i >= 1; i--)
                    path[++pos] = r_path[i];
                add_ans(path);
                continue;
            }
            else if (!visited[v])
            {
                visited[v] = 1;
                p_path[p_depth + 1] = each;
                dfs(v, ru, p_depth + 1, r_depth, p_path, r_path, target);
                visited[v] = 0;
            }
        }
    }
    if (r_depth <= 2)
    {
        for (auto each : GVU[ru])
        {
            int v = node_hashmap[each];
            if (each < target)
                continue;
            if (!visited[v])
            {
                visited[v] = 1;
                r_path[r_depth + 1] = each;
                dfs(pu, v, p_depth, r_depth + 1, p_path, r_path, target);
                visited[v] = 0;
            }
        }
    }
}
void work()
{
    ans_t p_path, r_path;
    for (int i = 1; i <= node_size; i++)
    {
        // #ifdef TEST
        //         if (i % 10 == 0)
        //             cout << i << endl;
        // #endif
        visited[i] = 1;
        dfs(i, i, 0, 0, p_path, r_path, node[i]);
        visited[i] = 0;
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
    string test_output_path_s = test_input_path_s.substr(0, test_input_path_s.rfind('/')) + "/output.txt";
    char output_path[] = "/projects/student/result.txt";
    freopen(test_output_path_s.c_str(), "w", stdout);
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
    cout << "The program's speed: " << chrono::duration<double, milli>(diff).count() / 1000 << "s" << endl;
#endif
    fclose(stdin);
    fclose(stdout);
    return 0;
}