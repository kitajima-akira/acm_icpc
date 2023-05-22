// dom2022c-ng.cpp - [ACM ICPC 2022 C] ICPC �Ɍ����Ă̗��K���� (���s��)
// https://icpc.iisf.or.jp/past-icpc/domestic2022/contest/all_ja.html#section_C
#include <iostream>
#include <utility>
using namespace std;

// n���A�����̒l���v�Z����B
// ���ǂ���Ɍv�Z����B
// ���Ԃ�������B
long long int get_power1(int n) {
	long long int p = 0;

	for (int k = 1; k <= n; k++)
		p += static_cast <long long int>(2 * k - 1);
	return p;
}

// �[���D��T����1���ڂ��珇�ɗ��K����ꍇ�Ƃ��Ȃ��ꍇ�Ƃ����߂Ĕ\�͒l�̑傫�������̗p����B
// ���̕��@�ł́An + m���傫���Ǝ��Ԃ������肷���ďI���Ȃ��B
// 
// n ���K����
// m �x������
// kt �A�����K����
// kr �A���x������
long long int get_max_power(int n, int m, int kt = 1, int kr = 1) {
	if (n <= 0 && m <= 0)
		// ���ׂĂ̓������I��
		return 0;

	long long int power;
	if (n > 0) {
		// ���K����ꍇ
		power = 2 * kt - 1 + get_max_power(n - 1, m, kt + 1, 1);
		if (m > 0) {
			// ���K�����x������ꍇ
			const long long int power_r = -(2 * kr - 1) + get_max_power(n, m - 1, 1, kr + 1);
			power = max(power, power_r);
		}
	}
	else {
		// ���K�ł��Ȃ��ꍇ (�x��)
		power = - (2 * kr - 1) + get_max_power(n, m - 1, 1, kr + 1);
	}
	
	// �f�o�b�O�p
	return power;
}

int main() {
	for (int n, m; (cin >> n >> m) && (n > 0 || m > 0);)
		cout << get_max_power(n, m) << endl;

	return 0;
}
