// dom2021d2.cpp - ���D�z��
// https://icpc.iisf.or.jp/past-icpc/domestic2021/contest/all_ja.html
// ���I�v��@�ŉ���
// https://icpc.iisf.or.jp/past-icpc/domestic2021/commentaries.html#D

#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

// �^������q���̍ő吔�����߂�B
int get_max_number_of_kids(vector<int> b) {
	// ���I�v��@�𗘗p���邽�߁A���בւ���B
	sort(b.begin(), b.end());

	// dp[k][x][y] = "{b1, ..., bk} �� 3 �������Ęa�� (x, y, (��ki=1 bi)-x-y) �ƂȂ�悤�ɂł��邩�ۂ�"
	// �܂�Ak�Ԗڂ܂ł̃f�[�^�ŁA�a��x, y�Ƃ��̎c��ɕ����ł��邩
	const int SUM_MAX = 50 * b.size();
	vector<vector<vector<int>>> dp(b.size(), vector<vector<int>>(SUM_MAX, vector<int>(SUM_MAX, false)));

	// �����ݒ�
	// b[0]�����̏ꍇ�ɁA3��������̂�3�ʂ� (�r���ł�0�̕����������Ă��悢�B)
	dp[0][0][0] = true;
	dp[0][b[0]][0] = true;
	dp[0][0][b[0]] = true;
	int sum_b = 0;  // (��ki=1 bi)

	// �\�𖄂߂� (�����ł���g���������߂�B)
	for (size_t i = 0; i < b.size() - 1; i++) {
		sum_b += b[i];
		const int bb = b[i + 1];  // �V����b

		// dp[i][x][y]�ƂȂ邷�ׂĂ̑g���������Ƃ�dp[i + 1]�����߂�B
		for (int x = 0; x <= sum_b; x++) {
			for (int y = 0; y <= x; y++) {
				if (dp[i][x][y]) {  // x, y�ŕ����\�Ȃ�
					dp[i + 1][x][y] = true;
					dp[i + 1][x + bb][y] = true;
					dp[i + 1][x][y + bb] = true;
				}
			}
		}
	}

	// �����\��x, y�Ƃ��̎c��ɑ΂��āA�a�̍ŏ��l�̍ő�l�����߂�B
	int k_max = 0;
	const int i = b.size() - 1;
	sum_b += b[i];
	for (int x = 1; x < sum_b; x++) {
		for (int y = 1; y <= x; y++) {
			if (dp[i][x][y]) {  // x, y�ŕ����\�Ȃ�
				const int sum_min = min(min(sum_b - x - y, x), y);  // �a�̍ŏ��l
				k_max = max(k_max, sum_min);
			}
		}
	}

	return k_max;
}

int main() {
	for (int n; (cin >> n) && (n > 0); ) {
		vector<int> b(n);
		for (auto& i : b)
			cin >> i;
		cout << get_max_number_of_kids(b) << endl;
	}
	return 0;
}
