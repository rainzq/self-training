#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int search(int m, vector<int> &cache)
{
    if (cache[m]) {
        return cache[m];
    }
    if (m == 1) {
        cache[m] = 1;
        return 1;
    }
    if (m % 2) {
        cache[m] = search(3 * m + 1, cache) + 1;
    } else {
        cache[m] = search(m / 2, cache) + 1;
    }
    return cache[m];
}

int max_cycle_length(int m, int n, vector<int> &cache)
{
    int max_len = 0;
    for (int i = m; i <= n; ++i) {
        max_len = max(max_len, search(i, cache));
    }
    return max_len;
}


int main()
{
    int m, n;
    vector<int> cache(10000000, 0);
    while (cin >> m >> n) {
        int t;
        if (m < n) {
            t = max_cycle_length(m, n, cache);
        } else {
            t = max_cycle_length(n, m, cache);
        }
        cout << m << " " << n << " " << t << endl;
    }
}
