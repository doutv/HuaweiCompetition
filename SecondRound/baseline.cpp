#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cstring>
#include <array>
#include <cmath>
#include <unordered_map>
#include <queue>
#include <pthread.h>
using namespace std;

#define LINUXOUTPUT
#define OUTPUT
#define TEST

#ifdef TEST
// 213
#include <chrono>
auto time_start = chrono::steady_clock::now();
string test_scale;
string input_path;
string output_path;
#else
string input_path = "/data/test_data.txt";
string output_path = "/projects/student/result.txt";
#endif

// 线程无关变量
typedef long long ll;
typedef pair<int, int> edge_t;
const int MAX_EDGE = 2000005;
vector<edge_t> GUV[MAX_EDGE];
vector<edge_t> GVU[MAX_EDGE];
int edge_size;
int node_size;
int node[MAX_EDGE * 2];
unordered_map<int, int> node_hashmap;
int u_arr[MAX_EDGE];
int v_arr[MAX_EDGE];
int c_arr[MAX_EDGE];
int in_degree[MAX_EDGE * 2];
int out_degree[MAX_EDGE * 2];

#ifdef TEST
// data 19630345 环数
// 1919   16032   151763   1577627  17883004
const int ANS3_MAX = 1000005;
const int ANS4_MAX = 1000005;
const int ANS5_MAX = 1000005;
const int ANS6_MAX = 2000005;
const int ANS7_MAX = 20000005;
#else
const int ANS3_MAX = 20000005;
const int ANS4_MAX = 20000005;
const int ANS5_MAX = 20000005;
const int ANS6_MAX = 20000005;
const int ANS7_MAX = 20000005;
#endif

// 线程1变量
int ans_size_1;
int ans3_1[ANS3_MAX * 3];
int ans4_1[ANS4_MAX * 4];
int ans5_1[ANS5_MAX * 5];
int ans6_1[ANS6_MAX * 6];
int ans7_1[ANS7_MAX * 7];
int *ans_1[5] = {ans3_1, ans4_1, ans5_1, ans6_1, ans7_1};
bool visited_1[MAX_EDGE];
int v1_to_u_1[MAX_EDGE];
int target_1;
int path_1[8];
int money_1[8];
unordered_map<int, vector<edge_t>> bag2_1;
bool bag3_1[MAX_EDGE];
int MAX_1; // 线程1搜索的最大节点的离散化id
// 线程2变量
int ans_size_2;
int ans3_2[ANS3_MAX * 3];
int ans4_2[ANS4_MAX * 4];
int ans5_2[ANS5_MAX * 5];
int ans6_2[ANS6_MAX * 6];
int ans7_2[ANS7_MAX * 7];
int *ans_2[5] = {ans3_2, ans4_2, ans5_2, ans6_2, ans7_2};
bool visited_2[MAX_EDGE];
int v1_to_u_2[MAX_EDGE];
int target_2;
int path_2[8];
int money_2[8];
unordered_map<int, vector<edge_t>> bag2_2;
bool bag3_2[MAX_EDGE];
int MAX_2; // 线程2搜索的最大节点的离散化id
// 线程3变量
int ans_size_3;
int ans3_3[ANS3_MAX * 3];
int ans4_3[ANS4_MAX * 4];
int ans5_3[ANS5_MAX * 5];
int ans6_3[ANS6_MAX * 6];
int ans7_3[ANS7_MAX * 7];
int *ans_3[5] = {ans3_3, ans4_3, ans5_3, ans6_3, ans7_3};
bool visited_3[MAX_EDGE];
int v1_to_u_3[MAX_EDGE];
int target_3;
int path_3[8];
int money_3[8];
unordered_map<int, vector<edge_t>> bag2_3;
bool bag3_3[MAX_EDGE];
int MAX_3; // 线程3搜索的最大节点的离散化id
// 线程4变量
int ans_size_4;
int ans3_4[ANS3_MAX * 3];
int ans4_4[ANS4_MAX * 4];
int ans5_4[ANS5_MAX * 5];
int ans6_4[ANS6_MAX * 6];
int ans7_4[ANS7_MAX * 7];
int *ans_4[5] = {ans3_4, ans4_4, ans5_4, ans6_4, ans7_4};
bool visited_4[MAX_EDGE];
int v1_to_u_4[MAX_EDGE];
int target_4;
int path_4[8];
int money_4[8];
unordered_map<int, vector<edge_t>> bag2_4;
bool bag3_4[MAX_EDGE];
int MAX_4; // 线程3搜索的最大节点的离散化id

