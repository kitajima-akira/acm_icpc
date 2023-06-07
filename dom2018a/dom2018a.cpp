// dom2018a.cpp - ACM ICPC 2018�N�����\�IA - �����i��
// https://icpc.iisf.or.jp/past-icpc/domestic2018/contest/all_ja.html#section_A
// Copyright 2023 Kitajima Akira
// Author: Kitajima Akira <kitajima@osakac.ac.jp>

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
	for (int n; cin >> n && n > 0; ) {
		// ���͂𓾂āA���v�l���v�Z���Ă����B
		vector<int> incomes(n);  // �����̃��X�g
		
		int sum = 0;  // �����̍��v�l
		for (auto& a : incomes) {
			cin >> a;
			sum += a;
		}

		// �����̕��ϒl���v�Z����B
		double average = static_cast<double>(sum) / n;

		// �����𖞂����l���𐔂��ďo�͂���B
		cout << count_if(incomes.begin(), incomes.end(), [=](int a) {return a <= average; }) << endl;
	}
	return 0;
}
