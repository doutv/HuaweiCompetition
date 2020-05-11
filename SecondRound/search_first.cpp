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
string test_scale = "697518";
string input_path = "./data/" + test_scale + "/test_data.txt";
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
int flag[MAX_EDGE][2];
int node_size;
int node[MAX_EDGE * 2];
unordered_map<int, int> node_hashmap;

const int ANS_MAX = 10000005;
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
        int v;
        for (i = 0; i < GVU[u].size(); i++)
        {
            v = node_hashmap[GVU[u][i].first];
            if (!visited[v] && GVU[u][i].first > target)
            {
                visited[v] = 1;
                flag[v][0] = target + 1;
                flag_reverse_dfs(v, depth + 1, target);
                visited[v] = 0;
            }
        }
    }
}
void dfs(int u, int depth, ans_t &path, int target)
{
    register int i, j;
    int v;
    for (i = 0; i < GUV[u].size(); i++)
    {
        v = node_hashmap[GUV[u][i].first];
        if (GUV[u][i].first <= target)
            continue;
        if (flag[v][0] == -2 && visited[v] == 0)
        {
            if (depth >= 2)
            {
                int len = depth + 1;
                path[depth] = GUV[u][i].first;
                money[depth-1] = GUV[u][i].second;
                money[depth] = flag[v][1];
                bool status = 1;
                int k, m;
                for (k = 0; k <= depth; k++) {
                    if (k == depth) m = 0;
                    else m = k + 1;
                    if (double(money[m]) / money[k] < 0.2
                            || double(money[m]) / money[k] > 3) {
                        status = 0;
                        break;
                    }
                }
                if (status) {
                    ans[len-3][++ans_size[len-3]] = path; 
                    // for (int &i: path) cout << i << ' ';
                    // cout << endl;
                }
            }
        }
        if (flag[v][0] != (target + 1) && flag[v][0] != -2 && depth >= 4)
            continue;
        if (!visited[v] && depth <= 5)
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
    memset(flag, -1, node_size + 5);
    ans_t path;
    int v, target;
    register int i, j;
    for (i = 1; i <= node_size; i++)
    {
        target = node[i];
        flag_reverse_dfs(i, 1, target);
        for (j = 0; j < GVU[i].size(); j++)
        {
            v = node_hashmap[GVU[i][j].first];
            flag[v][0] = -2;
            flag[v][1] = GVU[i][j].second;
        }
        path[0] = target;
        dfs(i, 1, path, target);
        for (j = 0; j < GVU[i].size(); j++)
        {
            v = node_hashmap[GVU[i][j].first];
            flag[v][0] = -1;
        }
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
        sort(ans[i] + 1, ans[i] + ans_size[i] + 1, cmp);
        for (j = 1; j <= ans_size[i]; j++)
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
    printf("ans size is %d\n", ans_size[0]+ans_size[1]+ans_size[2]+ans_size[3]+ans_size[4]);
    cout << "The program's speed: " << chrono::duration<double, milli>(diff).count() / 1000 << "s" << endl;
#endif
    fclose(stdin);
    fclose(stdout);
    return 0;
}