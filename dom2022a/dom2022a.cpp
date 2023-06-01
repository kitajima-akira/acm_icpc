// dom2022a.cpp - ACM ICPC 2022 国内予選A 感染ピークの回数
// https://icpc.iisf.or.jp/past-icpc/domestic2022/contest/all_ja.html#section_A

#include <iostream>
#include <vector>
using namespace std;

// データの並びaのピーク数を求める。
int count_peak(const vector<int>& a) {
	int peak_count = 0;
	// 三つの並び i, i + 1, i + 2 を一つずつずらして順に比較
	for (size_t i = 0; i + 2 < a.size(); i++)
		if (a[i + 1] > a[i] && a[i + 1] > a[i + 2])  // a[i + 1] が前後より大きい
			peak_count++;

	return peak_count;
}

int main() {
	for (int n; cin >> n && n > 0;) {  // nはデータの個数
		vector<int> positive_cases(n);  // 日ごとの新規陽性者数
		// データを読み込む。
		for (auto& v : positive_cases)  // vは格納する要素
			cin >> v;

		cout << count_peak(positive_cases) << endl;
	}
	return 0;
}
