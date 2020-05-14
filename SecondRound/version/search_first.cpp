#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cstring>
#include <array>
#include <cmath>
#include <unordered_map>
#include <queue>
using namespace std;

// #define LINUXOUTPUT
#define OUTPUT
#define TEST
// #define GUESSDATA

#ifdef GUESSDATA
#include <chrono>
#include <thread>
#endif
#ifdef TEST
// 3 8 181
#include <chrono>
auto time_start = chrono::steady_clock::now();
string test_scale;
string input_path;
string output_path;
#else
string input_path = "/data/test_data.txt";
string output_path = "/projects/student/result.txt";
#endif

typedef long long ll;
typedef pair<int, int> edge_t;

const int MAX_EDGE = 2000005;
vector<edge_t> GUV[MAX_EDGE];
vector<edge_t> GVU[MAX_EDGE];
int edge_size;

bool visited[MAX_EDGE];
int node_size;
int node[MAX_EDGE * 2];
int v1_to_u[MAX_EDGE];
int path[8];
int money[8];
unordered_map<int, int> node_hashmap;
unordered_map<int, vector<edge_t>> bag2;
bool bag3[MAX_EDGE];

#ifdef TEST
// data 19630345 环数
// 1919   16032   151763   1577627  17883004
const int ANS3_MAX = 10000005;
const int ANS4_MAX = 10000005;
const int ANS5_MAX = 10000005;
const int ANS6_MAX = 10000005;
const int ANS7_MAX = 20000005;
#else
const int ANS3_MAX = 20000005;
const int ANS4_MAX = 20000005;
const int ANS5_MAX = 20000005;
const int ANS6_MAX = 20000005;
const int ANS7_MAX = 20000005;
#endif

int ans_size;
int ans3[ANS3_MAX * 3];
int ans4[ANS4_MAX * 4];
int ans5[ANS5_MAX * 5];
int ans6[ANS6_MAX * 6];
int ans7[ANS7_MAX * 7];
int *ans[5] = {ans3, ans4, ans5, ans6, ans7};

int u_arr[MAX_EDGE];
int v_arr[MAX_EDGE];
int c_arr[MAX_EDGE];

int in_degree[MAX_EDGE * 2];
int out_degree[MAX_EDGE * 2];

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
    for (int i = 1; i <= node_size; i++)
    {
        if (GUV[i].size())
            sort(GUV[i].begin(), GUV[i].end(), cmp);
        if (GVU[i].size())
            sort(GVU[i].begin(), GVU[i].end(), cmp);
    }
#ifdef GUESSDATA
    // this_thread::sleep_for(chrono::milliseconds(node_size));   //node_size=29W
    int max_in_degree = 0,
        max_out_degree = 0;
    for (i = 1; i <= node_size; i++)
    {
        max_in_degree = max(in_degree[i], max_in_degree);
        max_out_degree = max(out_degree[i], max_out_degree);
    }
    this_thread::sleep_for(chrono::milliseconds(max_in_degree * 100));
