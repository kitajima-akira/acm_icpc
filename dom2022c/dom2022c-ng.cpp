// dom2022c-ng.cpp - [ACM ICPC 2022 C] ICPC に向けての練習日程 (失敗例)
// https://icpc.iisf.or.jp/past-icpc/domestic2022/contest/all_ja.html#section_C
#include <iostream>
#include <utility>
using namespace std;

// n日連続分の値を計算する。
// 問題どおりに計算する。
// 時間がかかる。
long long int get_power1(int n) {
	long long int p = 0;

	for (int k = 1; k <= n; k++)
		p += static_cast <long long int>(2 * k - 1);
	return p;
}

// 深さ優先探索で1日目から順に練習する場合としない場合とを求めて能力値の大きい方を採用する。
// この方法では、n + mが大きいと時間がかかりすぎて終わらない。
// 
// n 練習日数
// m 休息日数
// kt 連続練習日数
// kr 連続休息日数
long long int get_max_power(int n, int m, int kt = 1, int kr = 1) {
	if (n <= 0 && m <= 0)
		// すべての日程を終了
		return 0;

	long long int power;
	if (n > 0) {
		// 練習する場合
		power = 2 * kt - 1 + get_max_power(n - 1, m, kt + 1, 1);
		if (m > 0) {
			// 練習せず休息する場合
			const long long int power_r = -(2 * kr - 1) + get_max_power(n, m - 1, 1, kr + 1);
			power = max(power, power_r);
		}
	}
	else {
		// 練習できない場合 (休息)
		power = - (2 * kr - 1) + get_max_power(n, m - 1, 1, kr + 1);
	}
	
	// デバッグ用
	return power;
}

int main() {
	for (int n, m; (cin >> n >> m) && (n > 0 || m > 0);)
		cout << get_max_power(n, m) << endl;

	return 0;
}
