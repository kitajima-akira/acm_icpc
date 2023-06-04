// dom2019a.cpp - ACM ICPC 2019年国内予選A 期末試験の成績
// https://icpc.iisf.or.jp/past-icpc/domestic2019/problems/ja/contest/all_ja.html#section_A
// Copyright 2023 Kitajima Akira
// Author: Kitajima Akira <kitajima@osakac.ac.jp>
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
	for (int n, m; cin >> n >> m && !((n == 0) && (m == 0)); ) {// n 人数, m 科目数

		vector<int> sum(n, 0);  // n人分の合計値 (初期値0)

		// 各人の合計値を求める。
		for (int j = 0; j < m; j++) {
			// 科目ごとの処理
			for (auto& s : sum) {
				// 人ごとの処理
				int p;  // 得点データ
				cin >> p;
				s += p;  // 合計値に得点を追加する。
			}
		}

		// 最大値を見つける。
		auto it = max_element(sum.begin(), sum.end());

		// 1番大きい値を出力する。
		cout << *it << endl;
	}
	return 0;
}
