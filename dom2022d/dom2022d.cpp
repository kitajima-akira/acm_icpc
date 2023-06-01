// dom2022d.cpp - [ACM ICPC 2022 D] ����҂��̍s��
// https://icpc.iisf.or.jp/past-icpc/domestic2022/contest/all_ja.html#section_D

// Sample Input��138�����x�ŏI��

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

	// �^����ꂽ����partition (�傫��size)�ɑ΂��A
	// �s��before��ʂ����Ƃ���after���ɂȂ邩�ǂ����𔻒肷��B
	bool check_condition(const vector<int>& partition, size_t size) const {
		// �V�~�����[�V��������B
		auto current_partition = partition;  // �e��̐擪�ʒu

		// ����after�̐擪���珇�ɃV�~�����[�V�����������ƈ�v���邩���ׂ�B
		for (const auto& t : after) {
			// before����؂����e��̐擪�̒��ł̍ŏ��l(�Ƃ���͂ǂ̗�)�𓾂�B
			int next_ticket_no = before.size() + 1;  // �ŏ��̃`�P�b�g�ԍ�
			int next_queue;  // �ŏ��̃`�P�b�g�ԍ����L�҂������
			for (size_t i = 0; i < size; i++) {
				const int current_position = current_partition[i];
				if (current_position < partition[i + 1]  // ���̗񂪋�łȂ�
					&& before[current_position] < next_ticket_no) {  // �`�P�b�g�ԍ����ŏ�
					// �ŏ������X�V����B
					next_ticket_no = before[current_position];
					next_queue = i;
				}
			}

			// after�̏��ƈ�v���Ă��Ȃ���Εs��v�Ɣ���
			if (next_ticket_no != t)
				return false;

			// �L���[���X�V����B(��l���ꂳ����B)
			current_partition[next_queue]++;
		}
		return true;
	}

	// ��������k�܂łŎw�肳�ꂽ���ʂƈ�v���镪�����̑��������߂�B
	int num_combination(size_t k) const {
		//  �����͎��̃f�[�^�`���Ƃ���B
		// [�v�f�ԍ�]			0 1 2 3 4 5
		// [���тƋ�؂�]		4 2|3 5|1
		// [�f�[�^�`��]		{ 0, 2, 4, 5 }
		// �u�����̍ŏ��̗v�f�̗v�f�ԍ��v�̕��� +�u�Ō�̗v�f�̎��̗v�f�ԍ��v
		vector<int> current_partition(k + 1);  // ���x�d���̂��߁A�ő吔�p�ӂ��A�r���ő傫���͕ς��Ȃ��B
		current_partition[0] = 0;
		int r = 0;
		for (int i = 1; i <= k; i++) {
			r += explore_all(i, 0, current_partition, 1);
			if (r >= base)
				r -= base;
		}
		return r;
	}

private:
	vector<int> before;  // �Q�[�g�O�̏� (�`�P�b�g�ԍ��̗�)
	vector<int> after;  // �Q�[�h��̏� (�`�P�b�g�ԍ��̗�)
	int base;  // �v���̂Ƃ��̊ (���Ŏw��)

	// before��current_position���炿�傤��k��������S�p�^�[����񋓂��A�����𖞂������𐔂���B
	int explore_all(int k, int current_position, vector<int>& current_partition, size_t current_partition_index) const {
		if (k == 1) {
			// ��������
			// �Ō�̗v�f�͑S�̂̒����ɂ��Ă����B
			current_partition[current_partition_index] = before.size();

			// ����������܂����̂ŁA����ɏ]�������𔻒肷��B
			const auto r = check_condition(current_partition, current_partition_index) ? 1 : 0;
			return r;
		}

		// �����r���̐擪�̑傫����ς��Đ[���D��T��
		int r = 0;
		const int rest_size = before.size() - current_position;  // �c��l��
		for (int current_size = 1; current_size <= rest_size - (k - 1); current_size++) {
			const int next_partition = current_position + current_size;
			current_partition[current_partition_index] = next_partition;
			r += explore_all(k - 1, next_partition, current_partition, current_partition_index + 1);
			if (r >= base)
				r -= base;
		}
		return r;
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
