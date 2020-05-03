#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cstring>
#include <array>
#include <chrono>
#include <cmath>
#include <unordered_map>
using namespace std;

auto time_start = chrono::steady_clock::now();

// #define LINUXOUTPUT
#define OUTPUT
#define TEST

string input_path = "/data/test_data.txt";
string output_path = "/projects/student/result.txt";
#ifdef TEST
string test_scale = "std";
string test_input_path_s = "./data/" + test_scale + "/test_data.txt";
string test_output_path_s = test_input_path_s.substr(0, test_input_path_s.rfind('/')) + "/output.txt";
#endif

const int MAX_EDGE = 2000005;
typedef long long ll;

vector<pair<int, int>> GUV[MAX_EDGE];
vector<pair<int, int>> GVU[MAX_EDGE];
int edge_size;

bool visited[MAX_EDGE];
int flag[MAX_EDGE];
int node_size;
int node[MAX_EDGE * 2];
unordered_map<int, int> node_hashmap;

typedef array<int, 8> ans_t;
int ans_size;
// const int ANS_MAX = 20000005;
const int ANS_MAX = 4000005;
ans_t ans[ANS_MAX];

int u_arr[MAX_EDGE];
int v_arr[MAX_EDGE];
int c_arr[MAX_EDGE];

// pre--c1-->node--c2-->nxt
// st_from_node[i]=[pre,c1,c2,nxt]
// for each i, nxt is fixed. (i to nxt=one-to-one)
// 0.2<=c2/c1<=3
int len_st_from_node;
vector<tuple<int, int, vector<pair<int, int>>>> st_from_node;

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

inline void read_data()
{
    freopen(test_input_path_s.c_str(), "r", stdin);
    int u, v, c;
    int ch;
    register int i;
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
    for (i = 1; i <= node_size; i++)
    {
        node_hashmap[node[i]] = i;
    }
    for (i = 1; i <= edge_size; i++)
    {
        u = node_hashmap[u_arr[i]];
        v = node_hashmap[v_arr[i]];
        GUV[u].push_back(make_pair(v_arr[i], c_arr[i]));
        GVU[v].push_back(make_pair(u_arr[i], c_arr[i]));
    }
#ifdef TEST
    auto input_time_end = chrono::steady_clock::now();
    auto input_time_diff = input_time_end - time_start;
    cout << "prehandle cost: " << chrono::duration<double, milli>(input_time_diff).count() / 1000 << "s" << endl;
#endif
}
inline bool cmp(ans_t &x, ans_t &y)
{
    int now = 0;
    while (x[now] == y[now])
        ++now;
    return x[now] < y[now];
}

void flag_reverse_dfs(int u, int depth, int target, float nxtc)
{
    if (depth <= 3)
    {
        register int i;
        int v;
        float nowc;
        for (i = 0; i < GVU[u].size(); i++)
        {
            v = node_hashmap[GVU[u][i].first];
            nowc = GVU[u][i].second;
            if (!(0.2 <= nxtc / nowc <= 3))
                continue;
            if (!visited[v] && GVU[u][i].first > target)
            {
                visited[v] = 1;
                flag[v] = target;
                flag_reverse_dfs(v, depth + 1, target, nowc);
                visited[v] = 0;
            }
        }
    }
}
void dfs(int u, int depth, ans_t &path, int target, float prec)
{
    register int i;
    int v;
    for (i = 0; i < GUV[u].size(); i++)
    {
        if (GUV[u][i].first <= target)
            continue;
        float nowc = GUV[u][i].second;
        if (!(0.2 <= nowc / prec <= 3))
            continue;
        v = node_hashmap[GUV[u][i].first];
        if (flag[v] == -2 && visited[v] == 0)
        {
            if (depth >= 2)
            {
                path[0] = depth + 1;
                path[depth + 1] = GUV[u][i].first;
                ans[++ans_size] = path;
            }
        }
        if (flag[v] != target && flag[v] != -2 && depth >= 4)
            continue;
        if (!visited[v] && depth <= 5)
        {
            visited[v] = 1;
            path[depth + 1] = GUV[u][i].first;
            dfs(v, depth + 1, path, target, nowc);
            visited[v] = 0;
        }
    }
}
void iter_st_from_node(int node, int target)
{
    ans_t path;
    register int i, j;
    float c1, c2;
    int u, v;
    for (i = 0; i < GUV[node].size(); i++)
    {
        if (GUV[node][i].first < target)
            continue;
        c2 = GUV[node][i].second;
        u = node_hashmap[GUV[node][i].first];
        memset(flag, -1, node_size + 5);
        for (j = 0; j < GVU[node].size(); j++)
        {
            if (GVU[node][j].first < target)
                continue;
            c1 = GVU[node][j].second;
            v = node_hashmap[GVU[node][j].first];
            if (0.2 <= c2 / c1 <= 3)
            {
                flag[v] = -2;
            }
            flag_reverse_dfs(v, 1, target, c1);
        }
        path[1] = target;
        path[2] = GUV[node][i].first;
        dfs(u, 1, path, target, c2);
    }
}
inline void work()
{
    memset(flag, -1, node_size + 5);
    ans_t path;
    int target;
    register int i, j;
    for (i = 1; i <= node_size; i++)
    {
        target = node[i];
        iter_st_from_node(i, target);
    }
}
inline void output_data()
{
    register int i, j;
    freopen(test_output_path_s.c_str(), "w", stdout);
    sort(ans + 1, ans + ans_size + 1, cmp);
#ifdef TEST
    auto output_time_start = chrono::steady_clock::now();
#endif
    printf("%d\n", ans_size);
    for (i = 1; i <= ans_size; i++)
    {
        for (j = 1; j < ans[i][0]; j++)
        {
            IO::write(ans[i][j]);
            IO::push(',');
        }
        IO::write(ans[i][ans[i][0]]);
        IO::push('\n');
    }
    fwrite(IO::pbuf, 1, IO::pp - IO::pbuf, stdout);
#ifdef LINUXOUTPUT
    freopen("/dev/tty", "w", stdout);
#else
    freopen("CON", "w", stdout);
#endif
#ifdef TEST
    auto output_time_end = chrono::steady_clock::now();
    auto output_time_diff = output_time_end - output_time_start;
    cout << "output cost: " << chrono::duration<double, milli>(output_time_diff).count() / 1000 << "s" << endl;
#endif
    return;
}
int main()
{
    read_data();
    work();
    output_data();
#ifdef TEST
    auto time_end = chrono::steady_clock::now();
    auto diff = time_end - time_start;
#ifdef LINUXOUTPUT
    freopen("/dev/tty", "w", stdout);
#else
    freopen("CON", "w", stdout);
#endif
    printf("ans size is %d\n", ans_size);
    cout << "The program's speed: " << chrono::duration<double, milli>(diff).count() / 1000 << "s" << endl;
#endif
    fclose(stdin);
    fclose(stdout);
    return 0;
}