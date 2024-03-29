// dom2023d.cpp - ACM ICPC 2023年国内予選D 効率的な問題セット
// https://icpc.iisf.or.jp/past-icpc/domestic2023/contest/all_ja.html#section_D
// Copyright 2023 Kitajima Akira
// Author: Kitajima Akira <kitajima@osakac.ac.jp>

#include <iostream>
#include <string>
#include <set>
using namespace std;

// valuesの数値を組み合わせて(うまく選んで足すと)vになるかどうか判定する。
// valuesはsetで、昇順となっていることを前提としている。
bool is_constructable(int v, const set<int>& values) {
	if (values.empty())
		return false;

	if (values.find(v) != values.end())
		return true;

	// valuesの先頭(最小)の要素を取り出す。
	auto front = *values.begin();

	if (front > v)
		return false;  // valuesの残りはfrontより大きいので、もう見つからない。

	// frontを除く。
	auto copied_values = values;
	copied_values.erase(copied_values.begin());

	// frontを使う・使わないの両方を試す。
	return is_constructable(v, copied_values)  // 使わない 
		|| is_constructable(v - front, copied_values);  // 使う
}

// 最小問題数を求める。
int get_min_problems(const set<int>& required_points) {
	auto points = required_points;
	// 先頭の0は除外する。
	points.erase(points.begin());

	set<int> min_set;  // 最低限必要な点のリスト
	for (auto v : points)
		if (!is_constructable(v, min_set))
			min_set.insert(v);

	return min_set.size();
}

int main() {
	for (int n; cin >> n && n > 0;) {
		string s;
		cin >> s;
		set<int> required_points;
		for (int k = 0; k <= n; k++)
			if (s[k] == 'o')
				required_points.insert(k);

		cout << get_min_problems(required_points) << endl;

	}
	return 0;
}
