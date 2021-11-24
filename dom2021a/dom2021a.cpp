// dom2021a.cpp - �r�[�ʐ肢
// https://icpc.iisf.or.jp/past-icpc/domestic2021/contest/all_ja.html#section_A

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int lucky_number(vector<int> a) {
	if (a.size() == 1)
		return a[0];

	// �����ɕ��בւ���B
	sort(a.begin(), a.end());

	// ��̘o�������B
	while (a[0] == 0)
		a.erase(a.begin());

	// ��ԏ��Ȃ��o�̃r�[�ʂ̌��𓾂�B
	const int m = a[0];

	// �擪�ȊO�̘o����m���炷�B
	for (auto i = a.begin() + 1; i != a.end(); ++i) {
		*i -= m;
	}

	return lucky_number(a);
}

int main() {
	for (int a1, a2, a3, a4; (cin >> a1 >> a2 >> a3 >> a4) && (a1 || a2 || a3 || a4); ) {
		vector<int> a = { a1, a2, a3, a4 };
		cout << lucky_number(a) << endl;
	}
	return 0;
}
