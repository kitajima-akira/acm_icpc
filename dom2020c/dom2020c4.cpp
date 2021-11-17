// dom2020c.cpp - �ו�
// https://icpc.iisf.or.jp/past-icpc/domestic2020/contest/all_ja.html#section_C

#include <algorithm>
#include <iostream>
#include <map>
#include <cmath>
#include <vector>
using namespace std;

typedef long long int longint;  // �f�[�^�Ƃ��Ď�舵�����l�̌^
typedef map<longint, int> prime_factor;  // �f���������������̌^
typedef map<longint, longint> divisors;  // �񐔃��X�g�̌^

// p����f����f�������B
// �����ꂽ����Ԃ��B
int remove_factor(longint& p, longint f) {
	int c = 0;
	while (p % f == 0) {
		c++;
		p /= f;
	}
	return c;
}

// p��f���������������ʂ�pf�Ɋi�[����B
void prime_factorization(longint p, prime_factor& pf) {
	// 2�̐��𐔂���B
	int c = remove_factor(p, 2);
	if (c > 0)
		pf.emplace(2, c);

	// �c��̊�𐔂���B
	for (longint f = 3; f * f <= p; f += 2) {
		c = remove_factor(p, f);
		if (c > 0)
			pf.emplace(f, c);
	}

	// �c�����l��ǉ�����B
	if (p > 1)
		pf.emplace(p, 1);
}

void get_divisor(
	const longint p,
	prime_factor& pf_p, // ���ɂȂ鐔 (�f��������)
	prime_factor::const_iterator it,  // �ǂ����琔���グ�邩
	divisors& div_list,
	longint d = 1  // (���݂�)d
) {
	if (it == pf_p.end()) {
		// d���m��

		const longint co_divisor = p / d;
		if (d <= co_divisor)
			div_list.emplace(d, co_divisor);
		else
			div_list.emplace(co_divisor, d);
		return;
	}

	const auto factor = it->first;  // �f����
	const auto n_factor = it->second;   // �f�����̌�
	++it;

	// d��factor�ɂ��Ă̌������߂�B
	for (int i = 0; i <= n_factor; i++) {
		get_divisor(p, pf_p, it, div_list, d);
		d *= factor;
	}
}

int main() {
	for (longint p; // ��
		(cin >> p) && (p != 0);) {		
		// �f������������B
		prime_factor pf_p;  // �f����������������
		prime_factorization(p, pf_p);

		// w�̌������߂�B
		divisors div_p;
		get_divisor(p, pf_p, pf_p.begin(), div_p);

//		cout << "divisor: ";
//		for (const auto& d : div_p) {
//			cout << d.first << "-" << d.second << " ";
//		}
//		cout << endl;

		longint s_min = p + 2;
		for (const auto& div : div_p) {
			auto w = div.second;
			auto dxh = div.first;

			prime_factor pf_dxh;  // �f����������������
			prime_factorization(dxh, pf_dxh);

			divisors div_dxh;
			get_divisor(dxh, pf_dxh, pf_dxh.begin(), div_dxh);

			auto h = div_dxh.rbegin()->first;
			auto d = div_dxh.rbegin()->second;
			s_min = min(s_min, w + d + h);

			if (w != dxh && sqrt(w) <= dxh) {
				auto dxh = div.second;
				auto w = div.first;

				prime_factor pf_dxh;  // �f����������������
				prime_factorization(dxh, pf_dxh);

				divisors div_dxh;
				get_divisor(dxh, pf_dxh, pf_dxh.begin(), div_dxh);

				auto h = div_dxh.rbegin()->first;
				auto d = div_dxh.rbegin()->second;
				s_min = min(s_min, w + d + h);
			}
		}
		cout << s_min << endl;
	}
	return 0;
}
