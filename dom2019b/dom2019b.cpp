/** @file dom2019b.cpp @brief Problem B �X�N���[���L�[�{�[�h �������v���O����
 *
 * @see https://icpc.iisf.or.jp/past-icpc/domestic2019/problems/ja/contest/all_ja.html#section_B
 *
 * Copyright 2019 Akira Kitajima
 *
 * @author �k�� �� <kitajima@osakac.ac.jp>
 * @date 2019/11/26 �V�K�쐬
 */
#include <iostream>
#include <string>
#include <utility>
#include <vector>
using namespace std;

/** �L�[�{�[�h��̕����̈ʒu�𓾂�B
*  @param keyboard �L�[�{�[�h
*  @param c �T������
*  @return �����̈ʒu�̍��W (���オ(0, 0))
*/
pair<int, int> key_position(const vector<string>& keyboard, char c) {
	int x = -1, y = -1;  // ���߂�ʒu�̍��W

	// 1�s�����ɒ��ׂ�B
	for (int j = 0; j < keyboard.size(); j++)
		if ((x = keyboard[j].find_first_of(c)) != string::npos) {
			y = j;
			break;
		}
	return { x, y };
}

/** (x0, y0)����(x1, y1)�܂ł̃}���n�b�^�������𓾂�B
 * @param x0 ���W
 * @param y0 ���W
 * @param x1 ���W
 * @param y1 ���W
 * @return �}���n�b�^������
 */
int Manhattan_distance(int x0, int y0, int x1, int y1) {
	return abs(x1 - x0) + abs(y1 - y0);
}

/** �L�[�{�[�h�ŉ�����͂����當����@a s �����͂ł���̂��ŏ��l�𓾂�B
 *  @param keyboard �L�[�{�[�h
 *  @param s ���肷�镶����
 *  @return �ŏ��l
 */
int min_key_count(const vector<string>& keyboard, const string& s) {
	int min_length = 0;  // �ŏ��l
	int x = 0, y = 0;  // ���݂̈ʒu
	for (char c : s) {
		pair<int, int> pos = key_position(keyboard, c);  // ���݂̎��̈ʒu

		// ���݂̎�����͂���̂ɕK�v�ȉ񐔂����߂�B
		min_length += Manhattan_distance(x, y, pos.first, pos.second) + 1;

		// ���݂̈ʒu�̍X�V
		x = pos.first;
		y = pos.second;
	}
	return min_length;  // (��)
}

int main() {
	for (;;) {
		// �X�N���[���L�[�{�[�h�̑傫���𓾂�B
		int h, w;  // �X�N���[���L�[�{�[�h�̍����A��
		cin >> h >> w;

		// �I�����肷��B
		if (h == 0 && w == 0)
			break;

		// �L�[�{�[�h��ǂݍ��ށB
		vector<string> keyboard;
		for (int i = 0; i < h; i++) {
			string r;
			cin >> r;
			keyboard.push_back(r);
		}

		// ����ɗp���镶����𓾂�B
		string s;
		cin >> s;

		// �v�Z���A���ʂ��o�͂���B
		cout << min_key_count(keyboard, s) << endl;
	}
	return 0;
}