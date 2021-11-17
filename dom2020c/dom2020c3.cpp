// dom2020c.cpp - �ו�
// https://icpc.iisf.or.jp/past-icpc/domestic2020/contest/all_ja.html#section_C

#include <algorithm>
#include <iostream>
#include <map>
#include <cmath>
#include <vector>
using namespace std;

class luggage {
public:
	typedef long long int longint;  // �f�[�^�Ƃ��Ď�舵�����l�̌^
	typedef map<longint, int> prime_factor;  // �f���������������̌^

	luggage(longint given) : 
		p(given),
		w(),
		d(),
		h(),
		pf_p(),
		it()
		{
		// �f������������B
		prime_factorization(p, pf_p);
		it = pf_p.begin();
	}

	longint explore(longint current_w = 1) {
		longint s_min = p + 2;  // �a�̍ő�l��p + 2 (1 * 1 * p)�̏ꍇ

		if (it == pf_p.end()) {
			// w���m��
			w = current_w;
	//		cout << "w = " << w << endl;

			// w >= d >= h > 0�̂Ƃ�
			// w * w * w >= w * d * h
			// w * w >= d * h
			// ���̂Ƃ��̂�d��񋓂���B
			const auto dxh = p / w;
			return (w < sqrt(dxh)) ? s_min : w + explore_h(pf_p.begin());
		}

		const auto factor = it->first;  // �f����
		const auto n_factor = it->second;   // �f�����̌�
		++it;

		// factor�̌���0�̏ꍇ
		s_min = min(s_min, explore(current_w));

		// w��factor�ɂ��Ă̌������߂�B
		for (int i = 1; i <= n_factor; i++) {
			current_w *= factor;
			// pf_p���X�V(w�̕������炷)
			pf_p[factor]--;
			s_min = min(s_min, explore(current_w));
		}
		// pf_p���X�V(���̃��[�v�̂��߂Ɍ��ɖ߂�)
		pf_p[factor] = n_factor;

		return s_min;
	}

private:
	longint p;  // p
	longint w, d, h;
	prime_factor pf_p;  // �f����������������
	prime_factor::const_iterator it;  // �ǂ����琔���グ�邩

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

	// w >= d >= h > 0�̏ꍇ�̂� d��h�����߂�B
	longint explore_h(prime_factor::const_iterator it, longint current_h = 1) {
		if (it == pf_p.end()) {
			// h���m��
			h = current_h;
			// d�͂��̒l�Ō��܂�B
			const auto d = p / w / h;
	//		cout << "w = " << w << ", d = " << d << ", h = " << h << endl;
			return d + h;
		}

		const auto factor = it->first;  // �f����
		const auto n_factor = it->second;   // �f�����̌�
		++it;

		// w >= d >= h > 0�̏ꍇ�݂̂�����A
		// w >= h ... ����1
		// d >= h
		// d * h >= h * h
		// sqrt(d * h) >= h ...����2 (������ d * h == p / w)
		const auto h_max = min(w, static_cast<longint>(sqrt(p / w)));  // ����1�Ə���2�𗼕�������h�̏��

		// factor�̌���0�̏ꍇ
		longint s_min = explore_h(it, current_h);

		// h��factor�ɂ��Ă̌������߂�B
		for (int i = 1; i <= n_factor; i++) {
			current_h *= factor;
			if (current_h > h_max)  // ���~�b�g�𒴂���Ƒł��؂�
				return s_min;
			s_min = min(s_min, explore_h(it, current_h));
		}

		return s_min;
	}
};

int main() {
	for (luggage::longint p; // ��
		(cin >> p) && (p != 0);) {		
		luggage lg(p);
		// 3�̐��ɕ����Ęa�̍ŏ��l�����߂�B
		cout << lg.explore() << endl;
	}
	return 0;
}
