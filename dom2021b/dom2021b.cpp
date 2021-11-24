// dom2021b.cpp - �S�܂��v�Z�p�Y��
// https://icpc.iisf.or.jp/past-icpc/domestic2021/contest/all_ja.html#section_B

#include <iostream>
#include <vector>
using namespace std;

class hundo_puzzle {
public:
	hundo_puzzle(int width, int height, int min_value) :
		h(width, min_value - 1),
		v(height, min_value - 1),
		grid(height, vector<int>(width, min_value - 1)),
		not_determined_yet(min_value - 1) {
		// ��[�̐��̈�ԍ��� 0 �ƌ��߂Ă���
		h[0] = 0;
	}

	void set_grid(int x, int y, int n) {
		grid[y - 1][x - 1] = n;
	}

	bool has_unique_solution() {
		fix();
		return check_unique();
	}
private:
	vector<int> h;
	vector<int> v;
	vector<vector<int>> grid;
	int not_determined_yet;

	// ��[��x�̈ʒu���m�肵���Ƃ��ɁA�܂��m�肵�Ă��Ȃ����[�𖄂߁A����ɑ΂��čċA�I�ɖ��߂�B
	void fix_v(int x) {
		for (size_t y = 0; y < v.size(); y++) {
			if (v[y] == not_determined_yet && grid[y][x] != not_determined_yet) {
				v[y] = grid[y][x] - h[x];
				fix_h(y);
			}
		}
	}

	void fix_h(int y) {
		for (size_t x = 0; x < h.size(); x++) {
			if (h[x] == not_determined_yet && grid[y][x] != not_determined_yet) {
				h[x] = grid[y][x] - v[y];
				fix_v(x);
			}
		}
	}

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

	bool check_unique() const {
		// ��[�`�F�b�N
		auto result = find(h.begin(), h.end(), not_determined_yet);
		if (result != h.end())
			return false;

		// ���[�`�F�b�N
		result = find(v.begin(), v.end(), not_determined_yet);
		return result == v.end();
	}
};

int main() {
	for (int w, h; (cin >> w >> h) && (w || h); ) {
		hundo_puzzle hp(w, h, -100);
		const int k = w + h - 1;
		for (int i = 0; i < k; i++) {
			int x, y, n;
			cin >> x >> y >> n;
			hp.set_grid(x, y, n);
		}

		cout << (hp.has_unique_solution() ? "YES" : "NO") << endl;
	}
	return 0;
}
