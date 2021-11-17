// dom2021d2.cpp - 風船配り
// https://icpc.iisf.or.jp/past-icpc/domestic2021/contest/all_ja.html
// 動的計画法で解く
// https://icpc.iisf.or.jp/past-icpc/domestic2021/commentaries.html#D

#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

// 与えられる子供の最大数を求める。
int get_max_number_of_kids(vector<int> b) {
	// 動的計画法を利用するため、並べ替える。
	sort(b.begin(), b.end());

	// dp[k][x][y] = "{b1, ..., bk} を 3 分割して和が (x, y, (Σki=1 bi)-x-y) となるようにできるか否か"
	// つまり、k番目までのデータで、和がx, yとその残りに分割できるか
	const int SUM_MAX = 50 * b.size();
	vector<vector<vector<int>>> dp(b.size(), vector<vector<int>>(SUM_MAX, vector<int>(SUM_MAX, false)));

	// 初期設定
	// b[0]だけの場合に、3分割するのは3通り (途中では0の分割があってもよい。)
	dp[0][0][0] = true;
	dp[0][b[0]][0] = true;
	dp[0][0][b[0]] = true;
	int sum_b = 0;  // (Σki=1 bi)

	// 表を埋める (分割できる組合せを求める。)
	for (size_t i = 0; i < b.size() - 1; i++) {
		sum_b += b[i];
		const int bb = b[i + 1];  // 新しいb

		// dp[i][x][y]となるすべての組合せをもとにdp[i + 1]を求める。
		for (int x = 0; x <= sum_b; x++) {
			for (int y = 0; y <= x; y++) {
				if (dp[i][x][y]) {  // x, yで分割可能なら
					dp[i + 1][x][y] = true;
					dp[i + 1][x + bb][y] = true;
					dp[i + 1][x][y + bb] = true;
				}
			}
		}
	}

	// 分割可能なx, yとその残りに対して、和の最小値の最大値を求める。
	int k_max = 0;
	const int i = b.size() - 1;
	sum_b += b[i];
	for (int x = 1; x < sum_b; x++) {
		for (int y = 1; y <= x; y++) {
			if (dp[i][x][y]) {  // x, yで分割可能なら
				const int sum_min = min(min(sum_b - x - y, x), y);  // 和の最小値
				k_max = max(k_max, sum_min);
			}
		}
	}

	return k_max;
}

int main() {
	for (int n; (cin >> n) && (n > 0); ) {
		vector<int> b(n);
		for (auto& i : b)
			cin >> i;
		cout << get_max_number_of_kids(b) << endl;
	}
	return 0;
}
