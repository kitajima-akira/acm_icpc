// dom2020c.cpp - �ו�
// https://icpc.iisf.or.jp/past-icpc/domestic2020/contest/all_ja.html#section_C

#include <algorithm>
#include <iostream>
#include <cmath>
using namespace std;

typedef long long int longint;  // �f�[�^�Ƃ��Ď�舵�����l�̌^

// n�̖񐔂̂����Ad���傫�����̖񐔂����߂�B
// d���傫�����̖񐔂��Ȃ����d + 1��Ԃ��B
// ****���Ԃ������肷���Ė���****
longint next_divisor(longint n, longint d) {
	// d���傫�����̖񐔂��Ȃ��ꍇ
	if (n <= d)
		return d + 1;

	// d���傫��n�ȉ��̖񐔂�������B
	for (longint i = d + 1; i <= n; i++)
		if (n % i == 0)
			return i;

	// n�ȊO�̖񐔂�������Ȃ��ꍇ
	return n;
}

// p = w * d * h����s = w + d + h�̍ŏ��l�𓾂�B
longint estimate(longint p) {
	longint s_min = p + 2;  // p * 1 * 1 ��1�ԑ傫���Ȃ�͂��Ȃ̂ŏ����l�ɐݒ�
	// w��p�̖񐔂��ׂĂ̏ꍇ�Ŏ����B
	cout << "sqrt(p) = " << static_cast<longint>(sqrt(p)) << endl;
	for (longint w = 1; w <= p; w = next_divisor(p, w)) {
		const longint dxh = p / w;  // d * h

		// w >= d >= h >= 1�̂Ƃ�
		// w * w * w >= w * d * h
		// w * w >= d * h
		// w >= sqrt(d * h)
		// ���̂Ƃ��̂�d, h��񋓂���B
		const auto sqrt_dxh = sqrt(dxh);
//		cout << w << "(" << dxh << ((w >= sqrt_dxh) ? " <= " : " > ") << w * w << ")" << endl;
		if (w < sqrt_dxh)
			continue;
		cout << "case w = " << w << ", d * h = " << dxh << endl;
		// h��p / w (= d * h)�̖񐔂��ׂĂ̏ꍇ�Ŏ����B
		// w >= d >= h > 0�̏ꍇ�݂̂�����A
		// d >= h
		// d * h >= h * h
		// sqrt(d * h) >= h ...����2
		for (longint h = 1; h <= sqrt_dxh && h <= w; h = next_divisor(dxh, h)) {
				// d�����܂�B
			const longint d = dxh / h;
			cout << "\tcase h = " << h << ", d = " << d << endl;
			if (d > w)
				continue;
			cout << "\t\tcase d = " << d << endl;
			// s�����߂�B
			const longint s = w + d + h;
			if (s_min > s)
				cout << "s = " << s << " = " << w << " + " << d << " + " << h << endl;
			// s_min���X�V����B
			s_min = min(s_min, s);
		}
	}
	return s_min;
}

int main() {
	for (longint p; // ��
		(cin >> p) && (p != 0);) {	
		cout << estimate(p) << endl;
	}
	return 0;
}
