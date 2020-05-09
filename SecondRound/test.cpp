#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <unordered_map>

using namespace std;

const int ANS3_MAX = 1000005;
const int ANS4_MAX = 1000005;
const int ANS5_MAX = 1000005;
const int ANS6_MAX = 1000005;
const int ANS7_MAX = 1000005;
int ans_size;
int ans3[ANS3_MAX * 3];
int ans4[ANS4_MAX * 4];
int ans5[ANS5_MAX * 5];
int ans6[ANS6_MAX * 6];
int ans7[ANS7_MAX * 7];
int *ans[5] = {ans3, ans4, ans5, ans6, ans7};

int main()
{
    int path[4] = {3, 10, 100, 1000};
    int len = path[0];
    int *now_ans = ans[len - 3];
    ++*(now_ans);
    for (int i = 1; i <= len; i++)
        *(now_ans + len * (*now_ans) + i - 1) = path[i];
    for (int i = 1; i <= len; i++)
        cout << *(now_ans + len * (*now_ans) + i - 1) << endl;
    return 0;
}