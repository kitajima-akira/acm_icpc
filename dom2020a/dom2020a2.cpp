// dom2020a2.cpp - �J�E���g�A�b�v2020
// https://icpc.iisf.or.jp/past-icpc/domestic2020/contest/all_ja.html#section_A
// Copyright 2023 Kitajima Akira
// Author: Kitajima Akira <kitajima@osakac.ac.jp>

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

// data����sequnce�̏o���񐔂𐔂���B
int count_sequence(const vector<int>& data, const vector<int>& sequence) {
	int c = 0;  // �o����
	for (auto it = data.begin();
		(it = search(it, data.end(), sequence.begin(), sequence.end())) != data.end();
		++it) {
		c++;
	}
	return c;
}

int main() {
	const vector<int> a2020 = { 2, 0, 2, 0 };  // �����Ώۃf�[�^

	for (int n; cin >> n && n != 0;) {
		vector<int> d(n);  // �����̕���
		for (int& i : d)
			cin >> i;

		cout << count_sequence(d, a2020) << endl;
	}
	return 0;
}
