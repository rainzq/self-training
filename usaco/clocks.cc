/*
ID: rain2wh1
PROB: clocks
LANG: C++11
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <algorithm>

using namespace std;

class DialStat {
    private:
        int stat;
        static int tran_table[9];
        const int clear_over_bit_num = 0333333333;

    public:
        DialStat(): stat(0) {}
        explicit DialStat(int stat): stat(stat) {}
        explicit DialStat(vector<int> data): stat(0) {
            for (auto it = data.begin(); it != data.end(); ++it) {
                int t = ((*it) / 3) & 3;
                stat = (stat << 3) + t;
            }
        }

        DialStat transform(int i) const {
            int res = (stat + tran_table[i - 1]) & clear_over_bit_num;
            return DialStat(res);
        }
        bool is_final_state() const {
            if (stat == 0) return true;
            else return false;
        }
};

template<typename T>
std::istream & operator >> (std::istream &in, vector<T> &v)
{
    for (unsigned i = 0; i < v.size(); ++i) {
        in >> v[i];
    }
    return in;
}

template<typename T>
std::ostream & operator << (std::ostream &out, vector<T> &v)
{
    unsigned i = 0;
    if (v.size()) {
        out << v[i];
    } else {
        return out;
    }
    for (i = 1; i < v.size(); ++i) {
        out << " " << v[i];
    }
    return out;
}

// 参考http://www.nocow.cn/index.php/USACO/clocks 进行位运算加速
// 每一个时钟状态有4种状态，采用8进制表示，便于后续进行状态转换时清除溢出的情况
int DialStat::tran_table[9] = {0110110000, 0111000000, 0011011000, 0100100100, 0010111010, 0001001001, 0000110110, 0000000111, 0000011011};

ofstream fout("clocks.out");
ifstream fin("clocks.in");
vector<int> result;
void idfs(DialStat s, int k, int d);

// 结果从小到大遍历，并且不重复，每次的变换i都大于等于上一次
// result 数组按从小到大的顺序，最多变换3次
// 1 1 1 2 2 2 3 3 3
// 这样遍历的时候子节点全都是没有访问过的
// 可以当做数来遍历，所以不需要记录节点是否访问过
void idfs(DialStat s, int k, int d)
{
    if (d == 0 && s.is_final_state()) {
        fout << result << endl;
        exit(0);
    }
    if (d == 0) {
        return;
    }
    for (int i = k; i <= 9; ++i) {
        DialStat t = s.transform(i);
        result.push_back(i);
        // 寻找边界k
        k = result.back();
        int sz = result.size();
        if (sz >= 3 && result[sz-3] == k) {
            k += 1;
        }

        idfs(t, k, d - 1);
        result.pop_back();
    }
}

int main()
{
    vector<int> indata(9);
    fin >> indata;
    DialStat s(indata);

    int d = 0;
    for (d = 0; d <= 27; d++) {
        result.clear();
        idfs(s, 1, d);
    }
    return 0;
}

