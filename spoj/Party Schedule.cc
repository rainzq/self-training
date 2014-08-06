#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <assert.h>

using namespace std;

// data  : store party entrance fees and its fun value as the first and the second element of pair
// cache : store the intermediate value, pair<int, int> is fees used  and fun value got
// return pair<int, int> : fee used and fun value pair
pair<int, int> compute(int m, int n, const vector< pair<int, int> > &data,  vector< vector< pair<int, int> > > &cache)
{
    if (m == 0 || n == 0) {
        cache[m][n] = make_pair(0, 0);
        return cache[m][n];
    }
    if (cache[m][n] != make_pair(0, 0)) {
        return cache[m][n];
    }
    pair<int, int> t2 = compute(m, n - 1, data, cache);
    if (m >= data[n-1].first) {
        pair<int, int> t1 = compute(m - data[n-1].first, n - 1, data, cache);
        t1.first += data[n-1].first;
        t1.second += data[n-1].second;
        if ((t1.second > t2.second) || (t1.second == t2.second && t1.first < t2.first)) {
            cache[m][n] = t1;
        } else {
            cache[m][n] = t2;
        }
    } else {
        cache[m][n] = t2;
    }
    return cache[m][n];
}

int main()
{
    int m, n;
    int fee, value;
    while (cin >> m >> n) {
        if (m == 0 && n == 0) {
            return 0;
        }
        vector< pair<int, int> > data;
        vector< vector< pair<int, int> > > cache(m + 1, vector< pair<int, int> >(n + 1, make_pair(0, 0)));
        for (int i = 0; i < n; ++i) {
            cin >> fee >> value;
            data.push_back(make_pair(fee, value));
        }
        sort(data.begin(), data.end());
        pair<int, int> value = compute(m, n, data, cache);
        cout << value.first << " " << value.second << endl;
    }
    return 0;
}
