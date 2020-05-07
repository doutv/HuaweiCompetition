#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <cstring>
#include <array>
#include <cmath>
#include <unordered_map>
#include <ctime>

using namespace std;

#define TEST

typedef unsigned long long ull;
typedef unsigned int ui;

struct Path
{
    int length;
    vector<ui> path;

    Path(int length, const vector<ui> &path) : length(length), path(path) {}

    bool operator<(const Path &rhs) const
    {
        if (length != rhs.length)
            return length < rhs.length;
        for (int i = 0; i < length; i++)
        {
            if (path[i] != rhs.path[i])
                return path[i] < rhs.path[i];
        }
    }
};

class Solution
{
public:
    vector<vector<int>> G;
    unordered_map<ui, int> idHash; //sorted id to 0...n
    //bad case, don't follow
    unordered_map<ull, int> migic;
    vector<ui> ids;    //0...n to sorted id
    vector<ui> inputs; //u-v pairs
    vector<int> inDegrees;
    vector<bool> vis;
    vector<Path> ans;
    int nodeCnt;

    inline bool check(int x, int y)
    {
        if (x == 0 || y == 0)
            return false;
        return x <= 5ll * y && y <= 3ll * x;
    }

    void parseInput(string &testFile)
    {
        FILE *file = fopen(testFile.c_str(), "r");
        ui u, v, c;
        int cnt = 0;
        while (fscanf(file, "%u,%u,%u", &u, &v, &c) != EOF)
        {
            inputs.push_back(u);
            inputs.push_back(v);
            migic[(ull)u << 32 | v] = c;
            ++cnt;
        }
#ifdef TEST
        printf("%d Records in Total\n", cnt);
#endif
    }

    void constructGraph()
    {
        auto tmp = inputs;
        sort(tmp.begin(), tmp.end());
        tmp.erase(unique(tmp.begin(), tmp.end()), tmp.end());
        nodeCnt = tmp.size();
        ids = tmp;
        nodeCnt = 0;
        for (ui &x : tmp)
        {
            idHash[x] = nodeCnt++;
        }
#ifdef TEST
        printf("%d Nodes in Total\n", nodeCnt);
#endif
        int sz = inputs.size();
        G = vector<vector<int>>(nodeCnt);
        inDegrees = vector<int>(nodeCnt, 0);
        for (int i = 0; i < sz; i += 2)
        {
            int u = idHash[inputs[i]], v = idHash[inputs[i + 1]];
            G[u].push_back(v);
            ++inDegrees[v];
        }
    }

    //magic code,don't touch
    bool checkAns(vector<ui> tmp, int depth)
    {
        for (int i = 0; i < depth; i++)
        {
            int l = tmp[(i + depth - 1) % depth], m = tmp[i], r = tmp[(i + 1) % depth];
            if (!check(migic[(ull)l << 32 | m], migic[(ull)m << 32 | r]))
                return false;
        }
        return true;
    }

    void dfs(int head, int cur, int depth, vector<int> &path)
    {
        vis[cur] = true;
        path.push_back(cur);
        for (int &v : G[cur])
        {
            if (v == head && depth >= 3 && depth <= 7)
            {
                vector<ui> tmp;
                for (int &x : path)
                    tmp.push_back(ids[x]);
                if (checkAns(tmp, depth))
                    ans.emplace_back(Path(depth, tmp));
            }
            if (depth < 7 && !vis[v] && v > head)
            {
                dfs(head, v, depth + 1, path);
            }
        }
        vis[cur] = false;
        path.pop_back();
    }

    //search from 0...n
    //由于要求id最小的在前，因此搜索的全过程中不考虑比起点id更小的节点
    void solve()
    {
        vis = vector<bool>(nodeCnt, false);
        vector<int> path;
        for (int i = 0; i < nodeCnt; i++)
        {
            if (i % 100 == 0)
                cout << i << "/" << nodeCnt << endl;
            if (!G[i].empty())
            {
                dfs(i, i, 1, path);
            }
        }
        sort(ans.begin(), ans.end());
    }

    void save(string &outputFile)
    {
        printf("Total Loops %d\n", (int)ans.size());
        ofstream out(outputFile);
        out << ans.size() << endl;
        for (auto &x : ans)
        {
            auto path = x.path;
            int sz = path.size();
            out << path[0];
            for (int i = 1; i < sz; i++)
                out << "," << path[i];
            out << endl;
        }
    }
};

void solve(string testFile, string outputFile)
{
    Solution solution;
    solution.parseInput(testFile);
    solution.constructGraph();
    solution.solve();
    solution.save(outputFile);
}

int main()
{
    for (int i = 1; i <= 1; i++)
    {
        string test_scale = "639096";
        string input_path = "./data/" + test_scale + "/test_data.txt";
        string output_path = input_path.substr(0, input_path.rfind('/')) + "/result.txt";
        auto t = clock();
        solve(input_path, output_path);
        // cout << clock() - t << endl;
    }

    return 0;
}