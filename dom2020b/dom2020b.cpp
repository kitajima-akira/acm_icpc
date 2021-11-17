// dom2020b.cpp - ÚG’ÇÕ
// https://icpc.iisf.or.jp/past-icpc/domestic2020/contest/all_ja.html#section_B
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

// a‚ª“o˜^‚³‚ê‚Ä‚¢‚½‚çb‚à(–¢“o˜^‚Ìê‡)“o˜^‚·‚éB
void check_and_add(int a, int b, vector<int>& ilist) {
	if (find(ilist.begin(), ilist.end(), a) != ilist.end()) {
		// a‚ª‚·‚Å‚É“o˜^Ï‚İ
		if (find(ilist.begin(), ilist.end(), b) == ilist.end())
			// b‚Í“o˜^‚³‚ê‚Ä‚¢‚È‚¢
			ilist.push_back(b);
	}
}

// a‚Æb‚Ì‚Ç‚¿‚ç‚©‚ªŠ´õÒ‚È‚çA”½‘Î‚à“o˜^
void update(int a, int b, vector<int>& infected_list) {
	check_and_add(a, b, infected_list);
	check_and_add(b, a, infected_list);
}

int main() {
	for (;;) {
		int m;  // —˜—pÒ‚Ìl”
		int n;  // ‹L˜^‚ÌŒ”
		int p;  // Š´õ‚ªŠm”F‚³‚ê‚½—˜—pÒ‚ÌID

		cin >> m >> n >> p;

		if (m == 0 && n == 0 && p == 0)
			break;

		vector<int> infected_list;  // Š´õÒ‚ÌƒŠƒXƒg

		// Š´õÒ‚ğ“o˜^
		infected_list.push_back(p);

		for (int i = 0; i < n; i++) {
			int a, b;  // ”ZŒúÚGÒ‚Ì—˜—pÒID
			cin >> a >> b;

			// a‚Æb‚Ì‚Ç‚¿‚ç‚©‚ªŠ´õÒ‚È‚çA”½‘Î‚à“o˜^
			update(a, b, infected_list);
		}

		// Š´õÒ”‚ğo—Í
		cout << infected_list.size() << endl;
	}
	return 0;
}
