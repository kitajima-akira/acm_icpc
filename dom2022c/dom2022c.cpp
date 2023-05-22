// dom2022c.cpp - [ACM ICPC 2022 C] ICPC �Ɍ����Ă̗��K����
// https://icpc.iisf.or.jp/past-icpc/domestic2022/contest/all_ja.html#section_C
#include <iostream>
#include <utility>
using namespace std;

// n���A������ICPC�\�͂𓾂�B
long long int get_power(long long int n) {
	return n * n;
}

// ���Kn���A�x��m���̏ꍇ��ICPC�\�͂̍ő�l�𓾂�B
long long int get_max_power(int n, int m) {
	if (n == m)
		return 0;

	if (n == 0)
		return -get_power(m);

	// o(���K)�͘A���̕��������B
	// x(�x��)�͂Ȃ�ׂ��ϓ��ɕ����������������B

	// 2���������ꍇ
	// xxxxxoooooooxxxx
	int r = m % 2;
	int d = m / 2;
	long long int p = -get_power(d) + get_power(n) - get_power(d + r);

	// x�����܂������ł���(���܂�Ȃ�)�ƁA�ׂ����킯�Ă����悭�Ȃ�ꍇ������B
	// xxx o xxx oooooo xxx

	// x��s��������B
	for (int s = 3; s <= n + 1 && s <= m; s++) {
		r = m % s;
		d = m / s;
		long long int new_p = -(s - r) * get_power(d)  // ���܂��ĂȂ�x
			+ (s - 2) * get_power(1)  // �����̂��߂�o
			+ get_power(n - (s - 2))  // o�̍ő剻
			- r * get_power(d + 1);  // ���܂�̂���x

		if (new_p > p)
			p = new_p;
		else
			// �ǂ��Ȃ�Ȃ���΂����ŏI���B
			return p;
	}
	return p;
}

int main() {
	for (int n, m; (cin >> n >> m) && (n > 0 || m > 0);)
		cout << get_max_power(n, m) << endl;

	return 0;
}
