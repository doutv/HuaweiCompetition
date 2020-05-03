#include <iostream>

using namespace std;

const int count = 32;

int main()
{
    long long a = 1;
    unsigned long long  b = (a << count) + a;
    int c = b >> count;
    int d = a;
    cout << a << endl;
    cout << b << endl;
    cout << c << endl;
    cout << sizeof(int) << endl;
    cout << sizeof(long) << endl;
    cout << sizeof(unsigned long long) << endl;
    system("pause");
    return 0;
}