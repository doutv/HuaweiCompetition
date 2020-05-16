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

string input_path = "/data/test_data.txt";
string output_path = "/projects/student/result.txt";

typedef long long ll;
typedef pair<int, double> edge_t;

const int MAX_EDGE = 2000005;
vector<edge_t> GUV[MAX_EDGE];
vector<edge_t> GVU[MAX_EDGE];
int edge_size;

bool visited[MAX_EDGE];
int node_size;
int node[MAX_EDGE * 2];
double v1_to_u[MAX_EDGE];
int path[9];
double money;
unordered_map<int, int> node_hashmap;
unordered_map<int, vector<edge_t>> bag2;
bool bag3[MAX_EDGE];

const int ANS3_MAX = 20000005;
const int ANS4_MAX = 20000005;
const int ANS5_MAX = 20000005;
const int ANS6_MAX = 20000005;
const int ANS7_MAX = 20000005;
const int ANS8_MAX = 20000005;

int ans_size;
int ans3[ANS3_MAX * 3];
int ans4[ANS4_MAX * 4];
int ans5[ANS5_MAX * 5];
int ans6[ANS6_MAX * 6];
int ans7[ANS7_MAX * 7];
int ans8[ANS8_MAX * 8];
int *ans[6] = {ans3, ans4, ans5, ans6, ans7, ans8};

int u_arr[MAX_EDGE];
int v_arr[MAX_EDGE];
double c_arr[MAX_EDGE];

int in_degree[MAX_EDGE * 2];
int out_degree[MAX_EDGE * 2];

namespace IO
{
    const int MAXSIZE = 1 << 20;
    char buf[MAXSIZE], *p1, *p2;
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
    int u, v;
    int ch;
    double c;
    while (scanf("%d,%d,%lf", &u, &v, &c) != EOF)
    {
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
void flag_reverse_dfs(int u, int depth, double prec)
{
    for (int i = get_GVU_lower_bound(u); i < GVU[u].size(); i++)
    {
        int v = node_hashmap[GVU[u][i].first];
        if (!in_degree[v] || !out_degree[v])
            continue;
        if (depth >= 2)
        {
            double frac = prec / GVU[u][i].second;
            if (frac < 0.2 || frac > 3.0)
                continue;
        }
        if (depth == 1)
            v1_to_u[v] = GVU[u][i].second;
        else if (depth == 2)
        {
            if (bag2.find(v) == bag2.end())
                bag2[v] = vector<edge_t>{make_pair(node[u], GVU[u][i].second)};
            else
                bag2[v].push_back(make_pair(node[u], GVU[u][i].second));
        }
        else if (depth == 3)
        {
            bag3[v] = 1;
        }
        if (depth <= 2)
        {
            flag_reverse_dfs(v, depth + 1, GVU[u][i].second);
        }
    }
}
void dfs(int u, int depth, double prec)
{
    for (int i = get_GUV_lower_bound(u); i < GUV[u].size(); i++)
    {
        int v = node_hashmap[GUV[u][i].first];
        if (!in_degree[v] || !out_degree[v])
            continue;
        if (depth == 1)
        {
            money = GUV[u][i].second;
        }
        else if (depth >= 2)
        {
            double frac = GUV[u][i].second / prec;
            if (frac < 0.2 || frac > 3.0)
                continue;
        }
        if (!visited[v] && bag2.find(v) != bag2.end())
        {
            path[depth + 1] = GUV[u][i].first;
            for (int j = 0; j < bag2[v].size(); j++)
            {
                if (visited[node_hashmap[bag2[v][j].first]])
                    continue;
                int len = depth + 2;
                path[depth + 2] = bag2[v][j].first;
                double frac = money / v1_to_u[node_hashmap[bag2[v][j].first]];
                if (frac < 0.2 || frac > 3.0)
                {
                    continue;
                }
                frac = bag2[v][j].second / GUV[u][i].second;
                if (frac < 0.2 || frac > 3.0)
                {
                    continue;
                }
                int *now_ans = ans[len - 3];
                ++*(now_ans);
                for (int k = 1; k <= len; k++)
                    *(now_ans + len * (*now_ans) + k - 1) = path[k];
            }
        }
        if (!bag3[v] && depth >= 5)
            continue;
        if (!visited[v] && depth <= 5)
        {
            visited[v] = 1;
            path[depth + 1] = GUV[u][i].first;
            dfs(v, depth + 1, GUV[u][i].second);
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
        visited[i] = 1;
        double tmp = 0.0;
        flag_reverse_dfs(i, 1, tmp);
        path[1] = target;
        dfs(i, 1, tmp);
        visited[i] = 0;
    }
}

inline void output_data()
{
    int i, j, k;
    freopen(output_path.c_str(), "w", stdout);
    ans_size = *(ans3) + *(ans4) + *(ans5) + *(ans6) + *(ans7) + *(ans8);
    printf("%d\n", ans_size);
    for (i = 0; i <= 5; i++)
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
}
int main()
{
    read_data();
    work();
    output_data();
    fclose(stdin);
    fclose(stdout);
    return 0;
}