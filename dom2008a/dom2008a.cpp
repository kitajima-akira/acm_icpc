/** @file dom2008a.cpp @brief Equal Total Scores
 *  @author Kitajima Akira
 *  @date 2011/05/16
 *  @see https://icpc.iisf.or.jp/past-icpc/domestic2008/problems/all_ja.html
 *  �𓚎��Ԃ̖ڈ� 40��
 */

#include <iostream>
#include <set>
#include <algorithm>
using namespace std;

/** �𓚂����߂�B */
class solver {
public:
    /** 
	 * ��̃J�[�h�����ŏ���������B
     * @param n 1�l�ڂ̃J�[�h�̖���
     * @param m 2�l�ڂ̃J�[�h�̖���
	 */
    solver (int n, int m): diff(0) {}

    /**
	 * ���Y�̃J�[�h����w�肷��B
	 * �Ԏq�Ƃ̍����v�Z���Ă����B
     * @param �J�[�h�̔ԍ�
	 */
    void add2Taro(int datum) {
		taro.insert(datum);
		diff += datum;
    }

    /** 
	 * �Ԏq�̃J�[�h����w�肷��B
	 * ���Y�Ƃ̍����v�Z���Ă����B
     * @param �J�[�h�̔ԍ�
	 */
    void add2Hanako(int datum) {
		hanako.insert(datum);
		diff -= datum;
    }

    /**
	 * �������߂�B
	 * ���߂�����getTaro2Hanako()��getHanako2Taro()�œ�����B
	 * @returns �������܂������ǂ����B
	 * @see solver::getTaro2Hanako(), solver::getHanako2Taro()
	 */
    bool solve() {
		if (diff % 2)
			// ������Ȃ�������܂�Ȃ��B
			return false;

		int a = diff / 2;
		// �a���ŏ��ƂȂ�悤���Y�̏������l���珇�ɒ��ׂ�B
		for (auto t : taro) {
			hanako2taro = t - a; // ��������ׂ��l�͌��܂��Ă���B
			if (hanako.find(hanako2taro) != hanako.end()) {
				// �Ԏq����������ׂ��l�������Ă����B
				taro2hanako = t;
				return true;
			}
		}
		return false;
    }

    /** 
	 * ���Y����Ԏq�ɓn���J�[�h�̒l�𓾂�B
     * @returns �J�[�h�̒l
	 */
    int getTaro2Hanako() const {
		return taro2hanako;
    }

    /**
	 * �Ԏq���瑾�Y�ɓn���J�[�h�̒l�𓾂�B
     * @returns �J�[�h�̒l
	 */
    int getHanako2Taro() const {
		return hanako2taro;
    }

private:
    using cards = set<int>;
    using cards_cit = set<int>::const_iterator;
    cards taro, hanako;		// ���Y�̎D, �Ԏq�̎D
    int diff;			// �l�̍�
    int taro2hanako, hanako2taro; // ���Y����Ԏq�ɓn���D�A���̋t
};

int main() {
    for (int n, m; cin >> n >> m, n || m;) {
		solver s(n, m);
		int datum;
		while (n--) {
			cin >> datum;
			s.add2Taro(datum);
		}
		while (m--) {
			cin >> datum;
			s.add2Hanako(datum);
		}

		if (s.solve())
			cout << s.getTaro2Hanako() << ' ' << s.getHanako2Taro() << endl;
		else
			cout << -1 << endl;
    }

    return 0;
}
