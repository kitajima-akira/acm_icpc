/** @file dom2017a2.cpp @brief ACM ICPC 2017�N�����\�IA ���Y�N�̔���
 * @author Kitajima Akira <kitajima@osakac.ac.jp>
 * @copyright Copyright 2017 Kitajima Akira
 * @see http://icpc.iisf.or.jp/past-icpc/domestic2017/contest/all_ja.html#section_A
 */
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

using price_list_t = vector<int>;

/** 
 * �\pl�̒�����A���zm�𒴂��Ȃ�(�قȂ�)���I�ԁB
 * @param pl �l�i�̕\
 * @param m  ���z�̏��
 * @return ���v�l (0�Ȃ猩����Ȃ�����)
 */
int choice(const price_list_t& pl, int m) {
	// �\����בւ���B
	price_list_t list = pl;  // ���בւ��p�̕ϐ�
	sort(list.begin(), list.end());  // �l�i�̕\���������ɕ��בւ���B

	// ��̂����A��������i�A��������j�Ƃ��āA���ɍ��v�l��m�𒴂��Ȃ��͈͂ŁA���v�l�̍ő�l��������B
	int total_max = 0;  // ���݂�m�ȉ��ōő�̍��v�l
						// (�����l��������Ȃ������Ƃ��̒l0�Ƃ���B)
	for (size_t i = 0; (i < list.size() - 1) 
						&& (list[i] + list[i + 1] <= m); i++) { // ��ڂƂ���i��I�ԁB
		int total;		// ���v���z
		for (size_t j = i + 1; (j < list.size())
						&& ((total = list[i] + list[j]) <= m); j++) {  // ��ڂƂ���j��I�ԁB
			total_max = max(total_max, total);
		}
	}
	return total_max;
}

int main() {
	for (int n, m;  // �i���̌�n, �g���Ă悢�ő�̋��zm
		cin >> n >> m && (n > 0 || m > 0);) {
		// �l�i��ǂݎ��B
		price_list_t price_list(n);  // �l�i�\
		for (auto& a : price_list)
			cin >> a;

		// �i����I�ԁB
		int total = choice(price_list, m);

		// ���ʂ��o�͂���B
		if (total != 0)
			cout << total << endl;
		else
			cout << "NONE" << endl;
	}
	return 0;
}
