// dom2018c.cpp - ACM ICPC 2018�N�����\�IC - �����w�r���u�݂Ȃƃn���J�X�v
// https://icpc.iisf.or.jp/past-icpc/domestic2018/contest/all_ja.html#section_C
// Copyright 2023 Kitajima Akira
// Author: Kitajima Akira <kitajima@osakac.ac.jp>

#include <functional>
#include <iostream>
using namespace std;

// f(i) = 0 �ɍł��߂�i (i_min <= i <= i_max)��������B
// f()�͔͈͓��ŒP�������łȂ���΂Ȃ�Ȃ��B
// i�ƌv�Z�lf(i)��Ԃ��B
template<typename T, class F>
pair<T, T> approximate(F f, T i_min, T i_max) {
	T diff;  // ����Ɖ����Ƃ̍�
	T r;  // �֐�f�̌v�Z�l
	while ((diff = i_max - i_min) > 1) {
		// �V����i�����߂�B
		T i = i_min + (diff >> 1);

		// f(i)�����߁A0���ǂ������肷��B
		if ((r = f(i)) == 0)
			return { i, r };

		// 0���傫�����ǂ����Ŏ��͈̔͂����߂�B
		if (r > 0)
			i_max = i;
		else
			i_min = i;
	}
	return { i_min, r };
}

pair<int, int> solve(int b) {
	const long long int b2 = b * 2LL;

	// ���̊K����҂�����\�Z�ɍ������𒲂ׂ�B
	auto i_max = b / 2LL;  // i�̏��
	for (int n = 1; i_max > 2; n++) {
		// n (1�ԉ��̊K)�����܂�΁A���vb�ɂȂ�̂ɕK�v��i(���K����)�͌v�Z�ŋ��܂�B
		// 2 * b = (2 * n + i - 1) * i
		// (2 * n + i - 1) * i - 2 * b ��0�ɂ���i��������B
		const auto p = approximate(
			[&](long long int i) {return (2LL * n + i - 1LL) * i - b2; },
			1LL,
			i_max);

		// ������i�����傤�ǂȂ���ƂȂ�B
		if (p.second == 0)
			return { n, p.first };

		// ��O�̊K������͏��Ȃ��͂��Ȃ̂ŐV���ȏ����ݒ肷��B
		i_max = p.first + 1;
	}

	// i��1��2�̏ꍇ��
	return b % 2 ? make_pair(b / 2, 2) : make_pair(b, 1);
}

int main() {
	for (int b; cin >> b && b > 0; ) {
		auto answer = solve(b);
		cout << answer.first << " " << answer.second << endl;
	}
	return 0;
}
