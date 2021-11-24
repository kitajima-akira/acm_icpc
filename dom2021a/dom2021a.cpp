// dom2021a.cpp - ビー玉占い
// https://icpc.iisf.or.jp/past-icpc/domestic2021/contest/all_ja.html#section_A

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int lucky_number(vector<int> a) {
	if (a.size() == 1)
		return a[0];

	// 昇順に並べ替える。
	sort(a.begin(), a.end());

	// 空の椀を除く。
	while (a[0] == 0)
		a.erase(a.begin());

	// 一番少ない椀のビー玉の個数を得る。
	const int m = a[0];

	// 先頭以外の椀からm減らす。
	for (auto i = a.begin() + 1; i != a.end(); ++i) {
		*i -= m;
	}

	return lucky_number(a);
}

int main() {
	for (int a1, a2, a3, a4; (cin >> a1 >> a2 >> a3 >> a4) && (a1 || a2 || a3 || a4); ) {
		vector<int> a = { a1, a2, a3, a4 };
		cout << lucky_number(a) << endl;
	}
	return 0;
}
