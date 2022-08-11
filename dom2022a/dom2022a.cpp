// dom2022a.cpp - ACM ICPC 2022 �����\�IA �����s�[�N�̉�

#include <iostream>
using namespace std;

int main() {
	for (int n; cin >> n && n > 0;) {  // n�̓f�[�^�̌�
		int v;  // �ǂݍ��񂾃f�[�^
		cin >> v;
		int prevprev = v;  // 2��O�̃f�[�^
		cin >> v;
		int prev = v;  // 1��O�̃f�[�^
		int npeak = 0;  // �s�[�N�̉�
		for (int i = 0; i < n - 2; i++) {  // �擪�̓�������ČJ��Ԃ��B
			// �f�[�^����ǂݍ��݁A��O���s�[�N���������m�F����B
			cin >> v;
			if (prevprev < prev && v < prev)
				npeak++;

			// �f�[�^���X�V����B
			prevprev = prev;
			prev = v;
		}
		cout << npeak << endl;
	}
	return 0;
}
