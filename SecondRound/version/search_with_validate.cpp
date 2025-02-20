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
// 4 5 161
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
int node_size;
int node[MAX_EDGE * 2];
unordered_map<int, int> node_hashmap;
double c_prenode_to_node[MAX_EDGE];

unordered_map<int, vector<pair<int, double>>> bag2;

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
    {
        return GUV[u].size();
    }
    return l;
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
        for (i = get_GVU_lower_bound(u); i < GVU[u].size(); i++)
        {
            v = node_hashmap[GVU[u][i].first];
            if (!in_degree[v] || !out_degree[v])
                continue;
            nowc = GVU[u][i].second;
            frac = nxtc / nowc;
            if (frac < 0.2 || frac > 3)
                continue;
            if (!visited[v])
            {
                if (depth == 2)
                {
                    if (bag2.find(v) == bag2.end())
                    {
                        bag2.insert({v, vector<pair<int, double>>{make_pair(node[u], nowc)}});
                    }
                    else
                    {
                        bag2[v].push_back(make_pair(node[u], nowc));
                    }
                }
                if (depth == 3)
                {
                    flag[v] = target;
                }
                visited[v] = 1;
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
    int v, len, k;
    double nowc, frac;
    if (depth == 2)
    {
        if (bag2.find(u) != bag2.end())
        {
            for (i = 0; i < bag2[u].size(); i++)
            {
                nowc = bag2[u][i].second;
                frac = nowc / prec;
                if (frac < 0.2 || frac > 3)
                {
                    continue;
                }
                len = depth + 1;
                path[len] = bag2[u][i].first;
                int *now_ans = ans[len - 3];
                ++*(now_ans);
                for (j = 1; j <= len; j++)
                {
                    // if (now_ans + len * (*now_ans) + j - 1 == &target) {
                    //     cout << "target modified" << endl;
                    // }
                    *(now_ans + len * (*now_ans) + j - 1) = path[j];
                }
            }
        }
    }
    for (i = get_GUV_lower_bound(u); i < GUV[u].size(); i++)
    {
        v = node_hashmap[GUV[u][i].first];
        if (!in_degree[v] || !out_degree[v])
            continue;
        nowc = GUV[u][i].second;
        frac = nowc / prec;
        if (frac < 0.2 || frac > 3)
            continue;
        if (bag2.find(v) != bag2.end() && visited[v] == 0 && depth <= 5)
        {
            for (j = 0; j < bag2[v].size(); j++)
            {
                frac = bag2[v][j].second / nowc;
                int last = bag2[v][j].first;
                if (frac < 0.2 || frac > 3 || visited[node_hashmap[last]])
                {
                    continue;
                }
                path[depth + 1] = node[v];
                len = depth + 2;
                path[len] = last;
                int *now_ans = ans[len - 3];
                ++*(now_ans);
                for (k = 1; k <= len; k++)
                {
                    // if (now_ans + len * (*now_ans) + j - 1 == &target) {
                    //     cout << "target modified" << endl;
                    // }
                    *(now_ans + len * (*now_ans) + k - 1) = path[k];
                }
            }
        }
        if ((flag[v] != target && depth >= 4) || depth > 5)
            continue;
        if (!visited[v] && (depth <= 3 || flag[v] == target))
        {
            visited[v] = 1;
            path[depth + 1] = GUV[u][i].first;
            // if (target != 2534) {
            //     cout << "u: " << u << "; depth: " << depth << endl;
            // }
            dfs(v, depth + 1, nowc);
            visited[v] = 0;
        }
    }
}

inline void work()
{
    register int i, j, v;
    double nowc;
    // cout << 137 << "--" << node_hashmap[137] << endl;
    // cout << 148 << "--" << node_hashmap[148] << endl;
    // cout << 153 << "--" << node_hashmap[153] << endl;
    // cout << 182 << "--" << node_hashmap[182] << endl;
    for (i = 0; i <= node_size; i++)
        flag[i] = -1;
    for (i = 1; i <= node_size; i++)
    {
        if (!in_degree[i] || !out_degree[i])
            continue;
        target = node[i];
        path[1] = target;
        for (j = get_GUV_lower_bound(i); j < GUV[i].size(); j++)
        {
            v = node_hashmap[GUV[i][j].first];
            if (!in_degree[v] || !out_degree[v])
            {
                continue;
            }
            nowc = GUV[i][j].second;
            bag2.clear();
            path[2] = node[v];
            flag_reverse_dfs(i, 1, nowc);
            visited[v] = 1;
            dfs(v, 2, nowc);
            visited[v] = 0;
        }
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
    if (argc == 2)
    {
        test_scale = argv[1];
    }
    else
        test_scale = "9153";
    input_path = "../data/" + test_scale + "/test_data.txt";
    output_path = input_path.substr(0, input_path.rfind('/')) + "search_with_validate.txt";
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