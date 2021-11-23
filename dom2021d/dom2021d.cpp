// dom2021d.cpp - 風船配り
// https://icpc.iisf.or.jp/past-icpc/domestic2021/contest/all_ja.html#section_D

#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

// 風船をk個除く。
// 次のポール番号を返す。
int remove_baloon(vector<int>& b, size_t i, int k = 1) {
	if ((b[i] -= k) == 0)
		b.erase(b.begin() + i);
	else
		i++;
	return i;
}

// 与えられる子供の数を求める。
int get_number_of_kids(vector<int> b) {
	int k = 0;  // 与えられる子供の数
	while (b.size() >= 3) {
		int m = min(min(b[0], b[1]), b[2]);
		k += m;
		// 風船がなくなったらポールも消すので2から0の順となる。
		remove_baloon(b, 2, m);
		remove_baloon(b, 1, m);
		remove_baloon(b, 0, m);
	}

	return k;
}

// 与えられる子供の最大数を求める。
int get_max_number_of_kids(vector<int> b) {
	// permutationを利用するため、並べ替える。
	sort(b.begin(), b.end());

	// 全パターンをpermutationで列挙し、最大値を求める。
	int k_max = 0;  // 求める最大値
	do {
		k_max = max(k_max, get_number_of_kids(b));
	} while (next_permutation(b.begin(), b.end()));
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
