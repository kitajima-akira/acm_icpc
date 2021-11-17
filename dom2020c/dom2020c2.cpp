// dom2020c.cpp - 荷物
// https://icpc.iisf.or.jp/past-icpc/domestic2020/contest/all_ja.html#section_C

#include <algorithm>
#include <iostream>
#include <cmath>
using namespace std;

typedef long long int longint;  // データとして取り扱う数値の型

// nの約数のうち、dより大きい次の約数を求める。
// dより大きい次の約数がなければd + 1を返す。
// ****時間がかかりすぎて無理****
longint next_divisor(longint n, longint d) {
	// dより大きい次の約数がない場合
	if (n <= d)
		return d + 1;

	// dより大きくn以下の約数を見つける。
	for (longint i = d + 1; i <= n; i++)
		if (n % i == 0)
			return i;

	// n以外の約数が見つからない場合
	return n;
}

// p = w * d * hからs = w + d + hの最小値を得る。
longint estimate(longint p) {
	longint s_min = p + 2;  // p * 1 * 1 が1番大きくなるはずなので初期値に設定
	// wをpの約数すべての場合で試す。
	cout << "sqrt(p) = " << static_cast<longint>(sqrt(p)) << endl;
	for (longint w = 1; w <= p; w = next_divisor(p, w)) {
		const longint dxh = p / w;  // d * h

		// w >= d >= h >= 1のとき
		// w * w * w >= w * d * h
		// w * w >= d * h
		// w >= sqrt(d * h)
		// このときのみd, hを列挙する。
		const auto sqrt_dxh = sqrt(dxh);
//		cout << w << "(" << dxh << ((w >= sqrt_dxh) ? " <= " : " > ") << w * w << ")" << endl;
		if (w < sqrt_dxh)
			continue;
		cout << "case w = " << w << ", d * h = " << dxh << endl;
		// hをp / w (= d * h)の約数すべての場合で試す。
		// w >= d >= h > 0の場合のみだから、
		// d >= h
		// d * h >= h * h
		// sqrt(d * h) >= h ...条件2
		for (longint h = 1; h <= sqrt_dxh && h <= w; h = next_divisor(dxh, h)) {
				// dが決まる。
			const longint d = dxh / h;
			cout << "\tcase h = " << h << ", d = " << d << endl;
			if (d > w)
				continue;
			cout << "\t\tcase d = " << d << endl;
			// sを求める。
			const longint s = w + d + h;
			if (s_min > s)
				cout << "s = " << s << " = " << w << " + " << d << " + " << h << endl;
			// s_minを更新する。
			s_min = min(s_min, s);
		}
	}
	return s_min;
}

int main() {
	for (longint p; // 積
		(cin >> p) && (p != 0);) {	
		cout << estimate(p) << endl;
	}
	return 0;
}
