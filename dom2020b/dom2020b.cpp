// dom2020b.cpp - �ڐG�ǐ�
// https://icpc.iisf.or.jp/past-icpc/domestic2020/contest/all_ja.html#section_B
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

// a���o�^����Ă�����b��(���o�^�̏ꍇ)�o�^����B
void check_and_add(int a, int b, vector<int>& ilist) {
	if (find(ilist.begin(), ilist.end(), a) != ilist.end()) {
		// a�����łɓo�^�ς�
		if (find(ilist.begin(), ilist.end(), b) == ilist.end())
			// b�͓o�^����Ă��Ȃ�
			ilist.push_back(b);
	}
}

// a��b�̂ǂ��炩�������҂Ȃ�A���΂��o�^
void update(int a, int b, vector<int>& infected_list) {
	check_and_add(a, b, infected_list);
	check_and_add(b, a, infected_list);
}

int main() {
	for (;;) {
		int m;  // ���p�҂̐l��
		int n;  // �L�^�̌���
		int p;  // �������m�F���ꂽ���p�҂�ID

		cin >> m >> n >> p;

		if (m == 0 && n == 0 && p == 0)
			break;

		vector<int> infected_list;  // �����҂̃��X�g

		// �����҂�o�^
		infected_list.push_back(p);

		for (int i = 0; i < n; i++) {
			int a, b;  // �Z���ڐG�҂̗��p��ID
			cin >> a >> b;

			// a��b�̂ǂ��炩�������҂Ȃ�A���΂��o�^
			update(a, b, infected_list);
		}

		// �����Ґ����o��
		cout << infected_list.size() << endl;
	}
	return 0;
}
