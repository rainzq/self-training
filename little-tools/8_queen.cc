#include <stdlib.h>
#include <iostream>

using namespace std;

int *result;
int n; // n queens

void search(int i);
bool valid(int i, int v);
void print(int *result);

int main()
{
    cin >> n;
    result = new int[n];
    if (result == NULL) {
        cerr << "out of memory" << endl;
        return -1;
    }
    search(0);
    delete[] result;
    return 0;
}

void search(int i) 
{
    for (int t = 0; t < n; t++) {
        if (valid(i, t)) { // if t is a valid value in position i
            result[i] = t; // it will be overwritten later
            if (i == n - 1) { // found a solution
                print(result);
            } else { // partial solution
                search(i + 1);
            }
        }
    }
}

bool valid(int i, int v)
{
    for (int j = i - 1; j >= 0; --j) {
        if (v == result[j]) { // the same column
            return false;
        }
        if (abs(v - result[j]) == (i - j)) { // cross 
            return false;
        }
    }
    return true;
}

void print(int *result)
{
    for (int i = 0; i < n; ++i) {
        cout << result[i] << " ";
    }
    cout << endl;
}
