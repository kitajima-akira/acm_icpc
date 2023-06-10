// dom2018c.cpp - ACM ICPC 2018年国内予選C - 超高層ビル「みなとハルカス」
// https://icpc.iisf.or.jp/past-icpc/domestic2018/contest/all_ja.html#section_B
// Copyright 2023 Kitajima Akira
// Author: Kitajima Akira <kitajima@osakac.ac.jp>

#include <iostream>
using namespace std;

pair<int, int> solve(int b) {
	const long long int b2 = b * 2LL;

	// 下の階からぴったり予算に合うかを調べる。
	const auto limit = (b - 1) / 2;
	for (int n = 1; n < limit; n++) {
		const long long int n2 = n * 2LL;
		for (long long int i = 1LL, f; (f = (n2 + i - 1) * i) <= b2; i++)
			if (b2 == f)
				return { n, i };
	}

	return { b , 1 };
}

int main() {
	for (int b; cin >> b && b > 0; ) {
		auto answer = solve(b);
		cout << answer.first << " " << answer.second << endl;
	}
	return 0;
}
