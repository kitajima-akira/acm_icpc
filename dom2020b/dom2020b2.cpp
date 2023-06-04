// dom2020b2.cpp - 接触追跡(vector<bool>版)
// https://icpc.iisf.or.jp/past-icpc/domestic2020/contest/all_ja.html#section_B
// Copyright 2023 Kitajima Akira
// Author: Kitajima Akira <kitajima@osakac.ac.jp>
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

class infection_tracking {
public:
	infection_tracking(int m): 
		infected_table(m, false) {}

	void set_infected(int p) {
		infected_table[p] = true;
	}

	bool is_infected(int p) {
		return infected_table[p];
	}

	void update(int a, int b) {
		// aとbのどちらかが感染者なら、反対も登録
		if (is_infected(a))
			set_infected(b);
		else if (is_infected(b))
			set_infected(a);
	}

	int get_num_infected() {
		return count(infected_table.begin(), infected_table.end(), true);
	}

private:
	vector<bool> infected_table;  // 感染者のリスト

};

int main() {
	for (int m, n, p;  // m 利用者の人数, n 記録の件数, p 感染が確認された利用者のID 
		(cin >> m >> n >> p) && (m != 0 || n != 0 || p != 0); ) {
		infection_tracking it(m);  // 追跡データ
		// 感染者の登録
		it.set_infected(p - 1);  // 番号pは1から始まるので一つずらす。

		// データの読み出しと登録
		for (int i = 0; i < n; i++) {
			int a, b;  // 濃厚接触者の利用者ID
			cin >> a >> b;
			it.update(a - 1, b - 1);
		}

		// 感染者数の出力
		cout << it.get_num_infected() << endl;
	}
	return 0;
}
