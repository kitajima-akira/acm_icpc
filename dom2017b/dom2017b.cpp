// dom2017b.cpp - ACM ICPC 2017年国内予選B - ほとんど同じプログラム
// https://icpc.iisf.or.jp/past-icpc/domestic2017/contest/all_ja.html#section_B
// Copyright 2023 Kitajima Akira
// Author: Kitajima Akira <kitajima@osakac.ac.jp>

#include <iostream>
#include <string>
using namespace std;

// 判定結果を表す型
enum class judge_result {IDENTICAL, CLOSE, DIFFERENT};

// 二つの文字列の違いを判定する。
judge_result judge(const string& s1, const string& s2) {
	// 各文字列の先頭から順に確認する。
	bool one_mistake = false;  // 一つ目の違いを見つけたかどうか。
	auto p1 = s1.begin(), p2 = s2.begin();  // 現在確認中の文字の位置。
	for (bool in_string = false; p1 != s1.end() && p2 != s2.end(); ++p1, ++p2) {
		if (*p1 == '"' && *p2 == '"') {  // 文字列の区切りか?
			in_string = !in_string;
			continue;
		}
		if (*p1 != *p2) {  // 違う文字か?
			// 違っていると判定できるか確認する。
			if (!in_string || one_mistake)
				return judge_result::DIFFERENT;

			// 文字列中なので、文字列の終わりまで読み飛ばす。
			one_mistake = true;
			while (*p1 != '"')
				++p1;
			while (*p2 != '"')
				++p2;
			in_string = false;
		}
	}

	// どちらかの文字列が終わってからの判定処理を行う。
	if (p1 != s1.end() || p2 != s2.end())  // もう一方がまだ残っている?
		return judge_result::DIFFERENT;
	return one_mistake ? judge_result::CLOSE : judge_result::IDENTICAL;
}

int main() {
	for (string s1; cin >> s1 && s1 != "."; ) {
		string s2;
		cin >> s2;
		switch (judge(s1, s2)) {
		case judge_result::IDENTICAL:
			cout << "IDENTICAL\n";
			break;
		case judge_result::CLOSE:
			cout << "CLOSE\n";
			break;
		case judge_result::DIFFERENT:
			cout << "DIFFERENT\n";
			break;
		}
	}
	return 0;
}
