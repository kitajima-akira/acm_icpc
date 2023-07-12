// dom2023b2.cpp - ACM ICPC 2023 �����\�IB ���݂�����
// https://icpc.iisf.or.jp/past-icpc/domestic2023/contest/all_ja.html#section_B
// ����ɂ���O(m)�̕��@�Ŏ���
// https://icpc.iisf.or.jp/past-icpc/domestic2023/commentaries.html#B
// Copyright 2023 Kitajima Akira
// Author: Kitajima Akira <kitajima@osakac.ac.jp>

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

// �����ɂ��������Ă��݂������������B
int amida_trace(const vector<int>& hlines, int p, vector<int>& trace) {
	trace.push_back(p);
	// �ォ�珇�ɂ��ǂ��Ă����B(p�𓮂���)
	for (auto& x : hlines) {
		if (x + 1 == p)  // �����ɉ���
			p = x;
		else if (x == p)  // �����ɉ���
			p = x + 1;
		trace.push_back(p);
	}

	return p;
}

// 1�{�ǉ�����p����q�ɂ��ǂ���̂����邩���ׂ�B
pair<int, int> explore(int n, const vector<int>& hlines, int p, const vector<int>& trace_p, int q) {
	// q�������炽�ǂ�A�ǂ��i�ނ̂�trace_q�𓾂�B
	vector<int> trace_q;
	{
		// hlines�̋t�������߂�B
		auto reverse_hlines = hlines;
		reverse(reverse_hlines.begin(), reverse_hlines.end());

		// q����t�ɂ��ǂ�B
		amida_trace(reverse_hlines, q, trace_q);

		// ���ǂ��������t�ɂ��ďォ��̏��ɂ���B
		reverse(trace_q.begin(), trace_q.end());
	}

	// p�̏���q�̏����r���A�ׂɂȂ�ʒu������΂����ɉ��_������B
	for (int y = 0; y < trace_p.size(); y++) {
		if (trace_p[y] == trace_q[y] + 1) { // p, q�̏��ɕ���
			return { trace_p[y] - 1, y };
		}
		else if (trace_q[y] == trace_p[y] + 1) { // q, p�̏��ɕ���
			return { trace_q[y] - 1, y };
		}
	}

	return { -1, -1 };  // ������Ȃ�
}

int main() {
	for (int n, m, p, q; cin >> n >> m >> p >> q && n > 0 || m > 0 || p > 0 || q > 0; ) {
		vector<int> hlines(m);
		for (auto& x : hlines)
			cin >> x;
		vector<int> trace;
		if (amida_trace(hlines, p, trace) == q) {
			cout << "OK" << endl;
		}
		else {
			auto ans = explore(n, hlines, p, trace, q);
			if (ans.first >= 0)
				cout << ans.first << " " << ans.second << endl;
			else
				cout << "NG" << endl;
		}
	}
	return 0;
}
