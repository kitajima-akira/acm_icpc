// dom2022d3.cpp - [ACM ICPC 2022 D] 入場待ちの行列
// https://icpc.iisf.or.jp/past-icpc/domestic2022/contest/all_ja.html#section_D
//
// コロナ禍での特別ルールのため、事前準備が可能
// 
// 以下の部分は事前準備しておく。(コンテスト中に実装すると時間がなくなる)
//  * 長桁演算
//      https://github.com/square1001/bigint-library
//      参考 https://qiita.com/square1001/items/1aa12e04934b6e749962
//  * 組み合わせ(Combination)
//      下記コード参照
#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <set>
#include <vector>
#include "bigint-library/newbigint.h"
using namespace std;

// begin [組み合わせ] ///////////////////////////////////////////////////////////////

// 整数の冪乗計算
// 乗算回数が少なくなるように工夫 O(log r)
bigint power(long long int n, int r) {
	if (r == 0)
		return bigint(1);

	if (n == 2LL && r < 32)
		return n << (r - 1);
	
	// 以下、n > 2, r > 0の条件下で実行

	bigint p(n);  // 計算値
	vector<bigint> table = { p };  // n, n^2, n^4, ..., n^i の表

	// k = n^i < rとなる最大のkを求める。
	int i2 = 1;  // i^2
	while (2 * i2 <= r) {
		i2 *= 2; // i^2 + i^2
		p *= p;
		table.push_back(p);
	}

	int i = i2;  // 計算が終わった乗算回数
	while (i < r) {
		// 残り部分について n^j2 (j2は2の冪乗)となるj2を求める。
		int j2 = 1;
		int j = 1;
		while (i + 2 * j2 <= r) {
			j2 *= 2;
			j++;
		}

		p *= table[j - 1];
		i += j2;
	}

	return p;
}

// nを素因数分解する。
void get_prime_decomposition(int n, vector<int>& pl) {
	// 符号を求める。
	if (n < 0) {
		pl.push_back(-1);
		n = -n;
	}

	if (n == 0)
		return;

	// n = 1の場合
	if (n == 1) {
		if (pl.empty())
			pl.push_back(1);
		return;
	}

	// 素因数2の個数を数える。
	while (n % 2 == 0) {
		pl.push_back(2);
		n /= 2;
	}

	// 奇数の素因数を求める。
	for (int i = 3; i * i <= n; i += 2) {
		while (n % i == 0) {
			pl.push_back(i);
			n /= i;
		}
	}
	if (n > 1)
		pl.push_back(n);
}

// 有理数
// まだ実装は不完全 (乗算・除算のみ 正の数のみ 例外[0除算や0の表現など必要])
class rational_number {
public:
	rational_number(long long int n): 
		prime_factor_list() {
		// 素因数分解する。
		std::vector<int> pflist;
		get_prime_decomposition(n, pflist);

		// データを格納する。
		for (const auto& pf : pflist)
			if (pf != 1)
				prime_factor_list[pf] += 1;
	}

	rational_number& operator*=(const rational_number& n) {
		for (const auto& pf : n.prime_factor_list) {
			const auto factor = pf.first;
			const auto exponent = pf.second;
			prime_factor_list[factor] += exponent;
		}
		return *this;
	}

	rational_number operator*(const rational_number& b) const { 
		return rational_number(*this) *= b; 
	}

	rational_number& operator/=(const rational_number& n) {
		for (const auto& pf : n.prime_factor_list) {
			const auto factor = pf.first;
			const auto exponent = pf.second;
			if (prime_factor_list[factor] == exponent)
				prime_factor_list.erase(factor);
			else
				prime_factor_list[factor] -= exponent;
		}
		return *this;
	}

	pair<bigint, bigint> to_bigint() {
		bigint numerator = accumulate(
			prime_factor_list.begin(), 
			prime_factor_list.end(), 
			bigint(1),
			[](bigint acc, pair<long long int, int> pf) {
				const auto factor = pf.first;
				const auto exponent = pf.second;
				return exponent > 0 ? acc * power(factor, exponent) : acc;
			});
		bigint denominator = accumulate(
			prime_factor_list.begin(), 
			prime_factor_list.end(), 
			bigint(1),
			[](bigint acc, pair<int, int> pf) {
				const auto factor = pf.first;
				const auto exponent = pf.second;
				return exponent < 0 ? acc * power(factor, -exponent) : acc;
			});
		return { numerator, denominator };
	}

	std::string to_string() const {
		std::string s;
		bool first_time = true;
		for (auto pf : prime_factor_list) {
			const auto factor = pf.first;
			const auto exponent = pf.second;
			s += (first_time ? "" : " ") 
				+ std::to_string(factor) + "^" + std::to_string(exponent);
			first_time = false;
		}
		return s;
	}

	friend std::ostream& operator<<(std::ostream& os, const rational_number& r) {
		os << r.to_string();
		return os;
	}

private:
	map<long long int, int> prime_factor_list;  // 因数とその冪乗ごとに管理
};

// 組み合わせの数(n C r)を計算する。
bigint combination(int n, int r) {
	if (r == 1)
		return n;

	// 分子  n * (n - 1) * ... * (n - (r - 1))
	rational_number c(1);
	for (int i = 0; i < r; i++)
		c *= n - i;

	// 分母 r * r - 1 * ... * 1 
	for (int i = 2; i <= r; i++)
		c /= i;

	return c.to_bigint().first;
}

// end[組み合わせ] /////////////////////////////////////////////////////////////////

