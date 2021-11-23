// dom2020a.cpp - �J�E���g�A�b�v2020
// https://icpc.iisf.or.jp/past-icpc/domestic2020/contest/all_ja.html#section_A

#include <iostream>
#include <vector>
using namespace std;

int main() {
	for (;;) {
		// �f�[�^�Z�b�g����̓���

		int n;  // �����̌�
		cin >> n;

		if (n == 0)
			break;

		vector<int> d(n);  // �����̕���
		for (int& i : d) {
			cin >> i;
		}

		int c = 0;  // ��
		// �f�[�^�`�F�b�N
		// ������炵�Ċm�F (�Ō��3�̓`�F�b�N�s�v)
		for (size_t i = 0; i < d.size() - 3; i++) {
			// i�Ԗڂ���n�܂�f�[�^�̃`�F�b�N
			if (d[i] == 2 && d[i + 1] == 0
				&& d[i + 2] == 2 && d[i + 3] == 0) {
				c++;
			}
		}
		cout << c << endl;
	}
	return 0;
}