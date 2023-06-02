// dom2022d3.cpp - [ACM ICPC 2022 D] ����҂��̍s��
// https://icpc.iisf.or.jp/past-icpc/domestic2022/contest/all_ja.html#section_D
//
// �R���i�Ђł̓��ʃ��[���̂��߁A���O�������\
// 
// �ȉ��̕����͎��O�������Ă����B(�R���e�X�g���Ɏ�������Ǝ��Ԃ��Ȃ��Ȃ�)
//  * �������Z
//      https://github.com/square1001/bigint-library
//      �Q�l https://qiita.com/square1001/items/1aa12e04934b6e749962
//  * �g�ݍ��킹(Combination)
//      ���L�R�[�h�Q��
#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <set>
#include <vector>
#include "bigint-library/newbigint.h"
using namespace std;

// begin [�g�ݍ��킹] ///////////////////////////////////////////////////////////////

// �����̙p��v�Z
// ��Z�񐔂����Ȃ��Ȃ�悤�ɍH�v O(log r)
bigint power(long long int n, int r) {
	if (r == 0)
		return bigint(1);

	if (n == 2LL && r < 32)
		return n << (r - 1);
	
	// �ȉ��An > 2, r > 0�̏������Ŏ��s

	bigint p(n);  // �v�Z�l
	vector<bigint> table = { p };  // n, n^2, n^4, ..., n^i �̕\

	// k = n^i < r�ƂȂ�ő��k�����߂�B
	int i2 = 1;  // i^2
	while (2 * i2 <= r) {
		i2 *= 2; // i^2 + i^2
		p *= p;
		table.push_back(p);
	}

	int i = i2;  // �v�Z���I�������Z��
	while (i < r) {
		// �c�蕔���ɂ��� n^j2 (j2��2�̙p��)�ƂȂ�j2�����߂�B
		int j2 = 1;
		int j = 1;
		while (i + 2 * j2 <= r) {
			j2 *= 2;
			j++;
		}

		p *= table[j - 1];
		i += j2;
	}

	return p;
}

// n��f������������B
void get_prime_decomposition(int n, vector<int>& pl) {
	// ���������߂�B
	if (n < 0) {
		pl.push_back(-1);
		n = -n;
	}

	if (n == 0)
		return;

	// n = 1�̏ꍇ
	if (n == 1) {
		if (pl.empty())
			pl.push_back(1);
		return;
	}

	// �f����2�̌��𐔂���B
	while (n % 2 == 0) {
		pl.push_back(2);
		n /= 2;
	}

	// ��̑f���������߂�B
	for (int i = 3; i * i <= n; i += 2) {
		while (n % i == 0) {
			pl.push_back(i);
			n /= i;
		}
	}
	if (n > 1)
		pl.push_back(n);
}

// �L����
// �܂������͕s���S (��Z�E���Z�̂� ���̐��̂� ��O[0���Z��0�̕\���ȂǕK�v])
class rational_number {
public:
	rational_number(long long int n): 
		prime_factor_list() {
		// �f������������B
		std::vector<int> pflist;
		get_prime_decomposition(n, pflist);

		// �f�[�^���i�[����B
		for (const auto& pf : pflist)
			if (pf != 1)
				prime_factor_list[pf] += 1;
	}

	rational_number& operator*=(const rational_number& n) {
		for (const auto& pf : n.prime_factor_list) {
			const auto factor = pf.first;
			const auto exponent = pf.second;
			prime_factor_list[factor] += exponent;
		}
		return *this;
	}

	rational_number operator*(const rational_number& b) const { 
		return rational_number(*this) *= b; 
	}

	rational_number& operator/=(const rational_number& n) {
		for (const auto& pf : n.prime_factor_list) {
			const auto factor = pf.first;
			const auto exponent = pf.second;
			if (prime_factor_list[factor] == exponent)
				prime_factor_list.erase(factor);
			else
				prime_factor_list[factor] -= exponent;
		}
		return *this;
	}

