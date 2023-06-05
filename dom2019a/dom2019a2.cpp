// dom2019a2.cpp - ACM ICPC 2019�N�����\�IA ���������̐���
// https://icpc.iisf.or.jp/past-icpc/domestic2019/problems/ja/contest/all_ja.html#section_A
// Copyright 2023 Kitajima Akira
// Author: Kitajima Akira <kitajima@osakac.ac.jp>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

int main() {
	for (int n, m; cin >> n >> m && !((n == 0) && (m == 0)); ) {// n �l��, m �Ȗڐ�
		// �f�[�^�𓾂�B
		vector<vector<int>> table(m, vector<int>(n));  // �Ȗڂ��Ƃ̐��k�̓��_
		for (auto& subject_data : table)
			for (auto& p : subject_data)
				cin >> p;

		// ���k���Ƃ̘a�����߂�B
		vector<int> sum(n, 0);  // ���k���Ƃ̍��v�_
		for (const auto& subject_data : table)
			for (int i = 0; i < n; i++)
					sum[i] += subject_data[i];

		// 1�ԑ傫���l���o�͂���B
		cout << *max_element(sum.begin(), sum.end()) << endl;
	}
	return 0;
}