#endif
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
int target;
int get_GVU_lower_bound(int u)
{
    int l = 0, r = GVU[u].size() - 1;
    while (l < r)
    {
        int mid = (l + r) / 2;
        if (GVU[u][mid].first > target)
            r = mid;
        else
            l = mid + 1;
    }
    if (GVU[u][l].first <= target)
        return GVU[u].size();
    return l;
}
int get_GUV_lower_bound(int u)
{
    int l = 0, r = GUV[u].size() - 1;
    while (l < r)
    {
        int mid = (l + r) / 2;
        if (GUV[u][mid].first > target)
            r = mid;
        else
            l = mid + 1;
    }
    if (GUV[u][l].first <= target)
        return GUV[u].size();
    return l;
}
void flag_reverse_dfs(int u)
{
    // v3<--v2<--v1<--u
    for (int i = get_GVU_lower_bound(u); i < GVU[u].size(); i++)
    {
        // 反向第一层
        int v1 = node_hashmap[GVU[u][i].first];
        if (!in_degree[v1] || !out_degree[v1])
            continue;
        v1_to_u[v1] = GVU[u][i].second;
        for (int j = get_GVU_lower_bound(v1); j < GVU[v1].size(); j++)
        {
            // 反向第二层
            // bag2[v2]存的是所有能到达u的v1
            int v2 = node_hashmap[GVU[v1][j].first];
            if (!in_degree[v2] || !out_degree[v2])
                continue;
            if (bag2.find(v2) == bag2.end())
                bag2[v2] = vector<edge_t>{make_pair(node[v1], GVU[v1][j].second)};
            else
                bag2[v2].push_back(make_pair(node[v1], GVU[v1][j].second));
            // 标记能到达u的第三层节点v3
            for (int k = get_GVU_lower_bound(v2); k < GVU[v2].size(); k++)
            {
                int v3 = node_hashmap[GVU[v2][k].first];
                bag3[v3] = 1;
            }
        }
    }
}
void dfs(int u, int depth)
{
    for (int i = get_GUV_lower_bound(u); i < GUV[u].size(); i++)
    {
        int v = node_hashmap[GUV[u][i].first];
        if (!in_degree[v] || !out_degree[v])
            continue;
        if (!visited[v] && bag2.find(v) != bag2.end())
        {
            path[depth + 1] = GUV[u][i].first;
            money[depth] = GUV[u][i].second;
            for (int j = 0; j < bag2[v].size(); j++)
            {
                if (visited[node_hashmap[bag2[v][j].first]])
                    continue;
                int len = depth + 2;
                path[depth + 2] = bag2[v][j].first;
                money[depth + 1] = bag2[v][j].second;
                money[depth + 2] = v1_to_u[node_hashmap[bag2[v][j].first]];
                bool valid = 1;
                for (int k = 1; k <= len; k++)
                {
                    double frac;
                    if (k == len)
                        frac = (double)money[1] / (double)money[k];
                    else
                        frac = (double)money[k + 1] / (double)money[k];
                    if (frac < 0.2 || frac > 3)
                    {
                        valid = 0;
                        break;
                    }
                }
                if (valid)
                {
                    int *now_ans = ans[len - 3];
                    ++*(now_ans);
                    for (int k = 1; k <= len; k++)
                        *(now_ans + len * (*now_ans) + k - 1) = path[k];
                }
            }
        }
        if (!bag3[v] && depth >= 4)
            continue;
        if (!visited[v] && depth <= 4)
        {
            visited[v] = 1;
            path[depth + 1] = GUV[u][i].first;
            money[depth] = GUV[u][i].second;
            dfs(v, depth + 1);
            visited[v] = 0;
        }
    }
}
inline void work()
{
    int v;
    for (int i = 1; i <= node_size; i++)
    {
        if (!in_degree[i] || !out_degree[i])
            continue;
        bag2.clear();
        memset(bag3, 0, node_size + 5);
        target = node[i];
        flag_reverse_dfs(i);
        path[1] = target;
        dfs(i, 1);
    }
}

inline void output_data()
{
    int i, j, k;
    freopen(output_path.c_str(), "w", stdout);
#ifdef TEST
    auto output_time_start = chrono::steady_clock::now();
#endif
    ans_size = *(ans3) + *(ans4) + *(ans5) + *(ans6) + *(ans7);
    printf("%d\n", ans_size);
    for (i = 0; i <= 4; i++)
    {
        for (j = 1; j <= *ans[i]; j++)
        {
            for (k = 0; k < i + 2; k++)
            {
                IO::write(*(ans[i] + j * (i + 3) + k));
                IO::push(',');
            }
            IO::write(*(ans[i] + j * (i + 3) + i + 2));
            IO::push('\n');
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
    test_scale = argv[1];
    // test_scale = "std";
    input_path = "../data/" + test_scale + "/test_data.txt";
    output_path = input_path.substr(0, input_path.rfind('/')) + "/search_first.txt";
    cout << "Now running on data " + test_scale << endl;
    read_data();
    work();
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
    work();
    output_data();
    fclose(stdin);
    fclose(stdout);
    return 0;
}
#endif