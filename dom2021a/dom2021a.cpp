// dom2021a.cpp - ビー玉占い
// https://icpc.iisf.or.jp/past-icpc/domestic2021/contest/all_ja.html#section_A
// Copyright 2023 Kitajima Akira
// Author: Kitajima Akira <kitajima@osakac.ac.jp>

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

// ラッキーナンバーを求める。
// aのbeg以降を対象とする。
// aは更新される。
// ラッキーナンバーを返す。
int lucky_number(vector<int>& a, const vector<int>::iterator& beg) {
	// 昇順に並べ替える。
	sort(beg, a.end());

	// 空(値が0)の椀を飛ばす。
	auto min_non_zero_p = find_if_not(beg, a.end(), [](const int& x) { return x == 0; });

	const int m = *min_non_zero_p;  // 一番少ない椀のビー玉の個数

	if (min_non_zero_p + 1 == a.end())  // 空でない椀が一つだけ
		return m;  // 残ったビー玉の個数があなたの今日のラッキーナンバー

	// first_non_zero以外の椀からm減らす。
	for_each(min_non_zero_p + 1, a.end(), [&m](int& x) { x -= m; });

	return lucky_number(a, min_non_zero_p);  // 再帰的に繰り返す。
}

int main() {
	for (int a1, a2, a3, a4; (cin >> a1 >> a2 >> a3 >> a4) && (a1 || a2 || a3 || a4); ) {
		vector<int> a = { a1, a2, a3, a4 };
		cout << lucky_number(a, a.begin()) << endl;
	}
	return 0;
}
