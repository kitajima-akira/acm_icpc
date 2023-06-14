// dom2018a.cpp - ACM ICPC 2018�N�����\�IA - �����i��
// https://icpc.iisf.or.jp/past-icpc/domestic2018/contest/all_ja.html#section_A
// Copyright 2023 Kitajima Akira
// Author: Kitajima Akira <kitajima@osakac.ac.jp>

#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

// �����f�[�^1�Z�b�g�ɑ΂��ĉ��𓾂�B
int get_answer(const vector<int>& incomes) {
	// ���v�l���v�Z����B
	int sum = accumulate(incomes.begin(), incomes.end(), 0);  // �����̍��v�l
	// �����̕��ϒl���v�Z����B
	double average = static_cast<double>(sum) / incomes.size();
	// ���ψȏ�̐l�����v�Z����B
	return count_if(incomes.begin(), incomes.end(), [&](int a) {return a <= average; });
}

int main() {
	for (int n; cin >> n && n > 0; ) {
		// ���͂𓾂�B
		vector<int> incomes(n);  // �����̃��X�g
		for (auto& a : incomes)
			cin >> a;

		// �����𖞂����l���𐔂��ďo�͂���B
		cout << get_answer(incomes) << endl;
	}
	return 0;
}