	pair<bigint, bigint> to_bigint() {
		bigint numerator = accumulate(
			prime_factor_list.begin(), 
			prime_factor_list.end(), 
			bigint(1),
			[](bigint acc, pair<long long int, int> pf) {
				const auto factor = pf.first;
				const auto exponent = pf.second;
				return exponent > 0 ? acc * power(factor, exponent) : acc;
			});
		bigint denominator = accumulate(
			prime_factor_list.begin(), 
			prime_factor_list.end(), 
			bigint(1),
			[](bigint acc, pair<int, int> pf) {
				const auto factor = pf.first;
				const auto exponent = pf.second;
				return exponent < 0 ? acc * power(factor, -exponent) : acc;
			});
		return { numerator, denominator };
	}

	std::string to_string() const {
		std::string s;
		bool first_time = true;
		for (auto pf : prime_factor_list) {
			const auto factor = pf.first;
			const auto exponent = pf.second;
			s += (first_time ? "" : " ") 
				+ std::to_string(factor) + "^" + std::to_string(exponent);
			first_time = false;
		}
		return s;
	}

	friend std::ostream& operator<<(std::ostream& os, const rational_number& r) {
		os << r.to_string();
		return os;
	}

private:
	map<long long int, int> prime_factor_list;  // �����Ƃ��̙p�悲�ƂɊǗ�
};

// �g�ݍ��킹�̐�(n C r)���v�Z����B
bigint combination(int n, int r) {
	if (r == 1)
		return n;

	// ���q  n * (n - 1) * ... * (n - (r - 1))
	rational_number c(1);
	for (int i = 0; i < r; i++)
		c *= n - i;

	// ���� r * r - 1 * ... * 1 
	for (int i = 2; i <= r; i++)
		c /= i;

	return c.to_bigint().first;
}

// end[�g�ݍ��킹] /////////////////////////////////////////////////////////////////

// �u����҂��̍s��v[ACM ICPC 2022 D] ���������߂̃N���X
class audience_queue {
public:
	// �ŏ��̕��я�s�A�ŏI�I�ȓ��ꏇt�A�v���̂Ƃ��̊��p���ď����ݒ肷��B
	audience_queue(const vector<int>& s, const vector<int>& t, int b) :
		initial_line(), final_line(), base(b) {
		initial_line = s;
		final_line = t;
	}

	// ��������k�܂łŎw�肳�ꂽ���ʂƈ�v���镪�����̑��������߂�B
	bigint num_combination(int k) {
		// �����s�\�ȏꍇ�͐�ɏ����B
		if (!is_valid_final_line() || !check_splittability())
			return 0;

		// �������Ȃ��Ƃ����Ȃ��ӏ���k�ȏ゠��Ή��Ȃ��B
		const auto num_split = must_split_list.size();
		if (num_split >= k)
			return 0;

		// �������Ă����Ȃ��Ă������ӏ��̌�
		const auto num_splittable 
			= (initial_line.size() - 1) - num_split - dont_split_list.size();

		// �v�Z����B
		bigint r(1);  // 0�ӏ���������ꍇ�������l�Ƃ���B
		for (int i = 1; i <= num_splittable && i + num_split < k; i++) {
			// i�ӏ���������ꍇ
			r += combination(num_splittable, i);
			if (r > base) {
				// ��]�����߂�B
				bigint d = r / base;
				r -= d * base;
			}
		}

		return r;
	}

private:
	vector<int> initial_line;  // �Q�[�g�O�̏� (���Ȕԍ��̗�)
	vector<int> final_line;  // �Q�[�g��̏� (���Ȕԍ��̗�)
	bigint base;  // �v���̂Ƃ��̊ (���Ŏw��)

	set<int> dont_split_list;  // �����ł��Ȃ��ӏ� (���̗v�f�̎�O)
	set<int> must_split_list;  // �������Ȃ��Ƃ����Ȃ��ӏ� (���̗v�f�̎�O)

	int find_on_initial_line(size_t e) const {
		for (size_t i = 0; i < initial_line.size(); i++)
			if (initial_line[i] == e)
				return i;
		return -1;
	}

