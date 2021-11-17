// dom2020c.cpp - 荷物
// https://icpc.iisf.or.jp/past-icpc/domestic2020/contest/all_ja.html#section_C

#include <algorithm>
#include <iostream>
#include <map>
#include <cmath>
#include <vector>
using namespace std;

class luggage {
public:
	typedef long long int longint;  // データとして取り扱う数値の型
	typedef map<longint, int> prime_factor;  // 素因数分解した数の型

	luggage(longint given) : 
		p(given),
		w(),
		d(),
		h(),
		pf_p(),
		it()
		{
		// 素因数分解する。
		prime_factorization(p, pf_p);
		it = pf_p.begin();
	}

	longint explore(longint current_w = 1) {
		longint s_min = p + 2;  // 和の最大値はp + 2 (1 * 1 * p)の場合

		if (it == pf_p.end()) {
			// wが確定
			w = current_w;
	//		cout << "w = " << w << endl;

			// w >= d >= h > 0のとき
			// w * w * w >= w * d * h
			// w * w >= d * h
			// このときのみdを列挙する。
			const auto dxh = p / w;
			return (w < sqrt(dxh)) ? s_min : w + explore_h(pf_p.begin());
		}

		const auto factor = it->first;  // 素因数
		const auto n_factor = it->second;   // 素因数の個数
		++it;

		// factorの個数が0の場合
		s_min = min(s_min, explore(current_w));

		// wのfactorについての個数を決める。
		for (int i = 1; i <= n_factor; i++) {
			current_w *= factor;
			// pf_pを更新(wの分を減らす)
			pf_p[factor]--;
			s_min = min(s_min, explore(current_w));
		}
		// pf_pを更新(次のループのために元に戻す)
		pf_p[factor] = n_factor;

		return s_min;
	}

private:
	longint p;  // p
	longint w, d, h;
	prime_factor pf_p;  // 素因数分解した結果
	prime_factor::const_iterator it;  // どこから数え上げるか

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

	// w >= d >= h > 0の場合のみ dとhを求める。
	longint explore_h(prime_factor::const_iterator it, longint current_h = 1) {
		if (it == pf_p.end()) {
			// hが確定
			h = current_h;
			// dはその値で決まる。
			const auto d = p / w / h;
	//		cout << "w = " << w << ", d = " << d << ", h = " << h << endl;
			return d + h;
		}

		const auto factor = it->first;  // 素因数
		const auto n_factor = it->second;   // 素因数の個数
		++it;

		// w >= d >= h > 0の場合のみだから、
		// w >= h ... 条件1
		// d >= h
		// d * h >= h * h
		// sqrt(d * h) >= h ...条件2 (ここで d * h == p / w)
		const auto h_max = min(w, static_cast<longint>(sqrt(p / w)));  // 条件1と条件2を両方満たすhの上限

		// factorの個数が0の場合
		longint s_min = explore_h(it, current_h);

		// hのfactorについての個数を決める。
		for (int i = 1; i <= n_factor; i++) {
			current_h *= factor;
			if (current_h > h_max)  // リミットを超えると打ち切り
				return s_min;
			s_min = min(s_min, explore_h(it, current_h));
		}

		return s_min;
	}
};

int main() {
	for (luggage::longint p; // 積
		(cin >> p) && (p != 0);) {		
		luggage lg(p);
		// 3つの数に分けて和の最小値を求める。
		cout << lg.explore() << endl;
	}
	return 0;
}
