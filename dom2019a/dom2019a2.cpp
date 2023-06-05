// dom2019a2.cpp - ACM ICPC 2019年国内予選A 期末試験の成績
// https://icpc.iisf.or.jp/past-icpc/domestic2019/problems/ja/contest/all_ja.html#section_A
// Copyright 2023 Kitajima Akira
// Author: Kitajima Akira <kitajima@osakac.ac.jp>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

int main() {
	for (int n, m; cin >> n >> m && !((n == 0) && (m == 0)); ) {// n 人数, m 科目数
		// データを得る。
		vector<vector<int>> table(m, vector<int>(n));  // 科目ごとの生徒の得点
		for (auto& subject_data : table)
			for (auto& p : subject_data)
				cin >> p;

		// 生徒ごとの和を求める。
		vector<int> sum(n, 0);  // 生徒ごとの合計点
		for (const auto& subject_data : table)
			for (int i = 0; i < n; i++)
					sum[i] += subject_data[i];

		// 1番大きい値を出力する。
		cout << *max_element(sum.begin(), sum.end()) << endl;
	}
	return 0;
}
