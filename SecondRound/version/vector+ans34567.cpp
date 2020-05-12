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
// 9 19
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
double c_prenode_to_node[MAX_EDGE];

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
int path[8];
int target;

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

static bool cmp(pair<int, int> a, pair<int, int> b)
{
    return a.first < b.first;
}
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
    for (i = 1; i <= node_size; i++)
    {
        if (GUV[i].size())
            sort(GUV[i].begin(), GUV[i].end(), cmp);
        if (GVU[i].size())
            sort(GVU[i].begin(), GVU[i].end(), cmp);
    }
#ifdef GUESSDATA
    // this_thread::sleep_for(chrono::milliseconds(node_size));   //node_size=29W
    int max_in_degree = 0, max_out_degree = 0;
    for (i = 1; i <= node_size; i++)
    {
        max_in_degree = max(in_degree[i], max_in_degree);
        max_out_degree = max(out_degree[i], max_out_degree);
    }
    this_thread::sleep_for(chrono::milliseconds(max_in_degree * 100));
#endif
    // Topological sorting
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

void flag_reverse_dfs(int u, int depth, double nxtc)
{
    // 标记倒走3步以内能到达的点
    if (depth <= 3)
    {
        register int i;
        int v;
        double nowc;
        double frac;
        for (i = 0; i < GVU[u].size(); i++)
        {
            v = node_hashmap[GVU[u][i].first];
            if (!in_degree[v] || !out_degree[v])
                continue;
            nowc = GVU[u][i].second;
            frac = nxtc / nowc;
            if (frac < 0.2 || frac > 3)
                continue;
            if (!visited[v] && GVU[u][i].first > target)
            {
                visited[v] = 1;
                flag[v] = target;
                flag_reverse_dfs(v, depth + 1, nowc);
                visited[v] = 0;
            }
        }
    }
}

void dfs(int u, int depth, double prec)
{
    // pre--prec-->u--nowc-->v
    register int i, j;
    int v;
    double nowc, frac;
    for (i = 0; i < GUV[u].size(); i++)
    {
        if (GUV[u][i].first <= target)
            continue;
        v = node_hashmap[GUV[u][i].first];
        if (!in_degree[v] || !out_degree[v])
            continue;
        nowc = GUV[u][i].second;
        frac = nowc / prec;
        if (frac < 0.2 || frac > 3)
            continue;
        if (is_end[v] && visited[v] == 0)
        {
            frac = c_prenode_to_node[v] / nowc;
            if (frac >= 0.2 && frac <= 3)
            {
                int len = depth + 1;
                path[len] = GUV[u][i].first;
                int *now_ans = ans[len - 3];
                ++*(now_ans);
                for (j = 1; j <= len; j++)
                    *(now_ans + len * (*now_ans) + j - 1) = path[j];
            }
        }
        if (flag[v] != target && !is_end[v] && depth >= 5)
            continue;
        if (!visited[v] && depth <= 5)
        {
            visited[v] = 1;
            path[depth + 1] = GUV[u][i].first;
            dfs(v, depth + 1, nowc);
            visited[v] = 0;
        }
    }
}

inline void iter_st_from_node(int u)
{
    // pre--prec-->u--nowc-->nxt
    register int i, j;
    double prec, nowc, frac;
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
            pre = node_hashmap[GVU[u][j].first];
            if (!in_degree[pre] || !out_degree[pre])
                continue;
            prec = GVU[u][j].second;
            frac = nowc / prec;
            if (frac >= 0.2 && frac <= 3)
            {
                c_prenode_to_node[pre] = prec;
                is_end[pre] = 1;
                visited[pre] = 1;
                flag_reverse_dfs(pre, 2, prec);
                visited[pre] = 0;
            }
        }
        path[1] = target;
        path[2] = GUV[u][i].first;
        nxt = node_hashmap[GUV[u][i].first];
        visited[nxt] = 1;
        dfs(nxt, 2, nowc);
        visited[nxt] = 0;
    }
}
inline void work()
{
    register int i, j;
    for (i = 0; i <= node_size; i++)
        flag[i] = -1;
    for (i = 1; i <= node_size; i++)
    {
        if (!in_degree[i] || !out_degree[i])
            continue;
        target = node[i];
        iter_st_from_node(i);
    }
}
inline void output_data()
{
    register int i, j, k;
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
    input_path = "../data/" + test_scale + "/test_data.txt";
    output_path = input_path.substr(0, input_path.rfind('/')) + "/vector+ans34567.txt";
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