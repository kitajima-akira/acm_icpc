// dom2020b2.cpp - 接触追跡(vector<bool>版)
// https://icpc.iisf.or.jp/past-icpc/domestic2020/contest/all_ja.html#section_B
// Copyright 2023 Kitajima Akira
// Author: Kitajima Akira <kitajima@osakac.ac.jp>
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

// 接触追跡を解くクラス
class infection_tracking {
public:
	// コンストラクタ
	// 利用者m人、感染者ID idで初期設定する。
	infection_tracking(int m, int id): 
		infected_table(m, false) {
		set_infected(id);
	}

	// IDを感染者として登録する。
	void set_infected(int id) {
		infected_table[id - 1] = true;  // IDは1からなので、インデックスは1減らす。
	}

	// IDが感染者として登録されているか。
	bool is_infected(int id) const {
		return infected_table[id - 1];  // IDは1からなので、インデックスは1減らす。
	}

	// aとbが濃厚接触としてデータを更新する。
	void update(int a, int b) {
		// aとbのどちらかが感染者なら、反対も登録
		if (is_infected(a))
			set_infected(b);
		else if (is_infected(b))
			set_infected(a);
	}

	// 感染者数を返す。
	int get_num_infected() const {
		return count(infected_table.begin(), infected_table.end(), true);
	}

private:
	vector<bool> infected_table;  // 感染者のリスト

};

int main() {
	for (int m, n, p;  // m 利用者の人数, n 記録の件数, p 感染が確認された利用者のID 
		(cin >> m >> n >> p) && (m != 0 || n != 0 || p != 0); ) {
		// 感染者の登録
		infection_tracking it(m, p);  // 追跡データ

		// データの読み出しと登録
		for (int i = 0; i < n; i++) {
			int a, b;  // 濃厚接触者の利用者ID
			cin >> a >> b;
			it.update(a, b);
		}

		// 感染者数の出力
		cout << it.get_num_infected() << endl;
	}
	return 0;
}
