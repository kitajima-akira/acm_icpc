// dom2020b2.cpp - ÚG’ÇÕ(vector<bool>”Å)
// https://icpc.iisf.or.jp/past-icpc/domestic2020/contest/all_ja.html#section_B
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
		// a‚Æb‚Ì‚Ç‚¿‚ç‚©‚ªŠ´õÒ‚È‚çA”½‘Î‚à“o˜^
		if (is_infected(a))
			set_infected(b);
		else if (is_infected(b))
			set_infected(a);
	}

	int get_num_infected() {
		return count(infected_table.begin(), infected_table.end(), true);
	}

private:
	vector<bool> infected_table;  // Š´õÒ‚ÌƒŠƒXƒg

};

int main() {
	for (;;) {
		int m;  // —˜—pÒ‚Ìl”
		int n;  // ‹L˜^‚ÌŒ”
		int p;  // Š´õ‚ªŠm”F‚³‚ê‚½—˜—pÒ‚ÌID

		cin >> m >> n >> p;

		if (m == 0 && n == 0 && p == 0)
			break;

		infection_tracking it(m);
		// Š´õÒ‚ğ“o˜^
		it.set_infected(p - 1);  // ”Ô†p‚Í1‚©‚çn‚Ü‚é‚Ì‚Åˆê‚Â‚¸‚ç‚·B

		for (int i = 0; i < n; i++) {
			int a, b;  // ”ZŒúÚGÒ‚Ì—˜—pÒID
			cin >> a >> b;
			it.update(a - 1, b - 1);
		}

		// Š´õÒ”‚ğo—Í
		cout << it.get_num_infected() << endl;
	}
	return 0;
}
