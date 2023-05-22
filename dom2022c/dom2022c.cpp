// dom2022c.cpp - [ACM ICPC 2022 C] ICPC に向けての練習日程
// https://icpc.iisf.or.jp/past-icpc/domestic2022/contest/all_ja.html#section_C
#include <iostream>
#include <utility>
using namespace std;

// n日連続分のICPC能力を得る。
long long int get_power(long long int n) {
	return n * n;
}

// 練習n日、休息m日の場合のICPC能力の最大値を得る。
long long int get_max_power(int n, int m) {
	if (n == m)
		return 0;

	if (n == 0)
		return -get_power(m);

	// o(練習)は連続の方がいい。
	// x(休息)はなるべく均等に分割した方がいい。

	// 2分割した場合
	// xxxxxoooooooxxxx
	int r = m % 2;
	int d = m / 2;
	long long int p = -get_power(d) + get_power(n) - get_power(d + r);

	// xがうまく分割できる(あまりなし)と、細かくわけてもよりよくなる場合がある。
	// xxx o xxx oooooo xxx

	// xをs分割する。
	for (int s = 3; s <= n + 1 && s <= m; s++) {
		r = m % s;
		d = m / s;
		long long int new_p = -(s - r) * get_power(d)  // あまってないx
			+ (s - 2) * get_power(1)  // 分割のためのo
			+ get_power(n - (s - 2))  // oの最大化
			- r * get_power(d + 1);  // あまりのあるx

		if (new_p > p)
			p = new_p;
		else
			// 良くならなければそこで終わる。
			return p;
	}
	return p;
}

int main() {
	for (int n, m; (cin >> n >> m) && (n > 0 || m > 0);)
		cout << get_max_power(n, m) << endl;

	return 0;
}
