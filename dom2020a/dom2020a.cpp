// dom2020a.cpp - カウントアップ2020
// https://icpc.iisf.or.jp/past-icpc/domestic2020/contest/all_ja.html#section_A

#include <iostream>
#include <vector>
using namespace std;

int main() {
	for (;;) {
		// データセット一つ分の入力

		int n;  // 整数の個数
		cin >> n;

		if (n == 0)
			break;

		vector<int> d(n);  // 整数の並び
		for (int& i : d) {
			cin >> i;
		}

		int c = 0;  // 回数
		// データチェック
		// 一つずつずらして確認 (最後の3つはチェック不要)
		for (size_t i = 0; i < d.size() - 3; i++) {
			// i番目から始まるデータのチェック
			if (d[i] == 2 && d[i + 1] == 0
				&& d[i + 2] == 2 && d[i + 3] == 0) {
				c++;
			}
		}
		cout << c << endl;
	}
	return 0;
}