// dom2020a2.cpp - カウントアップ2020
// https://icpc.iisf.or.jp/past-icpc/domestic2020/contest/all_ja.html#section_A
// Copyright 2023 Kitajima Akira
// Author: Kitajima Akira <kitajima@osakac.ac.jp>

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
	const vector<int> a2020 = { 2, 0, 2, 0 };  // 検索対象データ

	for (int n; cin >> n && n != 0;) {
		vector<int> d(n);  // 整数の並び
		for (int& i : d)
			cin >> i;

		int c = 0;  // 出現回数
		for (auto it = d.begin();
			(it = search(it, d.end(), a2020.begin(), a2020.end())) != d.end(); 
			++it) {
			c++;
		}
		cout << c << endl;
	}
	return 0;
}
