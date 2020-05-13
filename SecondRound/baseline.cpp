#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cstring>
#include <array>
#include <cmath>
#include <unordered_map>
using namespace std;

#define LINUXOUTPUT
#define OUTPUT
#define TEST

#ifdef TEST
#include <chrono>
auto time_start = chrono::steady_clock::now();
string test_scale, input_path, output_path;
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
int searched[MAX_EDGE][2];
int flag[MAX_EDGE][2]; // 第一个记录终点，第二个记录trace中的有效点数
vector<pair<int, int>> trace[MAX_EDGE];
int node_size;
int node[MAX_EDGE * 2];
unordered_map<int, int> node_hashmap;

const int ANS_MAX = 2000000;
int ans_size[5];
ans_t ans[5][ANS_MAX];

int u_arr[MAX_EDGE];
int v_arr[MAX_EDGE];
int c_arr[MAX_EDGE];

int money[7];

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
void flag_reverse_dfs(int u, int depth, int target)
{
    if (depth <= 3)
    {
        register int i;
        int v, v_id;
        for (i = 0; i < GVU[u].size(); i++)
        {
            v_id = GVU[u][i].first;
            v = node_hashmap[v_id];
            if (!visited[v] && v_id > target)
            {
                visited[v] = 1;
                if (flag[v][0] != target + 1) {
                    flag[v][1] = 0;
                    flag[v][0] = target + 1;
                }
                flag[v][1] += 1;
                if (flag[v][1] > trace[v].size())
                trace[v].push_back(make_pair(node[u], GVU[u][i].second));
                else trace[v][flag[v][1]-1] = make_pair(node[u], GVU[u][i].second);
                flag_reverse_dfs(v, depth + 1, target);
                visited[v] = 0;
            }
        }
    }
}
inline void add_ans(int v_id, int depth, ans_t &path, int target, int o_depth)
{
    int v = node_hashmap[v_id];
    if (flag[v][0] == (target + 1)) {
        for (int i = 0; i < flag[v][1]; i++) {
            money[depth] = trace[v][i].second;
            path[depth] = v_id;
            int u_id = trace[v][i].first;
            if (depth == o_depth) {
                int u = node_hashmap[u_id];
                searched[u][0] = v;
                searched[u][1] = target;
            }
            add_ans(u_id, depth + 1, path, target, o_depth);
        }
    } else {
        bool status = 1;
        int k, m;
        for (k = 0; k < depth; k++) {
            if (k == depth - 1) m = 0;
            else m = k + 1;
            if (double(money[m]) / money[k] < 0.2
                    || double(money[m]) / money[k] > 3) {
                status = 0;
                break;
            }
        }
        if (status) {
            ans[depth-3][ans_size[depth-3]++] = path; 
            // for (int &i: path) cout << i << ' ';
            // cout << endl;
        }
    }
}
void dfs(int u, int depth, ans_t &path, int target)
{
    register int i, j;
    int v, v_id;
    for (i = 0; i < GUV[u].size(); i++)
    {
        v_id = GUV[u][i].first;
        v = node_hashmap[v_id];
        if (v_id <= target || (searched[v][0] == u && searched[v][1] == target))
            continue;
        if (flag[v][0] == (target + 1) && visited[v] == 0)
        {
            if (depth >= 1)
            {
                money[depth - 1] = GUV[u][i].second;
                add_ans(v_id, depth, path, target, depth);
            }
        }
        if (flag[v][0] != (target + 1) && depth >= 4)
            continue;
        if (!visited[v] && depth <= 3)
        {
            visited[v] = 1;
            path[depth] = GUV[u][i].first;
            money[depth-1] = GUV[u][i].second;
            dfs(v, depth + 1, path, target);
            visited[v] = 0;
        }
    }
}
inline void work()
{
    ans_t path;
    int v, target;
    register int i, j;
    for (i = 1; i <= node_size; i++)
    {
        // cout << i << endl;
        target = node[i];
        flag_reverse_dfs(i, 1, target);
        path[0] = target;
        dfs(i, 1, path, target);
    }
}
inline bool cmp(ans_t &x, ans_t &y)
{
    int now = 0;
    while (x[now] == y[now])
        ++now;
    return x[now] < y[now];
}
inline void output_data()
{
    register int i, j, k, an_size;
    freopen(output_path.c_str(), "w", stdout);
#ifdef TEST
    auto output_time_start = chrono::steady_clock::now();
#endif
    an_size = ans_size[0]+ans_size[1]+ans_size[2]+ans_size[3]+ans_size[4];
    printf("%d\n", an_size);
    for (i = 0; i <= 4; i++)
    {
        sort(ans[i], ans[i] + ans_size[i], cmp);
        for (j = 0; j < ans_size[i]; j++)
        {
            for (k = 0; k < i + 2; k++)
            {
                IO::write(ans[i][j][k]);
                IO::push(',');
            }
            IO::write(ans[i][j][i+2]);
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
int main(int argc, char ** argv)
{
#ifdef TEST
    if (argc == 2) {
        test_scale = argv[1];
    }
    input_path = "./data/" + test_scale + "/test_data.txt";
    output_path = input_path.substr(0, input_path.rfind('/')) + "/output.txt";
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
    printf("ans size is %d\n", ans_size[0]+ans_size[1]+ans_size[2]+ans_size[3]+ans_size[4]);
    cout << "The program's speed: " << chrono::duration<double, milli>(diff).count() / 1000 << "s" << endl;
#endif
    fclose(stdin);
    fclose(stdout);
    return 0;
}