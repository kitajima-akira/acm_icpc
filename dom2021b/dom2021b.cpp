// dom2021b.cpp - 百ます計算パズル
// https://icpc.iisf.or.jp/past-icpc/domestic2021/contest/all_ja.html#section_B

#include <iostream>
#include <vector>
using namespace std;

// 百ます計算パズルを解くためのクラス
class hundo_puzzle {
public:
	// ます目の幅width・高さheightと、値の最小値min_valueを指定して初期設定する。
	// 「min_value - 1」の値を未確定(空白のます)を表すのに用いる。
	hundo_puzzle(int width, int height, int min_value) :
		h(width, min_value - 1),
		v(height, min_value - 1),
		grid(height, vector<int>(width, min_value - 1)),
		not_determined_yet(min_value - 1) {
		// 上端の数の一番左を 0 と決めておく
		h[0] = 0;
	}

	// ますの指定した位置(x, y)に値nを設定する。
	void set_grid(int x, int y, int n) {
		grid[y - 1][x - 1] = n;
	}

	// すべてのますが一意に決まるか判定する。
	// 結果の真偽値を返す。
	bool has_unique_solution() {
		fix();
		return check_unique();
	}

private:
	vector<int> h;  // 上端
	vector<int> v;  // 左端
	vector<vector<int>> grid;  // ます目
	int not_determined_yet;  // 未確定であることを表す値

	// 上端のxの位置が確定したときに、まだ確定していない左端を埋め、それに対して再帰的に埋める。
	void fix_v(int x) {
		for (size_t y = 0; y < v.size(); y++) {
			if (v[y] == not_determined_yet && grid[y][x] != not_determined_yet) {
				v[y] = grid[y][x] - h[x];
				fix_h(y);
			}
		}
	}

	// 左端のyの位置が確定したときに、まだ確定していない上端を埋め、それに対して再帰的に埋める。
	void fix_h(int y) {
		for (size_t x = 0; x < h.size(); x++) {
			if (h[x] == not_determined_yet && grid[y][x] != not_determined_yet) {
				h[x] = grid[y][x] - v[y];
				fix_v(x);
			}
		}
	}

	// 今の状態から一意に決まる上端・左端を埋める。
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

	// 上端・左端について、すべて埋まっているかを確認する。
	// 結果の真偽値を返す。
	bool check_unique() const {
		return find(h.begin(), h.end(), not_determined_yet) == h.end()   // 上端チェック
			&& find(v.begin(), v.end(), not_determined_yet) == v.end();  // 左端チェック
	}
};

int main() {
	for (int w, h; (cin >> w >> h) && (w || h); ) {   // 幅・高さを読み込む。
		// 初期設定する。
		hundo_puzzle hp(w, h, -100);

		// 各ますのデータを読み込む。
		const int k = w + h - 1;
		for (int i = 0; i < k; i++) {
			int x, y, n;
			cin >> x >> y >> n;
			hp.set_grid(x, y, n);
		}

		// 答を求め、出力する。
		cout << (hp.has_unique_solution() ? "YES" : "NO") << endl;
	}
	return 0;
}
