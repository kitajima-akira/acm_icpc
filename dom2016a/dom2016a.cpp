// dom2016a.cpp - ACM ICPC 2016年国内予選A 被験者の選定
// https://icpc.iisf.or.jp/past-icpc/domestic2016/problems/all_ja.html#section_A
// Copyright 2023 Kitajima Akira
// Author: Kitajima Akira <kitajima@osakac.ac.jp>

#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

// 配列gradesの差の最小値を求める。
int get_min_diff(const vector<int>& a) {
	auto copied = a;
	// 並べ替える。
	sort(copied.begin(), copied.end());

	// 隣同士の差を求める。
	vector<int> diffs(copied.size());
	adjacent_difference(copied.begin(), copied.end(), diffs.begin());

	// 差の最小値を求める。
	return *min_element(diffs.begin() + 1, diffs.end());
}

int main() {
	for (int n; cin >> n && n > 0; ) {
		vector<int> grades(n);
		for (auto& a : grades)
			cin >> a;

		cout << get_min_diff(grades) << endl;
	}
	return 0;
}
