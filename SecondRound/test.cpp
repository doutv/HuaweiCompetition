#include <iostream>

using namespace std;

const int count = 32;

int main()
{
    int ans2[2] = {1, 2};
    int ans3[2] = {12, 13};
    int *ans[2] = {ans2, ans3};
    int *nowans = ans[0];
    cout << ++*nowans << endl;
    return 0;
}