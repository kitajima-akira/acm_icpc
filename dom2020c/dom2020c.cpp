// dom2020c.cpp - ACM ICPC2020年国内予選C 荷物
// Copyright 2021 KITAJIMA Akira
// https://icpc.iisf.or.jp/past-icpc/domestic2020/contest/all_ja.html#section_C

#include <algorithm>
#include <iostream>
#include <map>
#include <cmath>
#include <vector>
using namespace std;

typedef long long int longint;  // データとして取り扱う数値の型
typedef map<longint, int> prime_factor;  // 素因数分解した数の型

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

// p = w * d * hとなるh(とd)を列挙する。
// 再帰的に呼び出され、1回の処理はd * hの素因数一つに対する場合分け
// w >= d >= h >= 1の場合のみ
// 
// 引数
//   dxh, w	決まった値
//   pf_dxh	d * hの素因数分解形
//   it		pfのどの要素まで場合分けしたか
//   h		現在までに確定しているhの素因数の積
//
// 戻り値
//   d + hの最小値
longint explore_h(
			const longint dxh,
			const longint w,
			const prime_factor& pf_dxh, // 元になる数 (素因数分解)
			prime_factor::const_iterator it,  // どこから数え上げるか
			longint h = 1  // (現在の)h
		) {
	// 素因数がもうない場合
	if (it == pf_dxh.end()) {
		// hが確定
		// dはその値で決まる。
		const auto d = dxh / h;
		return d + h;
	}

	// 今回の素因数をfactorとする。
	const auto factor = it->first;  // 素因数
	const auto n_factor = it->second;   // 素因数の個数
	// itを次に進めておく。
	++it;

	// w >= d >= h >= 1の場合のみだから、
	// w >= h ... 条件1
	//
	// d >= h
	// d * h >= h * h
	// sqrt(d * h) >= h ...条件2
	const auto h_max = min(w, static_cast<longint>(sqrt(dxh)));  // 条件1と条件2を両方満たすhの上限

	longint s_min = 3 * w;
	// wに含まれるfactorの個数ごとに場合分け
	for (int i = 0; i <= n_factor; i++) {
		s_min = min(s_min, explore_h(dxh, w, pf_dxh, it, h));
		h *= factor;
		if (h > h_max)  // リミットを超えると打ち切り
			break;
	}

	return s_min;
}

// p = w * d * hとなるw(とhとd)を列挙する。
// hとdの列挙はexplore_h()を呼び出して行う。
// 再帰的に呼び出され、1回の処理はpの素因数一つに対する場合分け
// w >= d >= h >= 1の場合のみ
// pf_pを書き換え、pf_dxh(dxhの素因数分解形)を作りながら処理を進める。
// 
// 引数
//   p		決まった値
//   pf_p	pの素因数分解形
//   it		pfのどの要素まで場合分けしたか
//   w		現在までに確定しているwの素因数の積
//
// 戻り値
//   w + d + hの最小値
longint explore(
			const longint p,
			prime_factor& pf_p, // 元になる数 (素因数分解)
			prime_factor::const_iterator it,  // どこから数え上げるか
			longint w = 1  // (現在の)w
		) {
	// 素因数がもうない場合
	if (it == pf_p.end()) {
		// wが現在の引数で与えられたwで確定

		// w >= d >= h >= 1のとき
		// w * w * w >= w * d * h
		// w * w >= d * h
		// このときのみdを列挙する。
		const auto dxh = p / w;
		return (w < sqrt(dxh)) 
			? p + 2  // 後で更新される大きい値 (和の最大値はp + 2 (1 * 1 * p)の場合のため)
			: w + explore_h(dxh, w, pf_p, pf_p.begin());  // ここでのpf_pはpf_dxh (dxhの素因数分解形)を表す。
	}
	
	// 今回の素因数をfactorとする。
	const auto factor = it->first;  // 素因数
	const auto n_factor = it->second;   // 素因数の個数
	// itを次に進めておく。
	++it;

	longint s_min = p + 2;

	// wに含まれるfactorの個数ごとに場合分け
	for (int i = 0; i <= n_factor; i++) {
		s_min = min(s_min, explore(p, pf_p, it, w));
		w *= factor;
		// pf_pを更新(wの分を減らし、pf_pがpf_dxhの内容を表すように更新する。)
		pf_p[factor]--;
	}
	// pf_pを更新(pf_dxhだったのを次のループのために元のpf_pに戻す)
	pf_p[factor] = n_factor;

	return s_min;
}

void print_factors(const prime_factor& pf) {
	for (const auto& p : pf) {
		cout << " " << p.first;
		if (p.second > 1)
			cout << "^" << p.second;
	}
	cout << endl;
}

void print_all_divisors(
	const prime_factor& pf, 
	prime_factor::const_iterator it, 
	longint d = 1
) {
	// itがpfの最後まで到達した場合
	if (it == pf.end()) {
		cout << d << endl;
		return;
	}
	// 今回の素因数をfactorとする。
	const auto factor = it->first;  // 素因数
	const auto n_factor = it->second;   // 素因数の個数
	// itを次に進めておく。
	++it;

	// dに含めるfactorの個数ごとに場合分け
	for (int i = 0; i <= n_factor; i++, d *= factor)
		print_all_divisors(pf, it, d);
}

int main() {
	for (longint p; (cin >> p) && (p != 0);) {  // 積pを読み出し、0になるまでループを回す。		
		// p素因数分解する。
		prime_factor pf_p;  // pを素因数分解した結果
		prime_factorization(p, pf_p);
//		print_factors(pf_p);
//		print_all_divisors(pf_p, pf_p.begin());

		// 3つの数に分けて和の最小値を求める。
		cout << explore(p, pf_p, pf_p.begin()) << endl;
	}
	return 0;
}