// 线程变量
pthread_t threads[4];
pthread_attr_t attr;
void *status;
int **ans[4] = {ans_1, ans_2, ans_3, ans_4};
int ans_size;

namespace IO
{
    const int MAXSIZE = 1 << 20;
    char buf[MAXSIZE], *p1, *p2;
#define gc() (p1 == p2 && (p2 = (p1 = buf) + fread(buf, 1, MAXSIZE, stdin), p1 == p2) ? EOF : *p1++)
    inline int rd()
    {
        int x = 0;
        int16_t c = gc();
        while (!isdigit(c))
        {
            if (c == EOF)
                return c;
            c = gc();
        }
        while (isdigit(c))
            x = x * 10 + (c ^ 48), c = gc();
        return x;
    }
    inline void rd_to_line_end()
    {
        int16_t c = gc();
        while (c != '\n')
            c = gc();
    }
    char pbuf[MAXSIZE], *pp = pbuf;
    inline void push(const char &c)
    {
        if (pp - pbuf == MAXSIZE)
            fwrite(pbuf, 1, MAXSIZE, stdout), pp = pbuf;
        *pp++ = c;
    }
    inline void write(int x)
    {
        static int sta[35];
        int top = 0;
        do
        {
            sta[top++] = x % 10, x /= 10;
        } while (x);
        while (top)
            push(sta[--top] + '0');
    }
} // namespace IO

inline int max_id(float a, float b, float c, int N)
{
    float y = 0;
    int x;
    float min, max, mid;
    min = 0;
    max = 1;
    while(1) {
        mid = (max + min) / 2;
        y = a * pow((1-mid), 5.0) + b * pow(1-mid, 4.0) - c;
        if (-0.0001 > y) {
            max = mid;
        } else if ( y > 0.0001) {
            min = mid;
        } else {
            break;
        }
    }
    x = int(mid * N);
    return x;
}
static bool cmp(edge_t a, edge_t b)
{
    return a.first < b.first;
}
inline void read_data()
{
    freopen(input_path.c_str(), "r", stdin);
    int u, v, c;
    int ch;
    while (1)
    {
        u = IO::rd();
        if (u == EOF)
            break;
        v = IO::rd();
        c = IO::rd();
        node[++node_size] = u;
        node[++node_size] = v;
        ++edge_size;
        u_arr[edge_size] = u;
        v_arr[edge_size] = v;
        c_arr[edge_size] = c;
    }
    sort(node + 1, node + node_size + 1);
    node_size = unique(node + 1, node + node_size + 1) - node - 1;
    for (int i = 1; i <= node_size; i++)
    {
        node_hashmap[node[i]] = i;
    }
    for (int i = 1; i <= edge_size; i++)
    {
        u = node_hashmap[u_arr[i]];
        v = node_hashmap[v_arr[i]];
        ++in_degree[v];
        ++out_degree[u];
        GUV[u].push_back(make_pair(v_arr[i], c_arr[i]));
        GVU[v].push_back(make_pair(u_arr[i], c_arr[i]));
    }
    int r = edge_size / node_size;
    MAX_1 = max_id(float(r)/5, float(1)/4, float(12*r+15)/80, node_size);
    MAX_2 = max_id(float(r)/5, float(1)/4, float(4*r+5)/50, node_size);
    MAX_3 = max_id(float(r)/5, float(1)/4, float(4*r+5)/80, node_size);
    MAX_4 = node_size;
    // cout << MAX_1 << ' ' << MAX_2 << ' ' << MAX_3 << ' ' << MAX_4 << endl;
    for (int i = 1; i <= node_size; i++)
    {
        if (GUV[i].size())
            sort(GUV[i].begin(), GUV[i].end(), cmp);
        if (GVU[i].size())
            sort(GVU[i].begin(), GVU[i].end(), cmp);
    }
    // Topological sorting
    queue<int> q;
    for (int i = 1; i <= node_size; i++)
    {
        if (!in_degree[i] && out_degree[i])
            q.push(i);
    }
    while (!q.empty())
    {
        u = q.front();
        q.pop();
        for (int j = 0; j < GUV[u].size(); j++)
        {
            v = node_hashmap[GUV[u][j].first];
            --in_degree[v];
            if (!in_degree[v] && out_degree[v])
                q.push(v);
        }
    }
    for (int i = 1; i <= node_size; i++)
    {
        if (!out_degree[i] && in_degree[i])
            q.push(i);
    }
    while (!q.empty())
    {
        u = q.front();
        q.pop();
        for (int j = 0; j < GVU[u].size(); j++)
        {
            v = node_hashmap[GVU[u][j].first];
            --out_degree[v];
            if (!out_degree[v] && in_degree[v])
                q.push(v);
        }
    }
#ifdef TEST
    int cnt = 0;
    for (int i = 1; i <= node_size; i++)
    {
        if (!in_degree[i] || !out_degree[i])
            ++cnt;
    }
    printf("Topological sort cut %d points\n", cnt);
    auto input_time_end = chrono::steady_clock::now();
    auto input_time_diff = input_time_end - time_start;
    cout << "prehandle cost: " << chrono::duration<double, milli>(input_time_diff).count() / 1000 << "s" << endl;
#endif
}

