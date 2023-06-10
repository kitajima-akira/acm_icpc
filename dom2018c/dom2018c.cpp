// dom2018c.cpp - ACM ICPC 2018年国内予選C - 超高層ビル「みなとハルカス」
// https://icpc.iisf.or.jp/past-icpc/domestic2018/contest/all_ja.html#section_C
// Copyright 2023 Kitajima Akira
// Author: Kitajima Akira <kitajima@osakac.ac.jp>

#include <functional>
#include <iostream>
using namespace std;

// f(i) = 0 に最も近いi (i_min <= i <= i_max)を見つける。
// f()は範囲内で単調増加でなければならない。
// iと計算値f(i)を返す。
template<typename T, class F>
pair<T, T> approximate(F f, T i_min, T i_max) {
	T diff;  // 上限と下限との差
	T r;  // 関数fの計算値
	while ((diff = i_max - i_min) > 1) {
		// 新しいiを決める。
		T i = i_min + (diff >> 1);

		// f(i)を求め、0かどうか判定する。
		if ((r = f(i)) == 0)
			return { i, r };

		// 0より大きいかどうかで次の範囲を決める。
		if (r > 0)
			i_max = i;
		else
			i_min = i;
	}
	return { i_min, r };
}

pair<int, int> solve(int b) {
	const long long int b2 = b * 2LL;

	// 下の階からぴったり予算に合うかを調べる。
	auto i_max = b / 2LL;  // iの上限
	for (int n = 1; i_max > 2; n++) {
		// n (1番下の階)が決まれば、合計bになるのに必要なi(何階分か)は計算で求まる。
		// 2 * b = (2 * n + i - 1) * i
		// (2 * n + i - 1) * i - 2 * b を0にするiを見つける。
		const auto p = approximate(
			[&](long long int i) {return (2LL * n + i - 1LL) * i - b2; },
			1LL,
			i_max);

		// 見つけたiがちょうどなら解となる。
		if (p.second == 0)
			return { n, p.first };

		// 一つ前の階からよりは少ないはずなので新たな上限を設定する。
		i_max = p.first + 1;
	}

	// iが1と2の場合は
	return b % 2 ? make_pair(b / 2, 2) : make_pair(b, 1);
}

int main() {
	for (int b; cin >> b && b > 0; ) {
		auto answer = solve(b);
		cout << answer.first << " " << answer.second << endl;
	}
	return 0;
}
