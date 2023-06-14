// dom2018b.cpp - ACM ICPC 2018年国内予選B - 折り紙
// https://icpc.iisf.or.jp/past-icpc/domestic2018/contest/all_ja.html#section_B
// Copyright 2023 Kitajima Akira
// Author: Kitajima Akira <kitajima@osakac.ac.jp>

#include <iostream>
#include <vector>
using namespace std;

// 「折り紙」を解くクラス
class origami {
public:
	origami(int n, int m) :
		// 折り返す際の途中処理ではみ出すことも考慮して2倍の大きさにする。
		paper(2 * m, vector<int>(2 * n, 0)),  
		current_width(n),
		current_height(m) {
		// 各領域の枚数を設定する。
		for (int j = 0; j < current_height; j++)
			for (int i = 0; i < current_width; i++)
				paper[j][i] = 1;
	}

	// 折る処理を行う。
	void fold(int d, int c) {
		if (d == 1)
			fold_vertically(c);
		else
			fold_horizontally(c);
	}

	// (穴の開く)枚数を返す。
	int punch(int x, int y) const {
		return paper[y][x];
	}

	// デバッグ用に、折った様子を出力する。
	friend std::ostream& operator<<(std::ostream& os, const origami& o) {
		for (auto line : o.paper) {
			os << ": ";
			for (int piece : line)
				os << " " << piece;
			os << endl;
		}
		return os;
	}

private:
	vector<vector<int>> paper;  // 各領域の枚数
	int current_width, current_height;  // 現在の幅、高さ
	
	// 縦に折る。
	void fold_vertically(int c) {
		for (auto& line : paper) {
			// 折った先のデータに折った部分のデータを加える。
			for (int i = 0; i < c; i++) {
				const int ii = c + (c - i) - 1;  // i番目の行と重なる行
				line[ii] += line[i];
			}
			// 折った部分だけシフトする。
			for (int i = 0; i < line.size() - c; i++)
				line[i] = line[i + c];
		}

		current_width = 2 * c <= current_width ? current_width - c : c;
		// なくなった部分を0で埋める。
		for (auto& line : paper)
			for (int i = current_width; i < line.size(); i++)
				line[i] = 0;
	}

	// 横に折る。
	void fold_horizontally(int c) {
		for (int i = 0; i < c; i++) {
			const int ii = c + (c - i) - 1;  // i番目の行と重なる行
			// 折った先のデータに折った部分のデータを加える。
			for (int j = 0; j < paper[i].size(); j++)
				paper[ii][j] += paper[i][j];
		}

		// 折った部分だけシフトする。
		for (int i = 0; i < paper.size() - c; i++)
			paper[i] = paper[i + c];

		current_height = 2 * c <= current_height ? current_height - c : c;
		// なくなった部分を0で埋める。
		for (int j = current_height; j < paper.size(); j++)
			for (auto& piece : paper[j])
				piece = 0;
	}
};

int main() {
	for (int n, m, t, p; cin >> n >> m >> t >> p && (n > 0 || m > 0 || t >> 0 || p > 0);) {
		// 折り紙データを用意する。
		origami o(n, m);
		
		// 入力の指定どおりに折る。
		for (int i = 0; i < t; i++) {
			int d, c;
			cin >> d >> c;
			o.fold(d, c);
		}

		// 入力の指定どおりに穴を開ける。
		int num_holes = 0;
		for (int i = 0; i < p; i++) {
			int x, y;
			cin >> x >> y;
			// 結果を出力する。
			cout << o.punch(x, y) << endl;
		}
	}

	return 0;
}
