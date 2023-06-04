// dom2022b.cpp - ACM ICPC 2022 �����\�IB �N�ЂƂ���c���Ȃ�
// https://icpc.iisf.or.jp/past-icpc/domestic2022/contest/all_ja.html#section_B
// Copyright 2023 Kitajima Akira
// Author: Kitajima Akira <kitajima@osakac.ac.jp>

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

class card_game {
public:
	card_game(int n): 
		cards(n) {
	}

	void add(int p, int c1, int c2) {
		// �܂��C�����ԍ��̃J�[�h�� 2 �������Ă���v���C���[�́C�����̃J�[�h����Ɏ̂Ă�D
		cards[p] = (c1 == c2) ? vector<int>{} : vector<int>{ c1, c2 };
	}

	int play() {
		//��������S�����J�[�h���̂ĂĂ��܂�����C�����ŃQ�[���͏I������D�����łȂ���΁C�Q�[�����I������܂ŁC�ȉ��̓�����J��Ԃ��s���D����ȍ~�C�v���C���[ p �ɑ΂��āC�u���̃v���C���[�v�́C���̎��_�ł܂���D�� 1 ���ȏ�c���Ă���v���C���[�̂����C���v���̕����� p �����ԋ߂��҂��Ӗ�����D
		if (card_count() == 0)
			return 0;

		// ���̂悤�Ƀv���C���[ p ��I�ԁC
		//1 ��ڂ� P1 ��I�ԁD�������CP1 �̎�D���c���Ă��Ȃ���΁CP1 �̎��̃v���C���[��I�ԁD
		int p = -1;
		int count = 0;
		do {
			// ����ȍ~�́C�O�̉�őI�΂ꂽ�v���C���[�̎��̃v���C���[��I�ԁD
			p = next_player(p);
			
			// p �̎��̃v���C���[ p�� �́Cp �̎�D�����āC���̒��ōŏ��̔ԍ��̃J�[�h�������D
			int drawn_card = draw(p);
			count++;
			int pp = next_player(p);
			// p�� �������ԍ��̃J�[�h�� 2 �����悤�ɂȂ����ꍇ�ɂ́C���� 2 ���̃J�[�h����Ɏ̂Ă�D
			if (!remove_same_cards(pp, drawn_card))
				cards[pp].push_back(drawn_card);
		} while (card_count() != 0);  // ���̌��ʁC���ׂẴv���C���[�̃J�[�h���Ȃ��Ȃ�����C�Q�[���͏I������D
		return count;
	}

private:
	// �S���̃J�[�h�̎c�薇���𐔂���B
	int card_count() const {
		int count = 0;
		for (const auto& player_cards : cards) {
			count += player_cards.size();
		}
		return count;
	}

	// p �̎�D�����āC���̒��ōŏ��̔ԍ��̃J�[�h�������D
	int draw(int p) {
		auto& cardsp = cards[p];  // p�̎�D
		// �ŏ��̔ԍ���������B
		auto min_it = min_element(cardsp.begin(), cardsp.end());
		int min_number = *min_it;  // �ŏ��ԍ�
		// �u�����v�̂Ŏ�D���珜���B
		cardsp.erase(min_it);
		return min_number;
	}

	// p�̎��̃v���[���[��I�ԁB
	int next_player(int p) const {
		do {
			// p�̔ԍ������ɍX�V����B
			if (++p >= cards.size())
				p = 0;
		} while (cards[p].size() == 0);  // p�̎�D�̖�����0�Ȃ��蒼��
		return p;
	}

	// id�̃v���[���[��new_card�������Ă�����A������̂Ă�B
	// �̂Ă���true��Ԃ��B
	bool remove_same_cards(int id, int new_card) {
		for (auto it = cards[id].begin(); it != cards[id].end(); ++it) {
			if (*it == new_card) {
				cards[id].erase(it);
				return true;
			}
		}
		return false;
	}

	vector<vector<int>> cards;  // �v���C���[���Ƃ̎�D
};

int main() {
	for (int n; cin >> n && n > 0; ) {
		card_game g(n);
		for (int i = 0; i < n; i++) {
			int c1, c2;
			cin >> c1 >> c2;
			g.add(i, c1, c2);
		}
		cout << g.play() << endl;
	}
	return 0;
}
