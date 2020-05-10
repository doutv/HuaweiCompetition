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

#ifdef TEST
#include <chrono>
auto time_start = chrono::steady_clock::now();
string test_scale = "9153";
string input_path = "../data/" + test_scale + "/test_data.txt";
string output_path = input_path.substr(0, input_path.rfind('/')) + "/output.txt";
#else
string input_path = "/data/test_data.txt";
string output_path = "/projects/student/result.txt";
#endif

typedef long long ll;
typedef array<int, 8> ans_t;

const int MAX_EDGE = 2000005;
vector<pair<int, int>> GUV[MAX_EDGE];
vector<pair<int, int>> GVU[MAX_EDGE];
int edge_size;

bool visited[MAX_EDGE];
int flag[MAX_EDGE];
bool is_end[MAX_EDGE];
int node_size;
int node[MAX_EDGE * 2];
unordered_map<int, int> node_hashmap;
float c_prenode_to_node[MAX_EDGE];

int ans_size;
const int ANS_MAX = 20000005;
// const int ANS_MAX = 4000005;
ans_t ans[ANS_MAX];

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

inline void read_data()
{
    freopen(input_path.c_str(), "r", stdin);
    int u, v, c;
    int ch;
    register int i, j;
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
        ++in_degree[v];
        ++out_degree[u];
        GUV[u].push_back(make_pair(v_arr[i], c_arr[i]));
        GVU[v].push_back(make_pair(u_arr[i], c_arr[i]));
    }
    // Topological sorting
    // 删掉所有入度/出度为0的点
    queue<int> q;
    for (i = 1; i <= node_size; i++)
    {
        if (!in_degree[i] && out_degree[i])
            q.push(i);
    }
    while (!q.empty())
    {
        u = q.front();
        q.pop();
        for (j = 0; j < GUV[u].size(); j++)
        {
            v = node_hashmap[GUV[u][j].first];
            --in_degree[v];
            if (!in_degree[v] && out_degree[v])
                q.push(v);
        }
    }
    for (i = 1; i <= node_size; i++)
    {
        if (!out_degree[i] && in_degree[i])
            q.push(i);
    }
    while (!q.empty())
    {
        u = q.front();
        q.pop();
        for (j = 0; j < GVU[u].size(); j++)
        {
            v = node_hashmap[GVU[u][j].first];
            --out_degree[v];
            if (!out_degree[v] && in_degree[v])
                q.push(v);
        }
    }
#ifdef TEST
    int cnt = 0;
    for (i = 1; i <= node_size; i++)
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
inline bool cmp(ans_t &x, ans_t &y)
{
    int now = 0;
    while (x[now] == y[now])
        ++now;
    return x[now] < y[now];
}

void flag_reverse_dfs(int u, int depth, int target, float nxtc)
{
    // 标记倒走4步以内能到达的点
    if (depth <= 4)
    {
        register int i;
        int v;
        float nowc;
        for (i = 0; i < GVU[u].size(); i++)
        {
            v = node_hashmap[GVU[u][i].first];
            nowc = GVU[u][i].second;
            if (nxtc < 0.2 * nowc || nxtc > 3.0 * nowc)
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
    // pre--prec-->u--nowc-->v
    register int i;
    int v;
    float nowc;
    for (i = 0; i < GUV[u].size(); i++)
    {
        if (GUV[u][i].first <= target)
            continue;
        nowc = GUV[u][i].second;
        if (nowc < 0.2 * prec || nowc > 3.0 * prec)
            continue;
        v = node_hashmap[GUV[u][i].first];
        if (is_end[v] && visited[v] == 0)
        {
            if (c_prenode_to_node[v] >= 0.2 * nowc && c_prenode_to_node[v] <= 3.0 * nowc)
            {
                path[0] = depth + 1;
                path[depth + 1] = GUV[u][i].first;
                ans[++ans_size] = path;
            }
        }
        if (flag[v] != target && !is_end[v] && depth >= 4)
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

inline void iter_st_from_node(int u, int target)
{
    // pre--prec-->u--nowc-->nxt
    ans_t path;
    register int i, j;
    float prec, nowc;
    int pre, nxt;
    for (i = 0; i < GUV[u].size(); i++)
    {
        if (GUV[u][i].first < target || GVU[u].size() == 0)
            continue;
        nowc = GUV[u][i].second;
        memset(is_end, 0, node_size + 5);
        for (j = 0; j < GVU[u].size(); j++)
        {
            if (GVU[u][j].first < target)
                continue;
            prec = GVU[u][j].second;
            pre = node_hashmap[GVU[u][j].first];
            if (nowc >= 0.2 * prec && nowc <= 3.0 * prec)
            {
                c_prenode_to_node[pre] = prec;
                is_end[pre] = 1;
                visited[pre] = 1;
                flag_reverse_dfs(pre, 2, target, prec);
                visited[pre] = 0;
            }
        }
        path[1] = target;
        path[2] = GUV[u][i].first;
        nxt = node_hashmap[GUV[u][i].first];
        visited[nxt] = 1;
        dfs(nxt, 2, path, target, nowc);
        visited[nxt] = 0;
    }
}
inline void work()
{
    int target;
    register int i, j;
    for (i = 1; i <= node_size; i++)
    {
        if (!in_degree[i] || !out_degree[i])
        {
            continue;
        }
        target = node[i];
        iter_st_from_node(i, target);
    }
}
inline void output_data()
{
    register int i, j;
    freopen(output_path.c_str(), "w", stdout);
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
int main()
{
#ifdef TEST
    cout << "Now running on data " + test_scale << endl;
#endif
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