#include <iostream>
#include <vector>
#include <utility>
#include <set>
#include <assert.h>

using namespace std;

vector< pair<int, int> > compute(const vector<int> &v, int i, int j, vector< vector< set<int> > > &cache)
{
    if (!cache[i][j].empty()) {
        return cache[i][j];
    }
    if (i == j) {
        cache[i][j].insert(v[i]);
        return cache[i][j];
    }
    for (int k = i; k < j; ++k) {
        auto t1 = compute(v, i, k, cache);
        auto t2 = compute(v, k+1, j, cache);
        for (auto it1 = t1.begin(); it1 != t1.end(); ++it1) {
            for (auto it2 = t2.begin(); it2 != t2.end(); ++it2) {
                cache[i][j].insert(*it1 - *it2);
            }
        }
    }
    return cache[i][j];
}

int main()
{
    vector< vector < set<int> > > cache(100, vector< set<int> >(100));

}
