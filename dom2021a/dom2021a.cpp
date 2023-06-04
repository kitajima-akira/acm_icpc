// dom2021a.cpp - �r�[�ʐ肢
// https://icpc.iisf.or.jp/past-icpc/domestic2021/contest/all_ja.html#section_A
// Copyright 2023 Kitajima Akira
// Author: Kitajima Akira <kitajima@osakac.ac.jp>

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

// ���b�L�[�i���o�[�����߂�B
// a��beg�ȍ~��ΏۂƂ���B
// a�͍X�V�����B
// ���b�L�[�i���o�[��Ԃ��B
int lucky_number(vector<int>& a, const vector<int>::iterator& beg) {
	// �����ɕ��בւ���B
	sort(beg, a.end());

	// ��(�l��0)�̘o���΂��B
	auto min_non_zero_p = find_if_not(beg, a.end(), [](const int& x) { return x == 0; });

	const int m = *min_non_zero_p;  // ��ԏ��Ȃ��o�̃r�[�ʂ̌�

	if (min_non_zero_p + 1 == a.end())  // ��łȂ��o�������
		return m;  // �c�����r�[�ʂ̌������Ȃ��̍����̃��b�L�[�i���o�[

	// first_non_zero�ȊO�̘o����m���炷�B
	for_each(min_non_zero_p + 1, a.end(), [&m](int& x) { x -= m; });

	return lucky_number(a, min_non_zero_p);  // �ċA�I�ɌJ��Ԃ��B
}

int main() {
	for (int a1, a2, a3, a4; (cin >> a1 >> a2 >> a3 >> a4) && (a1 || a2 || a3 || a4); ) {
		vector<int> a = { a1, a2, a3, a4 };
		cout << lucky_number(a, a.begin()) << endl;
	}
	return 0;
}
