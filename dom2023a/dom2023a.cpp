// dom2023a.cpp - ACM ICPC 2023年国内予選A スポンサー賞はどのチーム？
// https://icpc.iisf.or.jp/past-icpc/domestic2023/contest/all_ja.html#section_A
// Copyright 2023 Kitajima Akira
// Author: Kitajima Akira <kitajima@osakac.ac.jp>

#include <iostream>
#include <vector>
using namespace std;

// 配列aの中からvalueに一番近いものを探す。
// 見つけた位置を返す。
int find_nearest(const vector<int>& a, int value) {
	int min_diff = abs(a[0] - value) + 1;  // valueとの差の最小値
	int min_index = -1;  // 最小値の位置
	for (int i = 0; i < a.size(); i++) {
		const auto diff = abs(a[i] - value);
		if (diff < min_diff) {
			min_diff = diff;
			min_index = i;
		}
	}
	return min_index;
}

int main() {
	for (int n; cin >> n && n > 0; ) {
		vector<int> time_list(n);
		for (auto& a : time_list)
			cin >> a;

		cout << find_nearest(time_list, 2023) + 1 << endl;
	}
	return 0;
}
