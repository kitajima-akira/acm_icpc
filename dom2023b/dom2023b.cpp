// dom2023b.cpp - ACM ICPC 2023 �����\�IB ���݂�����
// https://icpc.iisf.or.jp/past-icpc/domestic2023/contest/all_ja.html#section_B
// Copyright 2023 Kitajima Akira
// Author: Kitajima Akira <kitajima@osakac.ac.jp>

#include <iostream>
#include <vector>
using namespace std;

// �����ɂ��������Ă��݂������������B
int amida_draw(const vector<int>& hlines, int p) {
	// �ォ�珇�ɂ��ǂ��Ă����B(p�𓮂���)
	for (auto& x : hlines)
		if (x + 1 == p)  // �����ɉ���
			p = x;
		else if (x == p)  // �����ɉ���
			p = x + 1;

	return p;
}

// 1�{�ǉ�����p����q�ɂ��ǂ���̂����邩���ׂ�B
pair<int, int> explore(int n, const vector<int>& hlines, int p, int q) {
	// �����̒ǉ�����(�ォ���)�ʒuy�����ɒ��ׂ�B
	for (int y = 0; y <= hlines.size(); y++) {
		// �ǉ�����ꏊy�ɐ���}������B
		auto new_hlines = hlines;
		new_hlines.insert(new_hlines.begin() + y, 0);

		// �����̉��ʒux�����ɒ��ׂ�B
		for (int x = 1; x < n; x++) {
			// �����̈ʒu���߂�B
			new_hlines[y] = x;

			// q�ɂ��ǂ�������ׂ�B
			if (amida_draw(new_hlines, p) == q)
				return { x, y };
		}
	}
	return { -1, -1 };  // ������Ȃ�
}

int main() {
	for (int n, m, p, q; cin >> n >> m >> p >> q && n > 0 || m > 0 || p > 0 || q > 0; ) {
		vector<int> hlines(m);
		for (auto& x : hlines)
			cin >> x;
		if (amida_draw(hlines, p) == q) {
			cout << "OK" << endl;
		}
		else {
			auto ans = explore(n, hlines, p, q);
			if (ans.first >= 0)
				cout << ans.first << " " << ans.second << endl;
			else
				cout << "NG" << endl;
		}
	}
	return 0;
}
