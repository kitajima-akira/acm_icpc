// dom2021d3.cpp - 風船配り
// https://icpc.iisf.or.jp/past-icpc/domestic2021/contest/all_ja.html#section_D
// 動的計画法で解く (改良版)
// C++17 構造化束縛
// https://icpc.iisf.or.jp/past-icpc/domestic2021/commentaries.html#D
// Copyright 2023 Kitajima Akira
// Author: Kitajima Akira <kitajima@osakac.ac.jp>

#include <algorithm>
#include <iostream>
#include <tuple>
#include <vector>
using namespace std;

// 3つ組を表す型
using part3 = tuple<int, int, int>;

// 3つ組tを昇順に並べ替える。
// 並べ替えた3つ組を返す。
part3 sort(const part3& t) {
	const auto& [a, b, c] = t;
	if (a <= b && a <= c)
		return (b <= c) ? part3{ a, b, c } : part3{ a, c, b };

	else if (b <= a && b <= c)
		return (a <= c) ? part3{ b, a, c } : part3{ b, c, a };

	return (a <= b) ? part3{ c, a, b } : part3{ c, b, a };
}

// 新しい要素biをもとにcandidateを更新する。
void update_candidate(vector<part3>& candidate, int bi) {
	vector<part3> new_candidate;  // 新しい候補

	// 新しい候補を追加
	for (const auto& t : candidate) {
		const auto& [a, b, c] = t;
		// 各候補は、重複をなくすため、並ベ替えておく。
		new_candidate.push_back(sort({ a + bi, b, c }));
		new_candidate.push_back(sort({ a, b + bi, c }));
		new_candidate.push_back(sort({ a, b, c + bi }));
	}

	// 重複部分を削除
	sort(new_candidate.begin(), new_candidate.end());
	auto uniq_end = unique(new_candidate.begin(), new_candidate.end());
	new_candidate.erase(uniq_end, new_candidate.end());

	// 更新
	candidate = new_candidate;
}

// 新しい要素biをもとにcandidateを更新する。(候補削減なし)
// どれだけ効率が違うか。
void update_candidate_basic(vector<part3>& candidate, int bi) {
	vector<part3> new_candidate;  // 新しい候補

	// 新しい候補を追加
	for (const auto& t : candidate) {
		const auto& [a, b, c] = t;

		new_candidate.push_back({ a + bi, b, c });
		new_candidate.push_back({ a, b + bi, c });
		new_candidate.push_back({ a, b, c + bi });
	}

	// 更新
	candidate = new_candidate;
}

// 与えられる子供の最大数を求める。
int get_max_number_of_kids(vector<int> b) {
	vector<part3> candidate = { { 0, 0, 0 } };  // 分割ごとの風船の総和

	// bを一つずつ追加して、候補を更新する。
	for (const auto& i : b)
		update_candidate(candidate, i);

	// 和の最小値の最大値を求める。
	int k_max = 0;  // 求める最大値
	for (const auto& t : candidate)
		// t = [a, b, c]のとき、ソート済みなのでa <= b <= cだからaが最小値
		k_max = max(k_max, get<0>(t));

	return k_max;
}

int main() {
	for (int n; (cin >> n) && (n > 0); ) {
		vector<int> b(n);
		for (auto& i : b)
			cin >> i;
		cout << get_max_number_of_kids(b) << endl;
	}
	return 0;
}
