// dom2022d2.cpp - [ACM ICPC 2022 D] ����҂��̍s��
// https://icpc.iisf.or.jp/past-icpc/domestic2022/contest/all_ja.html#section_D

// �����̎}����͂��Ă�����̂́A��{�I�ɑ�������̂��߁A�傫�����l�ł͐������ԓ��ɏI���Ȃ��B

#include <iostream>
#include <vector>
using namespace std;

class queue {
public:
	// �ŏ��̕��я�s�A�ŏI�I�ȓ��ꏇt�A�v���̂Ƃ��̊��p���ď����ݒ肷��B
	queue(const vector<int>& s, const vector<int>& t, int b) :
		before(), after(), base(b) {
		before = s;
		after = t;
	}


	// ��������k�܂łŎw�肳�ꂽ���ʂƈ�v���镪�����̑��������߂�B
	int num_combination(int k) const {
		// �����s�\�ȏꍇ�͐�ɏ����B
		if (!is_valid_goal())
			return 0;

		int p = 0;
		for (int i = 0; i < before.size() - 1; i++) {
			if (!is_same_order(before[i], before[i + 1]))
				p++;
		}
		if (p >= k)
			return 0;

		return explore(0, 1, k);
	}

private:
	vector<int> before;  // �Q�[�g�O�̏� (�`�P�b�g�ԍ��̗�)
	vector<int> after;  // �Q�[�g��̏� (�`�P�b�g�ԍ��̗�)
	int base;  // �v���̂Ƃ��̊ (���Ŏw��)

	// �������������\��(0�ȏォ)���`�F�b�N����B
	bool is_valid_goal() const {
		// after�̍Ō�̗v�f���猩�Ă����B
		for (int i = after.size() - 1; i > 0; i--) {
			const auto last = after[i];
			// �v�flast���O�ɂ���Alast�Ɉ�ԋ߂��Alarge���傫���l��������B
			auto large = 0;  // last���傫���l (�����l�͂��ׂĂ̒l��菬����0)
			auto large_pos = 0;
			for (int j = i - 1; j >= 0; j--) {
				if (after[j] > last) {
					large = after[j];
					large_pos = j;
					break;
				}
			}
			if (large > 0) {  // ���������ꍇ
				// before�̒�����large��T���B
				int j;
				for (j = 0; j < before.size(); j++)
					if (before[j] == large)
						break;

				// large���܂߂�last�܂ł̌�(last�܂܂�)
				const auto prefix_size = i - large_pos;

				// large����̗v�f��������Ȃ��ꍇ
				if (j + prefix_size >= before.size())
					return false;

				// before��large����̕��т�after��large����̕��т��r����B
				for (int d = 1; large_pos + d <= i; d++)
					if (before[j + d] != after[large_pos + d])
						return false;

				// ��v���Ă�����large����last�܂ł�OK
				// large���瑱����悤i��ݒ�
				i = large_pos + 1;
			}
			// ������Ȃ������ꍇ�͂���last��OK

		}
		return true;
	}


	// a, b��after���ł����̏��Ԃ��ǂ���
	bool is_same_order(int a, int b) const {
		for (int t : after) {
			if (t == a)
				return true;  // ���a�����������B
			if (t == b)
				return false;  // ���b�����������B
		}
		return false;
	}

	// �S�T���Ŏ}���肵�Ȃ��琔����B
	// before��i�Ԗڂ��珇�ɕ�������Bnum_partition�����݂̕������ŁAk�����܂ōl����B
	// 
	// ��������
	// https://icpc.iisf.or.jp/past-icpc/domestic2022/commentaries.html#D
	// 
	// �� 4 2!3?5!1       
	// �� 1 3 4 2 5
	// (�P�[�X1) �C�ӂ̌���2�A���v�f�ɂ��āA���2�A���v�f�̏����قȂ�Ƃ��A����2�A���v�f�Ԃ͕K����؂�Ȃ��Ƃ����Ȃ��B (!)
	// (�P�[�X2) ����ԍ����O�ɂ��邷�ׂĂ̐l�̔ԍ������傫���Ƃ��A���̔ԍ��̑O�ŋ�؂��Ă��悢�B (?)
	// * �u!�̍��v n1 + ?�̑I��(��؂���)�� n2 < k�v�𖞂����g����(�e?�ɑ΂���0 or 1)�̐����v�Z����B
	// * 0 �` k - n1�̊er�ɑ΂��� n2 C r (Combination(n2,r) = n2 * (n2 - 1) * ... * r / r * ...* 1)�̘a
	int explore(int i, int num_partition, int k) const {
		// ��������k�𒴂����炻��ȏ�T������K�v�͂Ȃ��B
		if (num_partition > k)
			return 0;

		// �����ł���Ƃ����T���B
		int local_max = 0;
		for (size_t j = i; j < before.size() - 1; j++) {
			// j�Ԗڂ�j + 1�ԖڂƂ̊Ԃ���؂�邩�ǂ����𒲂ׂ�B
			if (!is_same_order(before[j], before[j + 1]))  // �P�[�X1
				// j �� j + 1�̊Ԃŋ�؂�B
				return explore(j + 1, num_partition + 1, k);

			local_max = max(local_max, before[j]);
			if (local_max < before[j + 1]) {  // �P�[�X2
				int r = explore(j + 1, num_partition + 1, k)  // ��؂����ꍇ�̐�
					+ explore(j + 1, num_partition, k);
				if (r > base)
					r -= base;
				return r;
			}
		}

		// �Ō�܂ōs�����ꍇ
		return 1;
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
		queue q(s, t, 998244353);
		cout << q.num_combination(k) << endl;
	}
	return 0;
}
