// dom2022b.cpp - ACM ICPC 2022 国内予選B 誰ひとり取り残さない
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
		// まず，同じ番号のカードを 2 枚持っているプレイヤーは，それらのカードを場に捨てる．
		cards[p] = (c1 == c2) ? vector<int>{} : vector<int>{ c1, c2 };
	}

	int play() {
		//もし万一全員がカードを捨ててしまったら，ここでゲームは終了する．そうでなければ，ゲームが終了するまで，以下の動作を繰り返し行う．これ以降，プレイヤー p に対して，「次のプレイヤー」は，その時点でまだ手札が 1 枚以上残っているプレイヤーのうち，時計回りの方向で p から一番近い者を意味する．
		if (card_count() == 0)
			return 0;

		// 次のようにプレイヤー p を選ぶ，
		//1 回目は P1 を選ぶ．ただし，P1 の手札が残っていなければ，P1 の次のプレイヤーを選ぶ．
		int p = -1;
		int count = 0;
		do {
			// それ以降は，前の回で選ばれたプレイヤーの次のプレイヤーを選ぶ．
			p = next_player(p);
			
			// p の次のプレイヤー p′ は，p の手札を見て，その中で最小の番号のカードを引く．
			int drawn_card = draw(p);
			count++;
			int pp = next_player(p);
			// p′ が同じ番号のカードを 2 枚持つようになった場合には，その 2 枚のカードを場に捨てる．
			if (!remove_same_cards(pp, drawn_card))
				cards[pp].push_back(drawn_card);
		} while (card_count() != 0);  // この結果，すべてのプレイヤーのカードがなくなったら，ゲームは終了する．
		return count;
	}

private:
	// 全員のカードの残り枚数を数える。
	int card_count() const {
		int count = 0;
		for (const auto& player_cards : cards) {
			count += player_cards.size();
		}
		return count;
	}

	// p の手札を見て，その中で最小の番号のカードを引く．
	int draw(int p) {
		auto& cardsp = cards[p];  // pの手札
		// 最小の番号を見つける。
		auto min_it = min_element(cardsp.begin(), cardsp.end());
		int min_number = *min_it;  // 最小番号
		// 「引く」ので手札から除く。
		cardsp.erase(min_it);
		return min_number;
	}

	// pの次のプレーヤーを選ぶ。
	int next_player(int p) const {
		do {
			// pの番号を次に更新する。
			if (++p >= cards.size())
				p = 0;
		} while (cards[p].size() == 0);  // pの手札の枚数が0ならやり直し
		return p;
	}

	// idのプレーヤーがnew_cardを持っていたら、それを捨てる。
	// 捨てたらtrueを返す。
	bool remove_same_cards(int id, int new_card) {
		for (auto it = cards[id].begin(); it != cards[id].end(); ++it) {
			if (*it == new_card) {
				cards[id].erase(it);
				return true;
			}
		}
		return false;
	}

	vector<vector<int>> cards;  // プレイヤーごとの手札
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
