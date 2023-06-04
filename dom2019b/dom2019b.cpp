/** @file dom2019b.cpp @brief Problem B スクリーンキーボード を解くプログラム
 *
 * @see https://icpc.iisf.or.jp/past-icpc/domestic2019/problems/ja/contest/all_ja.html#section_B
 *
 * Copyright 2019 Akira Kitajima
 *
 * @author 北嶋 暁 <kitajima@osakac.ac.jp>
 * @date 2019/11/26 新規作成
 */
#include <iostream>
#include <string>
#include <utility>
#include <vector>
using namespace std;

/** キーボード上の文字の位置を得る。
*  @param keyboard キーボード
*  @param c 探す文字
*  @return 文字の位置の座標 (左上が(0, 0))
*/
pair<int, int> key_position(const vector<string>& keyboard, char c) {
	int x = -1, y = -1;  // 求める位置の座標

	// 1行ずつ順に調べる。
	for (int j = 0; j < keyboard.size(); j++)
		if ((x = keyboard[j].find_first_of(c)) != string::npos) {
			y = j;
			break;
		}
	return { x, y };
}

/** (x0, y0)から(x1, y1)までのマンハッタン距離を得る。
 * @param x0 座標
 * @param y0 座標
 * @param x1 座標
 * @param y1 座標
 * @return マンハッタン距離
 */
int Manhattan_distance(int x0, int y0, int x1, int y1) {
	return abs(x1 - x0) + abs(y1 - y0);
}

/** キーボードで何回入力したら文字列@a s が入力できるのか最小値を得る。
 *  @param keyboard キーボード
 *  @param s 判定する文字列
 *  @return 最小値
 */
int min_key_count(const vector<string>& keyboard, const string& s) {
	int min_length = 0;  // 最小値
	int x = 0, y = 0;  // 現在の位置
	for (char c : s) {
		pair<int, int> pos = key_position(keyboard, c);  // 現在の字の位置

		// 現在の字を入力するのに必要な回数を求める。
		min_length += Manhattan_distance(x, y, pos.first, pos.second) + 1;

		// 現在の位置の更新
		x = pos.first;
		y = pos.second;
	}
	return min_length;  // (仮)
}

int main() {
	for (;;) {
		// スクリーンキーボードの大きさを得る。
		int h, w;  // スクリーンキーボードの高さ、幅
		cin >> h >> w;

		// 終了判定する。
		if (h == 0 && w == 0)
			break;

		// キーボードを読み込む。
		vector<string> keyboard;
		for (int i = 0; i < h; i++) {
			string r;
			cin >> r;
			keyboard.push_back(r);
		}

		// 判定に用いる文字列を得る。
		string s;
		cin >> s;

		// 計算し、結果を出力する。
		cout << min_key_count(keyboard, s) << endl;
	}
	return 0;
}