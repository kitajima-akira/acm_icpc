// dom2020c.cpp - 荷物
// https://icpc.iisf.or.jp/past-icpc/domestic2020/contest/all_ja.html#section_C

#include <algorithm>
#include <iostream>
#include <map>
#include <cmath>
#include <vector>
using namespace std;

typedef long long int longint;  // データとして取り扱う数値の型
typedef map<longint, int> prime_factor;  // 素因数分解した数の型
typedef map<longint, longint> divisors;  // 約数リストの型

// pから素因数fを除く。
// 除かれた個数を返す。
int remove_factor(longint& p, longint f) {
	int c = 0;
	while (p % f == 0) {
		c++;
		p /= f;
	}
	return c;
}

// pを素因数分解した結果をpfに格納する。
void prime_factorization(longint p, prime_factor& pf) {
	// 2の数を数える。
	int c = remove_factor(p, 2);
	if (c > 0)
		pf.emplace(2, c);

	// 残りの奇数を数える。
	for (longint f = 3; f * f <= p; f += 2) {
		c = remove_factor(p, f);
		if (c > 0)
			pf.emplace(f, c);
	}

	// 残った値を追加する。
	if (p > 1)
		pf.emplace(p, 1);
}

void get_divisor(
	const longint p,
	prime_factor& pf_p, // 元になる数 (素因数分解)
	prime_factor::const_iterator it,  // どこから数え上げるか
	divisors& div_list,
	longint d = 1  // (現在の)d
) {
	if (it == pf_p.end()) {
		// dが確定

		const longint co_divisor = p / d;
		if (d <= co_divisor)
			div_list.emplace(d, co_divisor);
		else
			div_list.emplace(co_divisor, d);
		return;
	}

	const auto factor = it->first;  // 素因数
	const auto n_factor = it->second;   // 素因数の個数
	++it;

	// dのfactorについての個数を決める。
	for (int i = 0; i <= n_factor; i++) {
		get_divisor(p, pf_p, it, div_list, d);
		d *= factor;
	}
}

int main() {
	for (longint p; // 積
		(cin >> p) && (p != 0);) {		
		// 素因数分解する。
		prime_factor pf_p;  // 素因数分解した結果
		prime_factorization(p, pf_p);

		// wの候補を求める。
		divisors div_p;
		get_divisor(p, pf_p, pf_p.begin(), div_p);

//		cout << "divisor: ";
//		for (const auto& d : div_p) {
//			cout << d.first << "-" << d.second << " ";
//		}
//		cout << endl;

		longint s_min = p + 2;
		for (const auto& div : div_p) {
			auto w = div.second;
			auto dxh = div.first;

			prime_factor pf_dxh;  // 素因数分解した結果
			prime_factorization(dxh, pf_dxh);

			divisors div_dxh;
			get_divisor(dxh, pf_dxh, pf_dxh.begin(), div_dxh);

			auto h = div_dxh.rbegin()->first;
			auto d = div_dxh.rbegin()->second;
			s_min = min(s_min, w + d + h);

			if (w != dxh && sqrt(w) <= dxh) {
				auto dxh = div.second;
				auto w = div.first;

				prime_factor pf_dxh;  // 素因数分解した結果
				prime_factorization(dxh, pf_dxh);

				divisors div_dxh;
				get_divisor(dxh, pf_dxh, pf_dxh.begin(), div_dxh);

				auto h = div_dxh.rbegin()->first;
				auto d = div_dxh.rbegin()->second;
				s_min = min(s_min, w + d + h);
			}
		}
		cout << s_min << endl;
	}
	return 0;
}
