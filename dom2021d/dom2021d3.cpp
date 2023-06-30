// dom2021d3.cpp - ���D�z��
// https://icpc.iisf.or.jp/past-icpc/domestic2021/contest/all_ja.html#section_D
// ���I�v��@�ŉ��� (���ǔ�)
// C++17 �\��������
// https://icpc.iisf.or.jp/past-icpc/domestic2021/commentaries.html#D
// Copyright 2023 Kitajima Akira
// Author: Kitajima Akira <kitajima@osakac.ac.jp>

#include <algorithm>
#include <iostream>
#include <tuple>
#include <vector>
using namespace std;

// 3�g��\���^
using part3 = tuple<int, int, int>;

// 3�gt�������ɕ��בւ���B
// ���בւ���3�g��Ԃ��B
part3 sort(const part3& t) {
	const auto& [a, b, c] = t;
	if (a <= b && a <= c)
		return (b <= c) ? part3{ a, b, c } : part3{ a, c, b };

	else if (b <= a && b <= c)
		return (a <= c) ? part3{ b, a, c } : part3{ b, c, a };

	return (a <= b) ? part3{ c, a, b } : part3{ c, b, a };
}

// �V�����v�fbi�����Ƃ�candidate���X�V����B
void update_candidate(vector<part3>& candidate, int bi) {
	vector<part3> new_candidate;  // �V�������

	// �V��������ǉ�
	for (const auto& t : candidate) {
		const auto& [a, b, c] = t;
		// �e���́A�d�����Ȃ������߁A���x�ւ��Ă����B
		new_candidate.push_back(sort({ a + bi, b, c }));
		new_candidate.push_back(sort({ a, b + bi, c }));
		new_candidate.push_back(sort({ a, b, c + bi }));
	}

	// �d���������폜
	sort(new_candidate.begin(), new_candidate.end());
	auto uniq_end = unique(new_candidate.begin(), new_candidate.end());
	new_candidate.erase(uniq_end, new_candidate.end());

	// �X�V
	candidate = new_candidate;
}

// �V�����v�fbi�����Ƃ�candidate���X�V����B(���팸�Ȃ�)
// �ǂꂾ���������Ⴄ���B
void update_candidate_basic(vector<part3>& candidate, int bi) {
	vector<part3> new_candidate;  // �V�������

	// �V��������ǉ�
	for (const auto& t : candidate) {
		const auto& [a, b, c] = t;

		new_candidate.push_back({ a + bi, b, c });
		new_candidate.push_back({ a, b + bi, c });
		new_candidate.push_back({ a, b, c + bi });
	}

	// �X�V
	candidate = new_candidate;
}

// �^������q���̍ő吔�����߂�B
int get_max_number_of_kids(vector<int> b) {
	vector<part3> candidate = { { 0, 0, 0 } };  // �������Ƃ̕��D�̑��a

	// b������ǉ����āA�����X�V����B
	for (const auto& i : b)
		update_candidate(candidate, i);

	// �a�̍ŏ��l�̍ő�l�����߂�B
	int k_max = 0;  // ���߂�ő�l
	for (const auto& t : candidate)
		// t = [a, b, c]�̂Ƃ��A�\�[�g�ς݂Ȃ̂�a <= b <= c������a���ŏ��l
		k_max = max(k_max, get<0>(t));

	return k_max;
}

int main() {
	for (int n; (cin >> n) && (n > 0); ) {
		vector<int> b(n);
		for (auto& i : b)
			cin >> i;
		cout << get_max_number_of_kids(b) << endl;
	}
	return 0;
}
