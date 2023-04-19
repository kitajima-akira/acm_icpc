/// @file dom2011b.cpp @brief 世界の天秤
/// @author Kitajima Akira
/// @date 2011/07/05
// 20min
#include <iostream>
#include <string>
#include <stack>
using namespace std;


// sitから始まる部分文字列が正しく対応しているか判定する。
bool is_balanced_paren_recursive(const string& s, string::const_iterator& s_it, const string& parens) {
	// sitが指す文字をチェック
	const size_t open_pos = parens.find(*s_it);
	if (open_pos == string::npos)   // 括弧以外の文字
		return true;
	else if (open_pos % 2 == 1)  // 閉じ括弧
		return false;

	// 開き括弧
	// 次の文字以降の対応する閉じ括弧を見つける。
	for (++s_it; s_it != s.end(); ++s_it) {
		const size_t pos = parens.find(*s_it);
		if (pos == string::npos)   // 括弧以外の文字
			continue;

		if (pos % 2 == 1)  // 閉じ括弧
			return (open_pos + 1 == pos);   // 対応しているかどうか

		// 開き括弧
		if (!is_balanced_paren_recursive(s, s_it, parens))
			return false;
		// sitは対応する閉じ括弧まで移動している。
		// 残りの文字は次のループで調べる。
	}
	// 対応が見つからずに終了
	return false;
}


bool match_parens2(const string& s, const string& parens) {
	for (auto s_it = s.begin(); s_it != s.end(); ++s_it)
		if (!is_balanced_paren_recursive(s, s_it, parens))
			return false;
	return true;
}

/// 文字列中の括弧が対応しているかチェックする。
/// @param s 対象文字列
/// @param parens チェック対象の括弧
///               (対応する開き括弧と閉じ括弧が順に並ぶ。)
/// @returns うまく対応していたら真となる。

bool match_parens(const string& s, const string& parens) {
	stack<char> st;
	for (const auto& c : s) {
		size_t pos = parens.find(c);
		if (pos != string::npos) {
			if (pos % 2 == 0) {
				// 開き括弧
				st.push(c);
			}
			else {
				// 閉じ括弧
				if (st.empty() || st.top() != parens[pos - 1])
					return false;
				st.pop();
			}
		}
	}
	return st.empty();
}


int main() {
	for (string s; getline(cin, s), s != ".";) {
		cout << (match_parens(s, "()[]") ? "yes" : "no") << endl;
		cout << (match_parens2(s, "()[]") ? "yes" : "no") << endl;
	}
return 0;
}
