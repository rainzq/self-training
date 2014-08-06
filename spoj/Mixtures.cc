#include <vector>
#include <utility>
#include <iostream>
#include <algorithm>
#include <limits>

using namespace std;

pair<long long, int> minimum_amount(int m, int n, 
                                          const vector<int> &data, 
                                          vector< vector < pair<long long, int> > > &cache)
{
    if (cache[m][n] != make_pair<long long, int>(0, 0)) {
        return cache[m][n];
    }
    if (m == n) {
        cache[m][n] = make_pair(static_cast<long long>(0), data[m]);
        return cache[m][n];
    }
    long long smoke = numeric_limits<long long>::max();
    int mix = 0;
    for (int k = m; k < n; ++k) {
        pair<long long, int> t1 = minimum_amount(m, k, data, cache);
        pair<long long, int> t2 = minimum_amount(k + 1, n, data, cache);
        long long ts = t1.first + t2.first + t1.second * t2.second;
        if (smoke > ts) {
            smoke = ts;
            mix = (t1.second + t2.second) % 100;
        }
    }
    cache[m][n] = make_pair(smoke, mix);
    return cache[m][n];
}

int main()
{
    int n;
    while (cin >> n) {
        int t;
        vector<int> data;
        vector< vector <pair<long long, int> > > cache(n, vector< pair<long long, int> >(n, make_pair<long long, int>(0, 0)));
        for (int i = 0; i < n; ++i) {
            cin >> t;
            data.push_back(t);
        }
        cout << minimum_amount(0, n - 1, data, cache).first << endl;
    }
}
