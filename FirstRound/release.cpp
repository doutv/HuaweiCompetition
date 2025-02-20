#include <iostream>
#include <algorithm>
#include <string>
#include <cstring>
#include <array>
#include <cmath>
using namespace std;

string input_path = "/data/test_data.txt";
string output_path = "/projects/student/result.txt";

const int INF = 50001;
typedef long long ll;

int GUV[INF][51];
int GVU[INF][51];

bool visited[INF];
int flag[INF];

typedef array<int, 8> ans_t;
int ans_size;
ans_t ans[3000005];

int u_max;
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
    int u, v;
    int ch;
    register int i;
    while (1)
    {
        u = IO::rd();
        if (u == EOF)
            break;
        v = IO::rd();
        IO::rd_to_line_end();
        if (u >= INF || v >= INF)
            continue;
        GUV[u][++GUV[u][0]] = v;
        GVU[v][++GVU[v][0]] = u;
        u_max = max(u_max, u);
    }
}
inline bool cmp(ans_t &x, ans_t &y)
{
    int now = 0;
    while (x[now] == y[now])
        ++now;
    return x[now] < y[now];
}
void flag_reverse_dfs(int u, int depth, int target)
{
    register int i;
    int v;
    for (i = 1; i <= GVU[u][0]; i++)
    {
        v = GVU[u][i];
        if (!visited[v] && v > target)
        {
            visited[v] = 1;
            flag[v] = target;
            if (depth <= 2)
                flag_reverse_dfs(v, depth + 1, target);
            visited[v] = 0;
        }
    }
}
void dfs(int u, int depth, ans_t &path, int target)
{
    register int i;
    int v;
    for (i = 1; i <= GUV[u][0]; i++)
    {
        v = GUV[u][i];
        if (v <= target)
            continue;
        if (flag[v] == -target && visited[v] == 0)
        {
            if (depth >= 2)
            {
                path[0] = depth + 1;
                path[depth + 1] = v;
                ans[++ans_size] = path;
            }
        }
        if (flag[v] != target && flag[v] != -target && depth >= 4)
            continue;
        if (!visited[v] && depth <= 5)
        {
            visited[v] = 1;
            path[depth + 1] = v;
            dfs(v, depth + 1, path, target);
            visited[v] = 0;
        }
    }
}
inline void work()
{
    ans_t path;
    register int i, j;
    int target;
    for (i = 1; i <= u_max; i++)
    {
        if (GUV[i][0] == 0)
            continue;
        flag_reverse_dfs(i, 1, i);
        for (j = 1; j <= GVU[i][0]; j++)
            flag[GVU[i][j]] = -i;
        path[1] = i;
        dfs(i, 1, path, i);
    }
}
inline void output_data()
{
    register int i, j;
    freopen(output_path.c_str(), "w", stdout);
    sort(ans + 1, ans + ans_size + 1, cmp);
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
    return;
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