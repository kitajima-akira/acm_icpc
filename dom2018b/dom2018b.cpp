// dom2018b.cpp - ACM ICPC 2018�N�����\�IB - �܂莆
// https://icpc.iisf.or.jp/past-icpc/domestic2018/contest/all_ja.html#section_B
// Copyright 2023 Kitajima Akira
// Author: Kitajima Akira <kitajima@osakac.ac.jp>

#include <iostream>
#include <vector>
using namespace std;

// �u�܂莆�v�������N���X
class origami {
public:
	origami(int n, int m) :
		// �܂�Ԃ��ۂ̓r�������ł͂ݏo�����Ƃ��l������2�{�̑傫���ɂ���B
		paper(2 * m, vector<int>(2 * n, 0)),  
		current_width(n),
		current_height(m) {
		// �e�̈�̖�����ݒ肷��B
		for (int j = 0; j < current_height; j++)
			for (int i = 0; i < current_width; i++)
				paper[j][i] = 1;
	}

	// �܂鏈�����s���B
	void fold(int d, int c) {
		if (d == 1)
			fold_vertically(c);
		else
			fold_horizontally(c);
	}

	// (���̊J��)������Ԃ��B
	int punch(int x, int y) const {
		return paper[y][x];
	}

	// �f�o�b�O�p�ɁA�܂����l�q���o�͂���B
	friend std::ostream& operator<<(std::ostream& os, const origami& o) {
		for (auto line : o.paper) {
			os << ": ";
			for (int piece : line)
				os << " " << piece;
			os << endl;
		}
		return os;
	}

private:
	vector<vector<int>> paper;  // �e�̈�̖���
	int current_width, current_height;  // ���݂̕��A����
	
	// �c�ɐ܂�B
	void fold_vertically(int c) {
		for (auto& line : paper) {
			// �܂�����̃f�[�^�ɐ܂��������̃f�[�^��������B
			for (int i = 0; i < c; i++) {
				const int ii = c + (c - i) - 1;  // i�Ԗڂ̍s�Əd�Ȃ�s
				line[ii] += line[i];
			}
			// �܂������������V�t�g����B
			for (int i = 0; i < line.size() - c; i++)
				line[i] = line[i + c];
		}

		current_width = 2 * c <= current_width ? current_width - c : c;
		// �Ȃ��Ȃ���������0�Ŗ��߂�B
		for (auto& line : paper)
			for (int i = current_width; i < line.size(); i++)
				line[i] = 0;
	}

	// ���ɐ܂�B
	void fold_horizontally(int c) {
		for (int i = 0; i < c; i++) {
			const int ii = c + (c - i) - 1;  // i�Ԗڂ̍s�Əd�Ȃ�s
			// �܂�����̃f�[�^�ɐ܂��������̃f�[�^��������B
			for (int j = 0; j < paper[i].size(); j++)
				paper[ii][j] += paper[i][j];
		}

		// �܂������������V�t�g����B
		for (int i = 0; i < paper.size() - c; i++)
			paper[i] = paper[i + c];

		current_height = 2 * c <= current_height ? current_height - c : c;
		// �Ȃ��Ȃ���������0�Ŗ��߂�B
		for (int j = current_height; j < paper.size(); j++)
			for (auto& piece : paper[j])
				piece = 0;
	}
};

int main() {
	for (int n, m, t, p; cin >> n >> m >> t >> p && (n > 0 || m > 0 || t >> 0 || p > 0);) {
		// �܂莆�f�[�^��p�ӂ���B
		origami o(n, m);
		
		// ���͂̎w��ǂ���ɐ܂�B
		for (int i = 0; i < t; i++) {
			int d, c;
			cin >> d >> c;
			o.fold(d, c);
		}

		// ���͂̎w��ǂ���Ɍ����J����B
		int num_holes = 0;
		for (int i = 0; i < p; i++) {
			int x, y;
			cin >> x >> y;
			// ���ʂ��o�͂���B
			cout << o.punch(x, y) << endl;
		}
	}

	return 0;
}
