// dom2020b.cpp - 接触追跡
// https://icpc.iisf.or.jp/past-icpc/domestic2020/contest/all_ja.html#section_B
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

// aが登録されていたらbも(未登録の場合)登録する。
void check_and_add(int a, int b, vector<int>& ilist) {
	if (find(ilist.begin(), ilist.end(), a) != ilist.end()) {
		// aがすでに登録済み
		if (find(ilist.begin(), ilist.end(), b) == ilist.end())
			// bは登録されていない
			ilist.push_back(b);
	}
}

// aとbのどちらかが感染者なら、反対も登録
void update(int a, int b, vector<int>& infected_list) {
	check_and_add(a, b, infected_list);
	check_and_add(b, a, infected_list);
}

int main() {
	for (;;) {
		int m;  // 利用者の人数
		int n;  // 記録の件数
		int p;  // 感染が確認された利用者のID

		cin >> m >> n >> p;

		if (m == 0 && n == 0 && p == 0)
			break;

		vector<int> infected_list;  // 感染者のリスト

		// 感染者を登録
		infected_list.push_back(p);

		for (int i = 0; i < n; i++) {
			int a, b;  // 濃厚接触者の利用者ID
			cin >> a >> b;

			// aとbのどちらかが感染者なら、反対も登録
			update(a, b, infected_list);
		}

		// 感染者数を出力
		cout << infected_list.size() << endl;
	}
	return 0;
}
