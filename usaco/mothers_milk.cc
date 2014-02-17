/*
ID: rain2wh1
PROB: milk3
LANG: C++
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

#define N 21

struct Stat {
    vector<int> st; // the number of milk in A, B, C containers
    static bool flag[N][N]; // record the stat that we have been to, use the first and second stat as its index
    static Stat capacity;
    
    Stat(): st(3, 0) {};
    // pour milk from the t1 container to the t2 container
    Stat pour_from_to(int t1, int t2) {
        Stat result = *this;
        if (st[t1] > 0) {
            result.st[t1] = max(0, st[t1] + st[t2] - Stat::capacity.st[t2]);
            result.st[t2] = min(Stat::capacity.st[t2], st[t1] + st[t2]);
        }
        return result;
    }

    bool has_checked() {
        return flag[st[0]][st[1]];
    }
    
    void check() {
        flag[st[0]][st[1]] = true;
    }
};


Stat Stat::capacity;
bool Stat::flag[N][N] = {false};

int main()
{
    ofstream fout("milk3.out");
    ifstream fin("milk3.in");
    queue<Stat> q;
    vector<int> output; // store the results
    Stat init_s;

    fin >> Stat::capacity.st[0] >> Stat::capacity.st[1] >> Stat::capacity.st[2];
    init_s.st[2] = Stat::capacity.st[2]; // initial stat (0, 0, C)
    init_s.check();
    q.push(init_s);
    if (init_s.st[0] == 0) {
        output.push_back(init_s.st[2]);
    }
    while (!q.empty()) {
        Stat s = q.front();
        q.pop();
        Stat trans_s;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (i != j) {
                    trans_s = s.pour_from_to(i, j);
                    if (!trans_s.has_checked()) {
                        trans_s.check();
                        q.push(trans_s);
                        if (trans_s.st[0] == 0) {
                            output.push_back(trans_s.st[2]);
                        }
                    }
                }
            }
        }
    }
    sort(output.begin(), output.end());
    vector<int>::iterator it = output.begin();
    fout << *it;
    ++it;
    for (; it != output.end(); ++it) {
        fout << " " << *it;
    }
    fout << endl;
}
