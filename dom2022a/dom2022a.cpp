// dom2022a.cpp - ACM ICPC 2022 国内予選A 感染ピークの回数

#include <iostream>
using namespace std;

int main() {
	for (int n; cin >> n && n > 0;) {  // nはデータの個数
		int v;  // 読み込んだデータ
		cin >> v;
		int prevprev = v;  // 2回前のデータ
		cin >> v;
		int prev = v;  // 1回前のデータ
		int npeak = 0;  // ピークの回数
		for (int i = 0; i < n - 2; i++) {  // 先頭の二つを除いて繰り返す。
			// データを一つ読み込み、一つ前がピークだったか確認する。
			cin >> v;
			if (prevprev < prev && v < prev)
				npeak++;

			// データを更新する。
			prevprev = prev;
			prev = v;
		}
		cout << npeak << endl;
	}
	return 0;
}
