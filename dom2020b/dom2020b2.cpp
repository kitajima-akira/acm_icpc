// dom2020b2.cpp - �ڐG�ǐ�(vector<bool>��)
// https://icpc.iisf.or.jp/past-icpc/domestic2020/contest/all_ja.html#section_B
// Copyright 2023 Kitajima Akira
// Author: Kitajima Akira <kitajima@osakac.ac.jp>
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

// �ڐG�ǐՂ������N���X
class infection_tracking {
public:
	// �R���X�g���N�^
	// ���p��m�l�A������ID id�ŏ����ݒ肷��B
	infection_tracking(int m, int id): 
		infected_table(m, false) {
		set_infected(id);
	}

	// ID�������҂Ƃ��ēo�^����B
	void set_infected(int id) {
		infected_table[id - 1] = true;  // ID��1����Ȃ̂ŁA�C���f�b�N�X��1���炷�B
	}

	// ID�������҂Ƃ��ēo�^����Ă��邩�B
	bool is_infected(int id) const {
		return infected_table[id - 1];  // ID��1����Ȃ̂ŁA�C���f�b�N�X��1���炷�B
	}

	// a��b���Z���ڐG�Ƃ��ăf�[�^���X�V����B
	void update(int a, int b) {
		// a��b�̂ǂ��炩�������҂Ȃ�A���΂��o�^
		if (is_infected(a))
			set_infected(b);
		else if (is_infected(b))
			set_infected(a);
	}

	// �����Ґ���Ԃ��B
	int get_num_infected() const {
		return count(infected_table.begin(), infected_table.end(), true);
	}

private:
	vector<bool> infected_table;  // �����҂̃��X�g

};

int main() {
	for (int m, n, p;  // m ���p�҂̐l��, n �L�^�̌���, p �������m�F���ꂽ���p�҂�ID 
		(cin >> m >> n >> p) && (m != 0 || n != 0 || p != 0); ) {
		// �����҂̓o�^
		infection_tracking it(m, p);  // �ǐՃf�[�^

		// �f�[�^�̓ǂݏo���Ɠo�^
		for (int i = 0; i < n; i++) {
			int a, b;  // �Z���ڐG�҂̗��p��ID
			cin >> a >> b;
			it.update(a, b);
		}

		// �����Ґ��̏o��
		cout << it.get_num_infected() << endl;
	}
	return 0;
}
