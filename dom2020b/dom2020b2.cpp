// dom2020b2.cpp - �ڐG�ǐ�(vector<bool>��)
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
		// a��b�̂ǂ��炩�������҂Ȃ�A���΂��o�^
		if (is_infected(a))
			set_infected(b);
		else if (is_infected(b))
			set_infected(a);
	}

	int get_num_infected() {
		return count(infected_table.begin(), infected_table.end(), true);
	}

private:
	vector<bool> infected_table;  // �����҂̃��X�g

};

int main() {
	for (int m, n, p;  // m ���p�҂̐l��, n �L�^�̌���, p �������m�F���ꂽ���p�҂�ID 
		(cin >> m >> n >> p) && (m != 0 || n != 0 || p != 0); ) {
		infection_tracking it(m);  // �ǐՃf�[�^
		// �����҂̓o�^
		it.set_infected(p - 1);  // �ԍ�p��1����n�܂�̂ň���炷�B

		// �f�[�^�̓ǂݏo���Ɠo�^
		for (int i = 0; i < n; i++) {
			int a, b;  // �Z���ڐG�҂̗��p��ID
			cin >> a >> b;
			it.update(a - 1, b - 1);
		}

		// �����Ґ��̏o��
		cout << it.get_num_infected() << endl;
	}
	return 0;
}
