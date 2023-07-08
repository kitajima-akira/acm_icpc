// dom2023b.cpp - ACM ICPC 2023 国内予選B あみだくじ
// https://icpc.iisf.or.jp/past-icpc/domestic2023/contest/all_ja.html#section_B
// Copyright 2023 Kitajima Akira
// Author: Kitajima Akira <kitajima@osakac.ac.jp>

#include <iostream>
#include <vector>
using namespace std;

// 横線にしたがってあみだくじを引く。
int amida_draw(const vector<int>& hlines, int p) {
	// 上から順にたどっていく。(pを動かす)
	for (auto& x : hlines)
		if (x + 1 == p)  // 左側に横線
			p = x;
		else if (x == p)  // 左側に横線
			p = x + 1;

	return p;
}

// 1本追加してpからqにたどりつくのがあるか調べる。
pair<int, int> explore(int n, const vector<int>& hlines, int p, int q) {
	// 横線の追加する(上からの)位置yを順に調べる。
	for (int y = 0; y <= hlines.size(); y++) {
		// 追加する場所yに線を挿入する。
		auto new_hlines = hlines;
		new_hlines.insert(new_hlines.begin() + y, 0);

		// 横線の横位置xを順に調べる。
		for (int x = 1; x < n; x++) {
			// 横線の位置を定める。
			new_hlines[y] = x;

			// qにたどりつくか調べる。
			if (amida_draw(new_hlines, p) == q)
				return { x, y };
		}
	}
	return { -1, -1 };  // 見つからない
}

int main() {
	for (int n, m, p, q; cin >> n >> m >> p >> q && n > 0 || m > 0 || p > 0 || q > 0; ) {
		vector<int> hlines(m);
		for (auto& x : hlines)
			cin >> x;
		if (amida_draw(hlines, p) == q) {
			cout << "OK" << endl;
		}
		else {
			auto ans = explore(n, hlines, p, q);
			if (ans.first >= 0)
				cout << ans.first << " " << ans.second << endl;
			else
				cout << "NG" << endl;
		}
	}
	return 0;
}
