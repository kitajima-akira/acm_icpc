// dom2023b2.cpp - ACM ICPC 2023 国内予選B あみだくじ
// https://icpc.iisf.or.jp/past-icpc/domestic2023/contest/all_ja.html#section_B
// 解説にあるO(m)の方法で実装
// https://icpc.iisf.or.jp/past-icpc/domestic2023/commentaries.html#B
// Copyright 2023 Kitajima Akira
// Author: Kitajima Akira <kitajima@osakac.ac.jp>

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

// 横線にしたがってあみだくじを引く。
int amida_trace(const vector<int>& hlines, int p, vector<int>& trace) {
	trace.push_back(p);
	// 上から順にたどっていく。(pを動かす)
	for (auto& x : hlines) {
		if (x + 1 == p)  // 左側に横線
			p = x;
		else if (x == p)  // 左側に横線
			p = x + 1;
		trace.push_back(p);
	}

	return p;
}

// 1本追加してpからqにたどりつくのがあるか調べる。
pair<int, int> explore(int n, const vector<int>& hlines, int p, const vector<int>& trace_p, int q) {
	// qを下からたどり、どう進むのかtrace_qを得る。
	vector<int> trace_q;
	{
		// hlinesの逆順を求める。
		auto reverse_hlines = hlines;
		reverse(reverse_hlines.begin(), reverse_hlines.end());

		// qから逆にたどる。
		amida_trace(reverse_hlines, q, trace_q);

		// たどった順を逆にして上からの順にする。
		reverse(trace_q.begin(), trace_q.end());
	}

	// pの順とqの順を比較し、隣になる位置があればそこに横棒を入れる。
	for (int y = 0; y < trace_p.size(); y++) {
		if (trace_p[y] == trace_q[y] + 1) { // p, qの順に並ぶ
			return { trace_p[y] - 1, y };
		}
		else if (trace_q[y] == trace_p[y] + 1) { // q, pの順に並ぶ
			return { trace_q[y] - 1, y };
		}
	}

	return { -1, -1 };  // 見つからない
}

int main() {
	for (int n, m, p, q; cin >> n >> m >> p >> q && n > 0 || m > 0 || p > 0 || q > 0; ) {
		vector<int> hlines(m);
		for (auto& x : hlines)
			cin >> x;
		vector<int> trace;
		if (amida_trace(hlines, p, trace) == q) {
			cout << "OK" << endl;
		}
		else {
			auto ans = explore(n, hlines, p, trace, q);
			if (ans.first >= 0)
				cout << ans.first << " " << ans.second << endl;
			else
				cout << "NG" << endl;
		}
	}
	return 0;
}
