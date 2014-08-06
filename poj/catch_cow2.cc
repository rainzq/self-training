#include <iostream>
#include <queue>
#include <vector>

using namespace std;

int search(int m, int n)
{
    if (m == n) {
        return 0;
    }
    queue<int> vq;
    vector<int> mmap(200001, 0);

    mmap[m] = 1;
    vq.push(m);
    int visited_node = m;
    int next = 0;
    while (!vq.empty()) {
        // pop queue
        visited_node = vq.front();
        vq.pop();

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
                return mmap[visited_node];
            }
            if (next > 0 && next <= 200000 && mmap[next] == 0) {
                // cout << next << "\t";
                mmap[next] = mmap[visited_node] + 1;
                vq.push(next);
            }
        }
    }
}

int main()
{
    int m, n;
    while (cin >> m >> n) {
        cout << search(m, n) << endl;
    }
}
