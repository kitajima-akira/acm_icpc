// dom2021d.cpp - ���D�z��
// https://icpc.iisf.or.jp/past-icpc/domestic2021/contest/all_ja.html

#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

// ���D��������B
// ���̃|�[���ԍ���Ԃ��B
int remove_baloon(vector<int>& b, size_t i) {
	if (--b[i] == 0)
		b.erase(b.begin() + i);
	else
		i++;
	return i;
}

// �^������q���̐������߂�B
int get_number_of_kids(vector<int> b) {
	int k = 0;  // �^������q���̐�
	while (b.size() >= 3) {
		k++;
		size_t i = 0;  // ���݂̃|�[��
		// 1��
		i = remove_baloon(b, i);
		// 2��
		i = remove_baloon(b, i);
		// 3��
		i = remove_baloon(b, i);
	}

	return k;
}

// �^������q���̍ő吔�����߂�B
int get_max_number_of_kids(vector<int> b) {
	// permutation�𗘗p���邽�߁A���בւ���B
	sort(b.begin(), b.end());

	// �S�p�^�[����permutation�ŗ񋓂��A�ő�l�����߂�B
	int k_max = 0;  // ���߂�ő�l
	do {
		k_max = max(k_max, get_number_of_kids(b));
	} while (next_permutation(b.begin(), b.end()));
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
