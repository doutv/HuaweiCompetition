#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cstring>
#include <array>
#include <cmath>
#include <unordered_map>
using namespace std;

// #define LINUXOUTPUT
// #define OUTPUT
// #define TEST

#ifdef TEST
#include <chrono>
auto time_start = chrono::steady_clock::now();
string test_scale = "std";
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

const int ANS3_MAX = 10000005;
const int ANS4_MAX = 10000005;
const int ANS5_MAX = 10000005;
const int ANS6_MAX = 10000005;
const int ANS7_MAX = 10000005;
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

void flag_reverse_dfs(int u, int depth, int target, float nxtc)
{
    // 标记倒走4步以内能到达的点
    if (depth <= 4)
    {
        register int i;
        int v;
        float nowc;
        float frac;
        for (i = 0; i < GVU[u].size(); i++)
        {
            v = node_hashmap[GVU[u][i].first];
            nowc = GVU[u][i].second;
            frac = nxtc / nowc;
            if (frac < 0.2 || frac > 3.0)
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
    register int i, j;
    int v;
    float nowc, frac;
    for (i = 0; i < GUV[u].size(); i++)
    {
        if (GUV[u][i].first <= target)
            continue;
        nowc = GUV[u][i].second;
        frac = nowc / prec;
        if (frac < 0.2 || frac > 3.0)
            continue;
        v = node_hashmap[GUV[u][i].first];
        if (is_end[v] && visited[v] == 0)
        {
            frac = c_prenode_to_node[v] / nowc;
            if (frac >= 0.2 && frac <= 3.0)
            {
                int len = depth + 1;
                path[len] = GUV[u][i].first;
                int *now_ans = ans[len - 3];
                ++*(now_ans);
                for (j = 1; j <= len; j++)
                    *(now_ans + len * (*now_ans) + j - 1) = path[j];
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
    float prec, nowc, frac;
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
            frac = nowc / prec;
            if (frac >= 0.2 && frac <= 3.0)
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
        target = node[i];
        iter_st_from_node(i, target);
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