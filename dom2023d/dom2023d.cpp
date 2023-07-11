// dom2023d.cpp - ACM ICPC 2023年国内予選D 効率的な問題セット
// https://icpc.iisf.or.jp/past-icpc/domestic2023/contest/all_ja.html#section_D
// Copyright 2023 Kitajima Akira
// Author: Kitajima Akira <kitajima@osakac.ac.jp>

#include <iostream>
#include <string>
#include <vector>
using namespace std;

// valuesの数値を組み合わせて(うまく選んで足すと)vになるかどうか判定する。
// valuesは昇順を前提としている。
bool constructable(int v, const vector<int>& values) {
	if (values.empty())
		return false;

	// valuesの先頭の要素を取り出す。
	auto front = values.front();

	if (front == v)
		return true;
	if (front > v)
		return false;  // valuesの残りはfrontより大きいので、もう見つからない。

	// frontを除く。
	auto copied_values = values;
	copied_values.erase(copied_values.begin());

	// frontを使う・使わないの両方を試す。
	return constructable(v, copied_values)  // 使わない 
		|| constructable(v - front, copied_values);  // 使う
}

// 最小問題数を求める。
int get_min_problems(const vector<int>& required_points) {
	auto points = required_points;
	// 先頭の0は除外する。
	points.erase(points.begin());

	vector<int> min_set;  // 最低限必要な点のリスト
	for (auto v : points)
		if (!constructable(v, min_set))
			min_set.push_back(v);

	return min_set.size();
}

int main() {
	for (int n; cin >> n && n > 0;) {
		string s;
		cin >> s;
		vector<int> required_points;
		for (int k = 0; k <= n; k++)
			if (s[k] == 'o')
				required_points.push_back(k);

		cout << get_min_problems(required_points) << endl;

	}
	return 0;
}
