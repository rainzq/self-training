#include <iostream>

using namespace std;

int main()
{
    int t;
    while (cin >> t) {
        if (t == 42) {
            return 0;
        }
        cout << t << endl;
    }
    return 0;
}
