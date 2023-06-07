// dom2018a.cpp - ACM ICPC 2018年国内予選A - 所得格差
// https://icpc.iisf.or.jp/past-icpc/domestic2018/contest/all_ja.html#section_A
// Copyright 2023 Kitajima Akira
// Author: Kitajima Akira <kitajima@osakac.ac.jp>

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
	for (int n; cin >> n && n > 0; ) {
		// 入力を得て、合計値も計算しておく。
		vector<int> incomes(n);  // 所得のリスト
		
		int sum = 0;  // 所得の合計値
		for (auto& a : incomes) {
			cin >> a;
			sum += a;
		}

		// 所得の平均値を計算する。
		double average = static_cast<double>(sum) / n;

		// 条件を満たす人数を数えて出力する。
		cout << count_if(incomes.begin(), incomes.end(), [=](int a) {return a <= average; }) << endl;
	}
	return 0;
}
