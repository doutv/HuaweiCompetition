#include <iostream>

using namespace std;

const int count = 32;

int main()
{
    int u, v, w;
    freopen("./SecondRound/test_data", "r", stdin);
    while (scanf("%d,%d,%d\n", &u, &v, &w) != EOF) {
        cout << u << '\t' << v << '\t' << w << endl;
    }
    system("pause");
    return 0;
}