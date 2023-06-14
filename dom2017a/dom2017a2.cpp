/** @file dom2017a2.cpp @brief ACM ICPC 2017年国内予選A 太郎君の買物
 * @author Kitajima Akira <kitajima@osakac.ac.jp>
 * @copyright Copyright 2017 Kitajima Akira
 * @see http://icpc.iisf.or.jp/past-icpc/domestic2017/contest/all_ja.html#section_A
 */
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

using price_list_t = vector<int>;

/** 
 * 表plの中から、金額mを超えない(異なる)二つを選ぶ。
 * @param pl 値段の表
 * @param m  金額の上限
 * @return 合計値 (0なら見つからなかった)
 */
int choice(const price_list_t& pl, int m) {
	// 表を並べ替える。
	price_list_t list = pl;  // 並べ替え用の変数
	sort(list.begin(), list.end());  // 値段の表を安い順に並べ替える。

	// 二つのうち、安い方をi、高い方をjとして、順に合計値がmを超えない範囲で、合計値の最大値を見つける。
	int total_max = 0;  // 現在のm以下で最大の合計値
						// (初期値を見つからなかったときの値0とする。)
	for (size_t i = 0; (i < list.size() - 1) 
						&& (list[i] + list[i + 1] <= m); i++) { // 一つ目としてiを選ぶ。
		int total;		// 合計金額
		for (size_t j = i + 1; (j < list.size())
						&& ((total = list[i] + list[j]) <= m); j++) {  // 二つ目としてjを選ぶ。
			total_max = max(total_max, total);
		}
	}
	return total_max;
}

int main() {
	for (int n, m;  // 品物の個数n, 使ってよい最大の金額m
		cin >> n >> m && (n > 0 || m > 0);) {
		// 値段を読み取る。
		price_list_t price_list(n);  // 値段表
		for (auto& a : price_list)
			cin >> a;

		// 品物を選ぶ。
		int total = choice(price_list, m);

		// 結果を出力する。
		if (total != 0)
			cout << total << endl;
		else
			cout << "NONE" << endl;
	}
	return 0;
}