	// �������������\��(0�ȏォ)���`�F�b�N����B
	// dont_split_list���X�V����B
	bool is_valid_final_line() {
		// final_line�̍Ō�̗v�f���猩�Ă����B
		for (int i = final_line.size() - 1; i > 0; i--) {
			const auto t = final_line[i];
			// �v�ft���O�ɂ���At�Ɉ�ԋ߂��At���傫���lu��������B
			// initial_line: ....... u .........
			//                       ^-- p
			// final_line:   ... u ....... t ... (u > t)
			//                   ^-- j     ^-- i
			//                   <-------> offset
			int j;  // u��final_line��̈ʒu
			for (j = i - 1; j >= 0; j--)
				if (final_line[j] > t)
					break;
			auto u = (j < 0) ? 0 : final_line[j];  // t���傫���l 
												// (�����l�͂��ׂĂ̒l��菬����0)

			if (u > 0) {  // ���������ꍇ
				// initial_line��u����̕��т�final_line��u����̕��т��r����B

				// initial_line�̒�����u��T���B
				int p = find_on_initial_line(u);
				const auto offset = i - j;  // u���܂߂�t�܂ł̌�(t�܂܂�)

				// initial_line�ł�u����̗v�f��������Ȃ��ꍇ
				if (p + offset >= initial_line.size())
					return false;  // �s�\

				// ��r����B
				for (int d = 1; j + d <= i; d++) {
					if (initial_line[p + d] != final_line[j + d])
						return false;
					// �������Ă͂����Ȃ����X�g�ɓo�^����B
					dont_split_list.insert(p + d);
				}

				// ��v���Ă�����u����t�܂ł�OK
				// u���玟��for���[�v�𑱂���悤i��ݒ�
				i = j + 1;
			}
			// ������Ȃ������ꍇ�͂���t��OK
		}
		return true;
	}

	// initial_line���̕������Ă͂����Ȃ������A�������Ȃ��Ƃ����Ȃ����������ׂČ�����B
	// must_split_list, dont_split_list���X�V����B
	bool check_splittability() {
		set<int> checked_number_list;  // �`�F�b�N�ς݂̍��Ȕԍ����X�g(�W��)
		for (int i = 0; i < final_line.size(); i++) {
			const int t = final_line[i];  // ���̃��[�v�Œ��ڂ���v�f

			// initial_line�̒�����t��T���B
			const auto p = find_on_initial_line(t);

			// initial_line��t���擪�ł�������A���̏o����(checked_number_list)�̗v�f
			// (final_line��i���O)�̎��Ȃ番���s�v
			// 
			// initial_line: ... t ...... u ... s.... (t < u)
			//                   ^-- p    ^-- q ^-- r
			// final_line:   ...... t .... s ........
			//                      ^-- i  ^-- j
			if (p > 0
				&& (checked_number_list.find(initial_line[p - 1]) 
					== checked_number_list.end())) {  // �������K�v��
				// �ʒup�ł͕����K�v�Ƃ���B
				must_split_list.insert(p);

				// final_line��i����ɂ���At��菬�����v�fs�����ׂČ�����B
				for (int j = i + 1; j < final_line.size(); j++) {
					const auto s = final_line[j];
					if (s < t) {
						const auto r = find_on_initial_line(s);

						// initial_line���s�̈ʒur���O�ɂ���A
						// t�܂��͒��߂�t���傫���v�fu��������B
						int q;
						for (q = r - 1; q >= 0; q--)
							if (initial_line[q] >= t)  // initial_line[q] == u
								break;
						if (q < 0)  // ������Ȃ������ꍇ
							return false;

						// q����r�܂ł̊Ԃ͕����s�Ƃ���B
						for (int k = q + 1; k <= r; k++)
							dont_split_list.insert(k);
					}
				}
			}

			// t���`�F�b�N�ς݃��X�g�ɉ�����B
			checked_number_list.insert(t);
		}
		return true;
	}
};

int main() {
	for (int n, k; (cin >> n >> k) && n > 0 && k > 0; ) {
		vector<int> s(n);
		for (auto& e : s)
			cin >> e;
		vector<int> t(n);
		for (auto& e : t)
			cin >> e;
		audience_queue q(s, t, 998244353);
		cout << q.num_combination(k) << endl;
	}
	return 0;
}
