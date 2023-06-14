// dom2018a.cpp - ACM ICPC 2018年国内予選A - 所得格差
// https://icpc.iisf.or.jp/past-icpc/domestic2018/contest/all_ja.html#section_A
// Copyright 2023 Kitajima Akira
// Author: Kitajima Akira <kitajima@osakac.ac.jp>

#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

// 所得データ1セットに対して解を得る。
int get_answer(const vector<int>& incomes) {
	// 合計値を計算する。
	int sum = accumulate(incomes.begin(), incomes.end(), 0);  // 所得の合計値
	// 所得の平均値を計算する。
	double average = static_cast<double>(sum) / incomes.size();
	// 平均以上の人数を計算する。
	return count_if(incomes.begin(), incomes.end(), [&](int a) {return a <= average; });
}

int main() {
	for (int n; cin >> n && n > 0; ) {
		// 入力を得る。
		vector<int> incomes(n);  // 所得のリスト
		for (auto& a : incomes)
			cin >> a;

		// 条件を満たす人数を数えて出力する。
		cout << get_answer(incomes) << endl;
	}
	return 0;
}
