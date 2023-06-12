// dom2017c.cpp - ACM ICPC2017�����\�IC �r�̂���뉀
// https://icpc.iisf.or.jp/past-icpc/domestic2017/contest/all_ja.html#section_C
// Copyright 2023 Kitajima Akira
// Author: Kitajima Akira <kitajima@osakac.ac.jp>

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

// �ʒu (�s, ��)
using position = pair<int, int>;

class pond {
public:
	pond (int d, int w):
		depth(d),
		width(w),
		hmap(d, vector<int>(w)) {
	}

	// �w��̈ʒu�ɕW���f�[�^���Z�b�g����B
	void set(const position& p, int val) {
		hmap[p.first][p.second] = val;
	}

	int capacity() {
		int c = 0;
		for (int bj = 0; bj < depth - 2; bj++) {
			for (int bi = 0; bi < width - 2; bi++) {
				// �J�n�ʒu bj�s��, bi���
				for (int ej = bj + 2; ej < depth; ej++) {
					for (int ei = bi + 2; ei < width; ei++) {
						// �J�n�ʒu ej�s��, ei���
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

	// �g�̕W���̍ŏ��l�����߂�B
	int outermost_height(const position& bp, const position& ep) {
		// bp�̍s�̍ŏ��l�����߂�B
		auto bp_begin = hmap[bp.first].begin() + bp.second;
		auto bp_end = hmap[bp.first].begin() + ep.second + 1;
		auto min_bp = min_element(bp_begin, bp_end);
		// ep�̍s�̍ŏ��l�����߂�B
		auto ep_begin = hmap[ep.first].begin() + bp.second;
		auto ep_end = hmap[ep.first].begin() + ep.second + 1;
		auto min_ep = min_element(ep_begin, ep_end);

		int height = min(*min_bp, *min_ep);  // �g�̕W���̍ŏ��l

		// bp�s�̎�����ep�s�̑O�܂ł̗��[�̍ŏ��l�����߂�B
		for_each(hmap.begin() + bp.first + 1, hmap.begin() + ep.first, [&](const auto& line) {
			int h = min(line[bp.second], line[ep.second]);  // ���[�ƉE�[�̏�������
			height = min(height, h);
		});
		return height;
	}

	int capacity(const position& bp, const position& ep) {
		int origin = outermost_height(bp, ep);  // �e�ʂ��v���_�̍���
		int total = 0;  // �S�e��
		try {
			// �e�Z���̗e�ʂ����ɋ��߂�B
			for_each(hmap.begin() + bp.first + 1, hmap.begin() + ep.first, [&](const auto& line) {
				for_each(line.begin() + bp.second + 1, line.begin() + ep.second, [&](const auto& e) {
					int c = origin - e;  // ���̃Z���̗e��
					if (e >= origin) {  // 1�ӏ��ł�����ȏ�Ȃ�
						throw 0;  // ����B
					}
					total += c;
				});
			});
		}
		catch (int c) {  // ��ꂽ�Ƃ��̗�O����
			return c;
		}

		return total;
	}
};

int main() {
	for (int d, w;  // �p�n�̉��s��, �� 
		cin >> d >> w, d > 0 && w > 0;) {
		pond p(d, w);
		for (int j = 0; j < d; j++)
			for (int i = 0; i < w; i++) {
				int e;  // �W��
				cin >> e;
				p.set({ j, i }, e);
			}
		cout << p.capacity() << endl;
	}
	return 0;
}
