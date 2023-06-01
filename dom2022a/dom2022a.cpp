// dom2022a.cpp - ACM ICPC 2022 �����\�IA �����s�[�N�̉�
// https://icpc.iisf.or.jp/past-icpc/domestic2022/contest/all_ja.html#section_A

#include <iostream>
#include <vector>
using namespace std;

// �f�[�^�̕���a�̃s�[�N�������߂�B
int count_peak(const vector<int>& a) {
	int peak_count = 0;
	// �O�̕��� i, i + 1, i + 2 ��������炵�ď��ɔ�r
	for (size_t i = 0; i + 2 < a.size(); i++)
		if (a[i + 1] > a[i] && a[i + 1] > a[i + 2])  // a[i + 1] ���O����傫��
			peak_count++;

	return peak_count;
}

int main() {
	for (int n; cin >> n && n > 0;) {  // n�̓f�[�^�̌�
		vector<int> positive_cases(n);  // �����Ƃ̐V�K�z���Ґ�
		// �f�[�^��ǂݍ��ށB
		for (auto& v : positive_cases)  // v�͊i�[����v�f
			cin >> v;

		cout << count_peak(positive_cases) << endl;
	}
	return 0;
}
