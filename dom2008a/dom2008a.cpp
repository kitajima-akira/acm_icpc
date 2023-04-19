/** @file dom2008a.cpp @brief Equal Total Scores
 *  @author Kitajima Akira
 *  @date 2011/05/16
 *  @see https://icpc.iisf.or.jp/past-icpc/domestic2008/problems/all_ja.html
 *  解答時間の目安 40分
 */

#include <iostream>
#include <set>
#include <algorithm>
using namespace std;

/** 解答を求める。 */
class solver {
public:
    /** 
	 * 二つのカード枚数で初期化する。
     * @param n 1人目のカードの枚数
     * @param m 2人目のカードの枚数
	 */
    solver (int n, int m): diff(0) {}

    /**
	 * 太郎のカードを一つ指定する。
	 * 花子との差も計算しておく。
     * @param カードの番号
	 */
    void add2Taro(int datum) {
		taro.insert(datum);
		diff += datum;
    }

    /** 
	 * 花子のカードを一つ指定する。
	 * 太郎との差も計算しておく。
     * @param カードの番号
	 */
    void add2Hanako(int datum) {
		hanako.insert(datum);
		diff -= datum;
    }

    /**
	 * 解を求める。
	 * 求めた解はgetTaro2Hanako()とgetHanako2Taro()で得られる。
	 * @returns 解が求まったかどうか。
	 * @see solver::getTaro2Hanako(), solver::getHanako2Taro()
	 */
    bool solve() {
		if (diff % 2)
			// 差が奇数なら解が求まらない。
			return false;

		int a = diff / 2;
		// 和が最小となるよう太郎の小さい値から順に調べる。
		for (auto t : taro) {
			hanako2taro = t - a; // 交換するべき値は決まっている。
			if (hanako.find(hanako2taro) != hanako.end()) {
				// 花子が交換するべき値を持っていた。
				taro2hanako = t;
				return true;
			}
		}
		return false;
    }

    /** 
	 * 太郎から花子に渡すカードの値を得る。
     * @returns カードの値
	 */
    int getTaro2Hanako() const {
		return taro2hanako;
    }

    /**
	 * 花子から太郎に渡すカードの値を得る。
     * @returns カードの値
	 */
    int getHanako2Taro() const {
		return hanako2taro;
    }

private:
    using cards = set<int>;
    using cards_cit = set<int>::const_iterator;
    cards taro, hanako;		// 太郎の札, 花子の札
    int diff;			// 値の差
    int taro2hanako, hanako2taro; // 太郎から花子に渡す札、その逆
};

int main() {
    for (int n, m; cin >> n >> m, n || m;) {
		solver s(n, m);
		int datum;
		while (n--) {
			cin >> datum;
			s.add2Taro(datum);
		}
		while (m--) {
			cin >> datum;
			s.add2Hanako(datum);
		}

		if (s.solve())
			cout << s.getTaro2Hanako() << ' ' << s.getHanako2Taro() << endl;
		else
			cout << -1 << endl;
    }

    return 0;
}
