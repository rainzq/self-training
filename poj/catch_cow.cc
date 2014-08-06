#include <iostream>
#include <queue>
#include <vector>

using namespace std;

int search(int m, int n)
{
    if (m == n) {
        return 0;
    }
    vector< queue<int> > vq(2); // double buffer for store each level nodes
    vector<int> mmap(200001, 0);
    int level = 0;
    int index = 0;

    mmap[m] = 1;
    vq[index].push(m);
    int visited_node = m;
    int next = 0;
    int index1 = 1;
    while (1) {
        ++level;
        while (!vq[index].empty()) {
            // pop queue
            visited_node = vq[index].front();
            vq[index].pop();

            // push another queue
            index1 = (index + 1) % 2;
            for (int i = 0; i < 3; ++i) {
                if (i == 0) {
                    next = visited_node + 1;
                } else if (i == 1) {
                    next = visited_node - 1;
                } else if (i == 2) {
                    next = visited_node * 2;
                }

                if (next == n) {
                    // cout << visited_node << endl;
                    return level;
                }
                if (next > 0 && next <= 200000 && mmap[next] == 0) {
                    // cout << next << "\t";
                    mmap[next] = level;
                    vq[index1].push(next);
                }
            }
        }
        // cout << endl;

        index ^= 0x1;
    }
    
}

int main()
{
    int m, n;
    while (cin >> m >> n) {
        cout << search(m, n) << endl;
    }
}