// 「入場待ちの行列」[ACM ICPC 2022 D] を解くためのクラス
class audience_queue {
public:
	// 最初の並び順s、最終的な入場順t、計数のときの基数を用いて初期設定する。
	audience_queue(const vector<int>& s, const vector<int>& t, int b) :
		initial_line(), final_line(), base(b) {
		initial_line = s;
		final_line = t;
	}

	// 分割数がkまでで指定された結果と一致する分け方の総数を求める。
	bigint num_combination(int k) {
		// 分割不可能な場合は先に除く。
		if (!is_valid_final_line() || !check_splittability())
			return 0;

		// 分割しないといけない箇所がk以上あれば解なし。
		const auto num_split = must_split_list.size();
		if (num_split >= k)
			return 0;

		// 分割してもしなくてもいい箇所の個数
		const auto num_splittable 
			= (initial_line.size() - 1) - num_split - dont_split_list.size();

		// 計算する。
		bigint r(1);  // 0箇所分割する場合を初期値とする。
		for (int i = 1; i <= num_splittable && i + num_split < k; i++) {
			// i箇所分割する場合
			r += combination(num_splittable, i);
			if (r > base) {
				// 剰余を求める。
				bigint d = r / base;
				r -= d * base;
			}
		}

		return r;
	}

private:
	vector<int> initial_line;  // ゲート前の順 (座席番号の列)
	vector<int> final_line;  // ゲート後の順 (座席番号の列)
	bigint base;  // 計数のときの基数 (問題で指定)

	set<int> dont_split_list;  // 分割できない箇所 (その要素の手前)
	set<int> must_split_list;  // 分割しないといけない箇所 (その要素の手前)

	int find_on_initial_line(size_t e) const {
		for (size_t i = 0; i < initial_line.size(); i++)
			if (initial_line[i] == e)
				return i;
		return -1;
	}

	// そもそも実現可能か(0以上か)をチェックする。
	// dont_split_listを更新する。
	bool is_valid_final_line() {
		// final_lineの最後の要素から見ていく。
		for (int i = final_line.size() - 1; i > 0; i--) {
			const auto t = final_line[i];
			// 要素tより前にある、tに一番近い、tより大きい値uを見つける。
			// initial_line: ....... u .........
			//                       ^-- p
			// final_line:   ... u ....... t ... (u > t)
			//                   ^-- j     ^-- i
			//                   <-------> offset
			int j;  // uのfinal_line上の位置
			for (j = i - 1; j >= 0; j--)
				if (final_line[j] > t)
					break;
			auto u = (j < 0) ? 0 : final_line[j];  // tより大きい値 
												// (初期値はすべての値より小さい0)

			if (u > 0) {  // 見つかった場合
				// initial_lineのuからの並びとfinal_lineのuからの並びを比較する。

				// initial_lineの中からuを探す。
				int p = find_on_initial_line(u);
				const auto offset = i - j;  // uを含めたtまでの個数(t含まず)

				// initial_lineでのuより後の要素数が足りない場合
				if (p + offset >= initial_line.size())
					return false;  // 不可能

				// 比較する。
				for (int d = 1; j + d <= i; d++) {
					if (initial_line[p + d] != final_line[j + d])
						return false;
					// 分割してはいけないリストに登録する。
					dont_split_list.insert(p + d);
				}

				// 一致していたらuからtまではOK
				// uから次のforループを続けるようiを設定
				i = j + 1;
			}
			// 見つからなかった場合はそのtはOK
		}
		return true;
	}

	// initial_line中の分割してはいけない部分、分割しないといけない部分をすべて見つける。
	// must_split_list, dont_split_listを更新する。
	bool check_splittability() {
		set<int> checked_number_list;  // チェック済みの座席番号リスト(集合)
		for (int i = 0; i < final_line.size(); i++) {
			const int t = final_line[i];  // このループで着目する要素

			// initial_lineの中からtを探す。
			const auto p = find_on_initial_line(t);

			// initial_lineのtが先頭であったり、他の出現済(checked_number_list)の要素
			// (final_lineのiより前)の次なら分割不要
			// 
			// initial_line: ... t ...... u ... s.... (t < u)
			//                   ^-- p    ^-- q ^-- r
			// final_line:   ...... t .... s ........
			//                      ^-- i  ^-- j
			if (p > 0
				&& (checked_number_list.find(initial_line[p - 1]) 
					== checked_number_list.end())) {  // 分割が必要か
				// 位置pでは分割必要とする。
				must_split_list.insert(p);

				// final_lineのiより後にある、tより小さい要素sをすべて見つける。
				for (int j = i + 1; j < final_line.size(); j++) {
					const auto s = final_line[j];
					if (s < t) {
						const auto r = find_on_initial_line(s);

						// initial_line上のsの位置rより前にある、
						// tまたは直近のtより大きい要素uを見つける。
						int q;
						for (q = r - 1; q >= 0; q--)
							if (initial_line[q] >= t)  // initial_line[q] == u
								break;
						if (q < 0)  // 見つからなかった場合
							return false;

						// qからrまでの間は分割不可とする。
						for (int k = q + 1; k <= r; k++)
							dont_split_list.insert(k);
					}
				}
			}

			// tをチェック済みリストに加える。
			checked_number_list.insert(t);
		}
		return true;
	}
};

int main() {
	for (int n, k; (cin >> n >> k) && n > 0 && k > 0; ) {
		vector<int> s(n);
		for (auto& e : s)
			cin >> e;
		vector<int> t(n);
		for (auto& e : t)
			cin >> e;
		audience_queue q(s, t, 998244353);
		cout << q.num_combination(k) << endl;
	}
	return 0;
}
