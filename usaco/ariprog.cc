/*
ID: rain2wh1
PROB: ariprog
LANG: C++11
 */
#include<fstream>
#include<iostream>
#include<bitset>
#include<utility>
#include<algorithm>
#include<vector>

using namespace std;

void print_result();

class BisqureSet {
    private:
        int m_;
        bitset<2 * 250 * 250 + 1> s;

    public:
        BisqureSet(int m): m_(m) {}
        ~BisqureSet() {}

        void init()
        {
            for (int i = 0; i <= m_; ++i) {
                for (int j = 0; j <= m_; ++j) {
                    s.set(i * i + j * j);
                }
            }
        }

        bool find(int key) {return s.test(key);}
};


int main()
{
    ofstream fout("ariprog.out");
    ifstream fin("ariprog.in");
    int n; // length of arithmetic progression
    int m; // upper bound to the bisquares with 0 <= p, q <= m
    fin >> n >> m;
    
    // a + max_b * (n - 1) = upper_bound
    int upper_bound = 2 * m * m;
    int max_b = upper_bound / (n - 1);
    vector< pair<int, int> > result;

    BisqureSet bs(m);
    bs.init();
    
    bool once = true; // gurantee step is assigned once
    int step = 1;
    for (int b = 1; b <= max_b; b += step) {
        for (int a = 0; a <= (upper_bound-(n-1)*b); ++a) {
            bool found = true;
            for (int i = 0; i < n; ++i) {
                if (!bs.find(a + b * i)) {
                    found = false;
                    break;
                }
            }
            if (found == true) {
                if (once) {
                    // TODO: need to prove that
                    // found the minium step of b
                    step = b;
                }
                once = false;
                result.push_back(make_pair(a, b));
            }
        }
    }
    auto cmp = [](pair<int, int> key1, pair<int, int> key2) {
        if (key1.second < key2.second) {
            return true;
        } 
        if ((key1.second == key2.second) && (key1.first < key2.first)) {
            return true;
        }
        return false;
    };
    if (result.empty()) {
        fout << "NONE" << endl;
    }
    sort(result.begin(), result.end(), cmp);

    for (auto it = result.begin(); it != result.end(); ++it) {
        fout << it->first << " " << it->second << endl;
    }

    return 0;
}
