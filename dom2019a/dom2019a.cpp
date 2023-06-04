// dom2019a.cpp - ACM ICPC 2019�N�����\�IA ���������̐���
// https://icpc.iisf.or.jp/past-icpc/domestic2019/problems/ja/contest/all_ja.html#section_A
// Copyright 2023 Kitajima Akira
// Author: Kitajima Akira <kitajima@osakac.ac.jp>
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
	for (int n, m; cin >> n >> m && !((n == 0) && (m == 0)); ) {// n �l��, m �Ȗڐ�

		vector<int> sum(n, 0);  // n�l���̍��v�l (�����l0)

		// �e�l�̍��v�l�����߂�B
		for (int j = 0; j < m; j++) {
			// �Ȗڂ��Ƃ̏���
			for (auto& s : sum) {
				// �l���Ƃ̏���
				int p;  // ���_�f�[�^
				cin >> p;
				s += p;  // ���v�l�ɓ��_��ǉ�����B
			}
		}

		// �ő�l��������B
		auto it = max_element(sum.begin(), sum.end());

		// 1�ԑ傫���l���o�͂���B
		cout << *it << endl;
	}
	return 0;
}
