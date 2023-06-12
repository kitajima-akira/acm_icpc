// dom2017c.cpp - ACM ICPC2017国内予選C 池のある庭園
// https://icpc.iisf.or.jp/past-icpc/domestic2017/contest/all_ja.html#section_C
// Copyright 2023 Kitajima Akira
// Author: Kitajima Akira <kitajima@osakac.ac.jp>

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

// 位置 (行, 列)
using position = pair<int, int>;

class pond {
public:
	pond (int d, int w):
		depth(d),
		width(w),
		hmap(d, vector<int>(w)) {
	}

	// 指定の位置に標高データをセットする。
	void set(const position& p, int val) {
		hmap[p.first][p.second] = val;
	}

	int capacity() {
		int c = 0;
		for (int bj = 0; bj < depth - 2; bj++) {
			for (int bi = 0; bi < width - 2; bi++) {
				// 開始位置 bj行目, bi列目
				for (int ej = bj + 2; ej < depth; ej++) {
					for (int ei = bi + 2; ei < width; ei++) {
						// 開始位置 ej行目, ei列目
						c = max(c, capacity({ bj, bi }, { ej, ei }));
					}
				}
			}
		}
		return c;
	}

private:
	int depth;
	int width;
	vector<vector<int>> hmap;

	// 枠の標高の最小値を求める。
	int outermost_height(const position& bp, const position& ep) {
		// bpの行の最小値を求める。
		auto bp_begin = hmap[bp.first].begin() + bp.second;
		auto bp_end = hmap[bp.first].begin() + ep.second + 1;
		auto min_bp = min_element(bp_begin, bp_end);
		// epの行の最小値を求める。
		auto ep_begin = hmap[ep.first].begin() + bp.second;
		auto ep_end = hmap[ep.first].begin() + ep.second + 1;
		auto min_ep = min_element(ep_begin, ep_end);

		int height = min(*min_bp, *min_ep);  // 枠の標高の最小値

		// bp行の次からep行の前までの両端の最小値を求める。
		for_each(hmap.begin() + bp.first + 1, hmap.begin() + ep.first, [&](const auto& line) {
			int h = min(line[bp.second], line[ep.second]);  // 左端と右端の小さい方
			height = min(height, h);
		});
		return height;
	}

	int capacity(const position& bp, const position& ep) {
		int origin = outermost_height(bp, ep);  // 容量を計る基点の高さ
		int total = 0;  // 全容量
		try {
			// 各セルの容量を順に求める。
			for_each(hmap.begin() + bp.first + 1, hmap.begin() + ep.first, [&](const auto& line) {
				for_each(line.begin() + bp.second + 1, line.begin() + ep.second, [&](const auto& e) {
					int c = origin - e;  // そのセルの容量
					if (e >= origin) {  // 1箇所でも周り以上なら
						throw 0;  // 溢れる。
					}
					total += c;
				});
			});
		}
		catch (int c) {  // 溢れたときの例外処理
			return c;
		}

		return total;
	}
};

int main() {
	for (int d, w;  // 用地の奥行き, 幅 
		cin >> d >> w, d > 0 && w > 0;) {
		pond p(d, w);
		for (int j = 0; j < d; j++)
			for (int i = 0; i < w; i++) {
				int e;  // 標高
				cin >> e;
				p.set({ j, i }, e);
			}
		cout << p.capacity() << endl;
	}
	return 0;
}