// 线程1搜索
int get_GVU_lower_bound_1(int u)
{
    int l = 0, r = GVU[u].size() - 1;
    while (l < r)
    {
        int mid = (l + r) / 2;
        if (GVU[u][mid].first > target_1)
            r = mid;
        else
            l = mid + 1;
    }
    if (GVU[u][l].first <= target_1)
        return GVU[u].size();
    return l;
}
int get_GUV_lower_bound_1(int u)
{
    int l = 0, r = GUV[u].size() - 1;
    while (l < r)
    {
        int mid = (l + r) / 2;
        if (GUV[u][mid].first > target_1)
            r = mid;
        else
            l = mid + 1;
    }
    if (GUV[u][l].first <= target_1)
        return GUV[u].size();
    return l;
}
void flag_reverse_dfs_1(int u)
{
    // v3<--v2<--v1<--u
    for (int i = get_GVU_lower_bound_1(u); i < GVU[u].size(); i++)
    {
        // 反向第一层
        int v1 = node_hashmap[GVU[u][i].first];
        if (!in_degree[v1] || !out_degree[v1])
            continue;
        v1_to_u_1[v1] = GVU[u][i].second;
        for (int j = get_GVU_lower_bound_1(v1); j < GVU[v1].size(); j++)
        {
            // 反向第二层
            // bag2_1[v2]存的是所有能到达u的v1
            int v2 = node_hashmap[GVU[v1][j].first];
            if (!in_degree[v2] || !out_degree[v2])
                continue;
            if (bag2_1.find(v2) == bag2_1.end())
                bag2_1[v2] = vector<edge_t>{make_pair(node[v1], GVU[v1][j].second)};
            else
                bag2_1[v2].push_back(make_pair(node[v1], GVU[v1][j].second));
            // 标记能到达u的第三层节点v3
            for (int k = get_GVU_lower_bound_1(v2); k < GVU[v2].size(); k++)
            {
                int v3 = node_hashmap[GVU[v2][k].first];
                bag3_1[v3] = 1;
            }
        }
    }
}
void dfs_1(int u, int depth)
{
    for (int i = get_GUV_lower_bound_1(u); i < GUV[u].size(); i++)
    {
        int v = node_hashmap[GUV[u][i].first];
        if (!in_degree[v] || !out_degree[v])
            continue;
        if (!visited_1[v] && bag2_1.find(v) != bag2_1.end())
        {
            path_1[depth + 1] = GUV[u][i].first;
            money_1[depth] = GUV[u][i].second;
            for (int j = 0; j < bag2_1[v].size(); j++)
            {
                if (visited_1[node_hashmap[bag2_1[v][j].first]])
                    continue;
                int len = depth + 2;
                path_1[depth + 2] = bag2_1[v][j].first;
                money_1[depth + 1] = bag2_1[v][j].second;
                money_1[depth + 2] = v1_to_u_1[node_hashmap[bag2_1[v][j].first]];
                bool valid = 1;
                for (int k = 1; k <= len; k++)
                {
                    double frac;
                    if (k == len)
                        frac = (double)money_1[1] / (double)money_1[k];
                    else
                        frac = (double)money_1[k + 1] / (double)money_1[k];
                    if (frac < 0.2 || frac > 3)
                    {
                        valid = 0;
                        break;
                    }
                }
                if (valid)
                {
                    int *now_ans = ans_1[len - 3];
                    ++*(now_ans);
                    for (int k = 1; k <= len; k++)
                        *(now_ans + len * (*now_ans) + k - 1) = path_1[k];
                }
            }
        }
        if (!bag3_1[v] && depth >= 4)
            continue;
        if (!visited_1[v] && depth <= 4)
        {
            visited_1[v] = 1;
            path_1[depth + 1] = GUV[u][i].first;
            money_1[depth] = GUV[u][i].second;
            dfs_1(v, depth + 1);
            visited_1[v] = 0;
        }
    }
}
inline void *work_1(void *)
{
    int v;
#ifdef TEST
    auto time_start = chrono::steady_clock::now();
    cout << "线程1 MAX " << MAX_1 << endl;
#endif
    for (int i = 1; i <= MAX_1; i++)
    {
        if (!in_degree[i] || !out_degree[i])
            continue;
        bag2_1.clear();
        memset(bag3_1, 0, node_size + 5);
        target_1 = node[i];
        flag_reverse_dfs_1(i);
        path_1[1] = target_1;
        dfs_1(i, 1);
    }
#ifdef TEST
    auto time_end = chrono::steady_clock::now();
    auto diff = time_end - time_start;
    cout << "线程1时间: " << chrono::duration<double, milli>(diff).count() / 1000 << "s" << endl;
#endif
    pthread_exit(NULL);
}
// 线程2搜索
int get_GVU_lower_bound_2(int u)
{
    int l = 0, r = GVU[u].size() - 1;
    while (l < r)
    {
        int mid = (l + r) / 2;
        if (GVU[u][mid].first > target_2)
            r = mid;
        else
            l = mid + 1;
    }
    if (GVU[u][l].first <= target_2)
        return GVU[u].size();
    return l;
}
int get_GUV_lower_bound_2(int u)
{
    int l = 0, r = GUV[u].size() - 1;
    while (l < r)
    {
        int mid = (l + r) / 2;
        if (GUV[u][mid].first > target_2)
            r = mid;
        else
            l = mid + 1;
    }
    if (GUV[u][l].first <= target_2)
        return GUV[u].size();
    return l;
}
void flag_reverse_dfs_2(int u)
{
    // v3<--v2<--v1<--u
    for (int i = get_GVU_lower_bound_2(u); i < GVU[u].size(); i++)
    {
        // 反向第一层
        int v1 = node_hashmap[GVU[u][i].first];
        if (!in_degree[v1] || !out_degree[v1])
            continue;
        v1_to_u_2[v1] = GVU[u][i].second;
        for (int j = get_GVU_lower_bound_2(v1); j < GVU[v1].size(); j++)
        {
            // 反向第二层
            // bag2_2[v2]存的是所有能到达u的v1
            int v2 = node_hashmap[GVU[v1][j].first];
            if (!in_degree[v2] || !out_degree[v2])
                continue;
            if (bag2_2.find(v2) == bag2_2.end())
                bag2_2[v2] = vector<edge_t>{make_pair(node[v1], GVU[v1][j].second)};
            else
                bag2_2[v2].push_back(make_pair(node[v1], GVU[v1][j].second));
            // 标记能到达u的第三层节点v3
            for (int k = get_GVU_lower_bound_2(v2); k < GVU[v2].size(); k++)
            {
                int v3 = node_hashmap[GVU[v2][k].first];
                bag3_2[v3] = 1;
            }
        }
    }
}
void dfs_2(int u, int depth)
{
    for (int i = get_GUV_lower_bound_2(u); i < GUV[u].size(); i++)
    {
        int v = node_hashmap[GUV[u][i].first];
        if (!in_degree[v] || !out_degree[v])
            continue;
        if (!visited_2[v] && bag2_2.find(v) != bag2_2.end())
        {
            path_2[depth + 1] = GUV[u][i].first;
            money_2[depth] = GUV[u][i].second;
            for (int j = 0; j < bag2_2[v].size(); j++)
            {
                if (visited_2[node_hashmap[bag2_2[v][j].first]])
                    continue;
                int len = depth + 2;
                path_2[depth + 2] = bag2_2[v][j].first;
                money_2[depth + 1] = bag2_2[v][j].second;
                money_2[depth + 2] = v1_to_u_2[node_hashmap[bag2_2[v][j].first]];
                bool valid = 1;
                for (int k = 1; k <= len; k++)
                {
                    double frac;
                    if (k == len)
                        frac = (double)money_2[1] / (double)money_2[k];
                    else
                        frac = (double)money_2[k + 1] / (double)money_2[k];
                    if (frac < 0.2 || frac > 3)
                    {
                        valid = 0;
                        break;
                    }
                }
                if (valid)
                {
                    int *now_ans = ans_2[len - 3];
                    ++*(now_ans);
                    for (int k = 1; k <= len; k++)
                        *(now_ans + len * (*now_ans) + k - 1) = path_2[k];
                }
            }
        }
        if (!bag3_2[v] && depth >= 4)
            continue;
        if (!visited_2[v] && depth <= 4)
        {
            visited_2[v] = 1;
            path_2[depth + 1] = GUV[u][i].first;
            money_2[depth] = GUV[u][i].second;
            dfs_2(v, depth + 1);
            visited_2[v] = 0;
        }
    }
}
inline void *work_2(void *)
{
    int v;
#ifdef TEST
    auto time_start = chrono::steady_clock::now();
    cout << "线程2 MAX " << MAX_2 << endl;
#endif
    for (int i = MAX_1 + 1; i <= MAX_2; i++)
    {
        if (!in_degree[i] || !out_degree[i])
            continue;
        bag2_2.clear();
        memset(bag3_2, 0, node_size + 5);
        target_2 = node[i];
        flag_reverse_dfs_2(i);
        path_2[1] = target_2;
        dfs_2(i, 1);
    }
#ifdef TEST
    auto time_end = chrono::steady_clock::now();
    auto diff = time_end - time_start;
    cout << "线程2时间: " << chrono::duration<double, milli>(diff).count() / 1000 << "s" << endl;
#endif
    pthread_exit(NULL);
}
// 线程3搜索
int get_GVU_lower_bound_3(int u)
{
    int l = 0, r = GVU[u].size() - 1;
    while (l < r)
    {
        int mid = (l + r) / 2;
        if (GVU[u][mid].first > target_3)
            r = mid;
        else
            l = mid + 1;
    }
    if (GVU[u][l].first <= target_3)
        return GVU[u].size();
    return l;
}
int get_GUV_lower_bound_3(int u)
{
    int l = 0, r = GUV[u].size() - 1;
    while (l < r)
    {
        int mid = (l + r) / 2;
        if (GUV[u][mid].first > target_3)
            r = mid;
        else
            l = mid + 1;
    }
    if (GUV[u][l].first <= target_3)
        return GUV[u].size();
    return l;
}
void flag_reverse_dfs_3(int u)
{
    // v3<--v2<--v1<--u
    for (int i = get_GVU_lower_bound_3(u); i < GVU[u].size(); i++)
    {
        // 反向第一层
        int v1 = node_hashmap[GVU[u][i].first];
        if (!in_degree[v1] || !out_degree[v1])
            continue;
        v1_to_u_3[v1] = GVU[u][i].second;
        for (int j = get_GVU_lower_bound_3(v1); j < GVU[v1].size(); j++)
        {
            // 反向第二层
            // bag2_3[v2]存的是所有能到达u的v1
            int v2 = node_hashmap[GVU[v1][j].first];
            if (!in_degree[v2] || !out_degree[v2])
                continue;
            if (bag2_3.find(v2) == bag2_3.end())
                bag2_3[v2] = vector<edge_t>{make_pair(node[v1], GVU[v1][j].second)};
            else
                bag2_3[v2].push_back(make_pair(node[v1], GVU[v1][j].second));
            // 标记能到达u的第三层节点v3
            for (int k = get_GVU_lower_bound_3(v2); k < GVU[v2].size(); k++)
            {
                int v3 = node_hashmap[GVU[v2][k].first];
                bag3_3[v3] = 1;
            }
        }
    }
}
void dfs_3(int u, int depth)
{
    for (int i = get_GUV_lower_bound_3(u); i < GUV[u].size(); i++)
    {
        int v = node_hashmap[GUV[u][i].first];
        if (!in_degree[v] || !out_degree[v])
            continue;
        if (!visited_3[v] && bag2_3.find(v) != bag2_3.end())
        {
            path_3[depth + 1] = GUV[u][i].first;
            money_3[depth] = GUV[u][i].second;
            for (int j = 0; j < bag2_3[v].size(); j++)
            {
                if (visited_3[node_hashmap[bag2_3[v][j].first]])
                    continue;
                int len = depth + 2;
                path_3[depth + 2] = bag2_3[v][j].first;
                money_3[depth + 1] = bag2_3[v][j].second;
                money_3[depth + 2] = v1_to_u_3[node_hashmap[bag2_3[v][j].first]];
                bool valid = 1;
                for (int k = 1; k <= len; k++)
                {
                    double frac;
                    if (k == len)
                        frac = (double)money_3[1] / (double)money_3[k];
                    else
                        frac = (double)money_3[k + 1] / (double)money_3[k];
                    if (frac < 0.2 || frac > 3)
                    {
                        valid = 0;
                        break;
                    }
                }
                if (valid)
                {
                    int *now_ans = ans_3[len - 3];
                    ++*(now_ans);
                    for (int k = 1; k <= len; k++)
                        *(now_ans + len * (*now_ans) + k - 1) = path_3[k];
                }
            }
        }
        if (!bag3_3[v] && depth >= 4)
            continue;
        if (!visited_3[v] && depth <= 4)
        {
            visited_3[v] = 1;
            path_3[depth + 1] = GUV[u][i].first;
            money_3[depth] = GUV[u][i].second;
            dfs_3(v, depth + 1);
            visited_3[v] = 0;
        }
    }
}
inline void *work_3(void *)
{
    int v;
#ifdef TEST
    auto time_start = chrono::steady_clock::now();
    cout << "线程3 MAX " << MAX_3 << endl;
#endif
    for (int i = MAX_2 + 1; i <= MAX_3; i++)
    {
        if (!in_degree[i] || !out_degree[i])
            continue;
        bag2_3.clear();
        memset(bag3_3, 0, node_size + 5);
        target_3 = node[i];
        flag_reverse_dfs_3(i);
        path_3[1] = target_3;
        dfs_3(i, 1);
    }
#ifdef TEST
    auto time_end = chrono::steady_clock::now();
    auto diff = time_end - time_start;
    cout << "线程3时间: " << chrono::duration<double, milli>(diff).count() / 1000 << "s" << endl;
#endif
    pthread_exit(NULL);
}
// 线程4搜索
int get_GVU_lower_bound_4(int u)
{
    int l = 0, r = GVU[u].size() - 1;
    while (l < r)
    {
        int mid = (l + r) / 2;
        if (GVU[u][mid].first > target_4)
            r = mid;
        else
            l = mid + 1;
    }
    if (GVU[u][l].first <= target_4)
        return GVU[u].size();
    return l;
}
int get_GUV_lower_bound_4(int u)
{
    int l = 0, r = GUV[u].size() - 1;
    while (l < r)
    {
        int mid = (l + r) / 2;
        if (GUV[u][mid].first > target_4)
            r = mid;
        else
            l = mid + 1;
    }
    if (GUV[u][l].first <= target_4)
        return GUV[u].size();
    return l;
}
void flag_reverse_dfs_4(int u)
{
    // v3<--v2<--v1<--u
    for (int i = get_GVU_lower_bound_4(u); i < GVU[u].size(); i++)
    {
        // 反向第一层
        int v1 = node_hashmap[GVU[u][i].first];
        if (!in_degree[v1] || !out_degree[v1])
            continue;
        v1_to_u_4[v1] = GVU[u][i].second;
        for (int j = get_GVU_lower_bound_4(v1); j < GVU[v1].size(); j++)
        {
            // 反向第二层
            // bag2_4[v2]存的是所有能到达u的v1
            int v2 = node_hashmap[GVU[v1][j].first];
            if (!in_degree[v2] || !out_degree[v2])
                continue;
            if (bag2_4.find(v2) == bag2_4.end())
                bag2_4[v2] = vector<edge_t>{make_pair(node[v1], GVU[v1][j].second)};
            else
                bag2_4[v2].push_back(make_pair(node[v1], GVU[v1][j].second));
            // 标记能到达u的第三层节点v3
            for (int k = get_GVU_lower_bound_4(v2); k < GVU[v2].size(); k++)
            {
                int v3 = node_hashmap[GVU[v2][k].first];
                bag3_4[v3] = 1;
            }
        }
    }
}
void dfs_4(int u, int depth)
{
    for (int i = get_GUV_lower_bound_4(u); i < GUV[u].size(); i++)
    {
        int v = node_hashmap[GUV[u][i].first];
        if (!in_degree[v] || !out_degree[v])
            continue;
        if (!visited_4[v] && bag2_4.find(v) != bag2_4.end())
        {
            path_4[depth + 1] = GUV[u][i].first;
            money_4[depth] = GUV[u][i].second;
            for (int j = 0; j < bag2_4[v].size(); j++)
            {
                if (visited_4[node_hashmap[bag2_4[v][j].first]])
                    continue;
                int len = depth + 2;
                path_4[depth + 2] = bag2_4[v][j].first;
                money_4[depth + 1] = bag2_4[v][j].second;
                money_4[depth + 2] = v1_to_u_4[node_hashmap[bag2_4[v][j].first]];
                bool valid = 1;
                for (int k = 1; k <= len; k++)
                {
                    double frac;
                    if (k == len)
                        frac = (double)money_4[1] / (double)money_4[k];
                    else
                        frac = (double)money_4[k + 1] / (double)money_4[k];
                    if (frac < 0.2 || frac > 3)
                    {
                        valid = 0;
                        break;
                    }
                }
                if (valid)
                {
                    int *now_ans = ans_4[len - 3];
                    ++*(now_ans);
                    for (int k = 1; k <= len; k++)
                        *(now_ans + len * (*now_ans) + k - 1) = path_4[k];
                }
            }
        }
        if (!bag3_4[v] && depth >= 4)
            continue;
        if (!visited_4[v] && depth <= 4)
        {
            visited_4[v] = 1;
            path_4[depth + 1] = GUV[u][i].first;
            money_4[depth] = GUV[u][i].second;
            dfs_4(v, depth + 1);
            visited_4[v] = 0;
        }
    }
}
inline void *work_4(void *)
{
    int v;
#ifdef TEST
    auto time_start = chrono::steady_clock::now();
    cout << "线程4 MAX " << MAX_4 << endl;
#endif
    for (int i = MAX_3 + 1; i <= MAX_4; i++)
    {
        if (!in_degree[i] || !out_degree[i])
            continue;
        bag2_4.clear();
        memset(bag3_4, 0, node_size + 5);
        target_4 = node[i];
        flag_reverse_dfs_4(i);
        path_4[1] = target_4;
        dfs_4(i, 1);
    }
#ifdef TEST
    auto time_end = chrono::steady_clock::now();
    auto diff = time_end - time_start;
    cout << "线程4时间: " << chrono::duration<double, milli>(diff).count() / 1000 << "s" << endl;
#endif
    pthread_exit(NULL);
}

