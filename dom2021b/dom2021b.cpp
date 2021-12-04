// dom2021b.cpp - �S�܂��v�Z�p�Y��
// https://icpc.iisf.or.jp/past-icpc/domestic2021/contest/all_ja.html#section_B

#include <iostream>
#include <vector>
using namespace std;

// �S�܂��v�Z�p�Y�����������߂̃N���X
class hundo_puzzle {
public:
	// �܂��ڂ̕�width�E����height�ƁA�l�̍ŏ��lmin_value���w�肵�ď����ݒ肷��B
	// �umin_value - 1�v�̒l�𖢊m��(�󔒂̂܂�)��\���̂ɗp����B
	hundo_puzzle(int width, int height, int min_value) :
		h(width, min_value - 1),
		v(height, min_value - 1),
		grid(height, vector<int>(width, min_value - 1)),
		not_determined_yet(min_value - 1) {
		// ��[�̐��̈�ԍ��� 0 �ƌ��߂Ă���
		h[0] = 0;
	}

	// �܂��̎w�肵���ʒu(x, y)�ɒln��ݒ肷��B
	void set_grid(int x, int y, int n) {
		grid[y - 1][x - 1] = n;
	}

	// ���ׂĂ̂܂�����ӂɌ��܂邩���肷��B
	// ���ʂ̐^�U�l��Ԃ��B
	bool has_unique_solution() {
		fix();
		return check_unique();
	}

private:
	vector<int> h;  // ��[
	vector<int> v;  // ���[
	vector<vector<int>> grid;  // �܂���
	int not_determined_yet;  // ���m��ł��邱�Ƃ�\���l

	// ��[��x�̈ʒu���m�肵���Ƃ��ɁA�܂��m�肵�Ă��Ȃ����[�𖄂߁A����ɑ΂��čċA�I�ɖ��߂�B
	void fix_v(int x) {
		for (size_t y = 0; y < v.size(); y++) {
			if (v[y] == not_determined_yet && grid[y][x] != not_determined_yet) {
				v[y] = grid[y][x] - h[x];
				fix_h(y);
			}
		}
	}

	// ���[��y�̈ʒu���m�肵���Ƃ��ɁA�܂��m�肵�Ă��Ȃ���[�𖄂߁A����ɑ΂��čċA�I�ɖ��߂�B
	void fix_h(int y) {
		for (size_t x = 0; x < h.size(); x++) {
			if (h[x] == not_determined_yet && grid[y][x] != not_determined_yet) {
				h[x] = grid[y][x] - v[y];
				fix_v(x);
			}
		}
	}

	// ���̏�Ԃ����ӂɌ��܂��[�E���[�𖄂߂�B
	void fix() {
		for (size_t x = 0; x < h.size(); x++) {
			if (h[x] != not_determined_yet) {
				fix_v(x);
			}
		}
		for (size_t y = 0; y < v.size(); y++) {
			if (v[y] != not_determined_yet) {
				fix_h(y);
			}
		}
	}

	// ��[�E���[�ɂ��āA���ׂĖ��܂��Ă��邩���m�F����B
	// ���ʂ̐^�U�l��Ԃ��B
	bool check_unique() const {
		return find(h.begin(), h.end(), not_determined_yet) == h.end()   // ��[�`�F�b�N
			&& find(v.begin(), v.end(), not_determined_yet) == v.end();  // ���[�`�F�b�N
	}
};

int main() {
	for (int w, h; (cin >> w >> h) && (w || h); ) {   // ���E������ǂݍ��ށB
		// �����ݒ肷��B
		hundo_puzzle hp(w, h, -100);

		// �e�܂��̃f�[�^��ǂݍ��ށB
		const int k = w + h - 1;
		for (int i = 0; i < k; i++) {
			int x, y, n;
			cin >> x >> y >> n;
			hp.set_grid(x, y, n);
		}

		// �������߁A�o�͂���B
		cout << (hp.has_unique_solution() ? "YES" : "NO") << endl;
	}
	return 0;
}
