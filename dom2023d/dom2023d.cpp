// dom2023d.cpp - ACM ICPC 2023�N�����\�ID �����I�Ȗ��Z�b�g
// https://icpc.iisf.or.jp/past-icpc/domestic2023/contest/all_ja.html#section_D
// Copyright 2023 Kitajima Akira
// Author: Kitajima Akira <kitajima@osakac.ac.jp>

#include <iostream>
#include <string>
#include <vector>
using namespace std;

// values�̐��l��g�ݍ��킹��(���܂��I��ő�����)v�ɂȂ邩�ǂ������肷��B
// values�͏�����O��Ƃ��Ă���B
bool constructable(int v, const vector<int>& values) {
	if (values.empty())
		return false;

	// values�̐擪�̗v�f�����o���B
	auto front = values.front();

	if (front == v)
		return true;
	if (front > v)
		return false;  // values�̎c���front���傫���̂ŁA����������Ȃ��B

	// front�������B
	auto copied_values = values;
	copied_values.erase(copied_values.begin());

	// front���g���E�g��Ȃ��̗����������B
	return constructable(v, copied_values)  // �g��Ȃ� 
		|| constructable(v - front, copied_values);  // �g��
}

// �ŏ���萔�����߂�B
int get_min_problems(const vector<int>& required_points) {
	auto points = required_points;
	// �擪��0�͏��O����B
	points.erase(points.begin());

	vector<int> min_set;  // �Œ���K�v�ȓ_�̃��X�g
	for (auto v : points)
		if (!constructable(v, min_set))
			min_set.push_back(v);

	return min_set.size();
}

int main() {
	for (int n; cin >> n && n > 0;) {
		string s;
		cin >> s;
		vector<int> required_points;
		for (int k = 0; k <= n; k++)
			if (s[k] == 'o')
				required_points.push_back(k);

		cout << get_min_problems(required_points) << endl;

	}
	return 0;
}