inline void output_data()
{
    int i, j, k, m;
#ifdef TEST
    cout << "start output" << endl;
    auto output_time_start = chrono::steady_clock::now();
#endif
    freopen(output_path.c_str(), "w", stdout);
    // int **ans[4] = {ans_1, ans_2, ans_3, ans_4};
    // int *ans_4[5] = {ans3_4, ans4_4, ans5_4, ans6_4, ans7_4};
    // int ans3_4[ANS3_MAX * 3];
    // ans3_4第一位储存线程四搜到的三元环个数
    ans_size = 0;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 5; j++)
        {
            ans_size += *(ans[i][j]);
        }
    }
    printf("%d\n", ans_size);
    for (i = 0; i < 5; i++)
    { // 节点数相同的环
        for (j = 0; j < 4; j++)
        { // 不同线程的环
            for (k = 1; k <= *(ans[j][i]); k++)
            {
                for (m = 0; m < i + 2; m++)
                {
                    IO::write(*(ans[j][i] + k * (i + 3) + m));
                    IO::push(',');
                }
                IO::write(*(ans[j][i] + k * (i + 3) + i + 2));
                IO::push('\n');
            }
        }
    }
    fwrite(IO::pbuf, 1, IO::pp - IO::pbuf, stdout);
#ifdef TEST
#ifdef LINUXOUTPUT
    freopen("/dev/tty", "w", stdout);
#else
    freopen("CON", "w", stdout);
#endif
    auto output_time_end = chrono::steady_clock::now();
    auto output_time_diff = output_time_end - output_time_start;
    cout << "output cost: " << chrono::duration<double, milli>(output_time_diff).count() / 1000 << "s" << endl;
#endif
    return;
}
#ifdef TEST
int main(int argc, char **argv)
{
    if (argc == 2)
    {
        test_scale = argv[1];
    }
    else
    {
        test_scale = "std";
    }
    input_path = "./data/" + test_scale + "/test_data.txt";
    output_path = input_path.substr(0, input_path.rfind('/')) + "/multithread+search_first.txt";
    cout << "Now running on data " + test_scale << endl;
    read_data();
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(&threads[0], NULL, work_1, NULL);
    pthread_create(&threads[1], NULL, work_2, NULL);
    pthread_create(&threads[2], NULL, work_3, NULL);
    pthread_create(&threads[3], NULL, work_4, NULL);
    pthread_attr_destroy(&attr);
    pthread_join(threads[0], &status);
    pthread_join(threads[1], &status);
    pthread_join(threads[2], &status);
    pthread_join(threads[3], &status);
    output_data();
    auto time_end = chrono::steady_clock::now();
    auto diff = time_end - time_start;
#ifdef LINUXOUTPUT
    freopen("/dev/tty", "w", stdout);
#else
    freopen("CON", "w", stdout);
#endif
    printf("ans size is %d\n", ans_size);
    cout << "The program's speed: " << chrono::duration<double, milli>(diff).count() / 1000 << "s" << endl;
    fclose(stdin);
    fclose(stdout);
    return 0;
}
#else
int main()
{
    read_data();
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(&threads[0], NULL, work_1, NULL);
    pthread_create(&threads[1], NULL, work_2, NULL);
    pthread_create(&threads[2], NULL, work_3, NULL);
    pthread_create(&threads[3], NULL, work_4, NULL);
    pthread_attr_destroy(&attr);
    pthread_join(threads[0], &status);
    pthread_join(threads[1], &status);
    pthread_join(threads[2], &status);
    pthread_join(threads[3], &status);
    output_data();
    fclose(stdin);
    fclose(stdout);
    return 0;
}
#endif