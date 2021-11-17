// dom2021c.cpp - 木変形パズル
// https://icpc.iisf.or.jp/past-icpc/domestic2021/contest/all_ja.html#section_C
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class node {
public:
	node(char c = 0):
		value(c),
		e1(nullptr),
		e2(nullptr),
		e3(nullptr),
		parent(nullptr) {}

	~node() {
		if (e1 != nullptr)
			delete e1;
		if (e2 != nullptr)
			delete e2;
		if (e3 != nullptr)
			delete e3;
	}

	// <root> ::= <node> '+' <node> '+' <node>
	//			| <node> '-' <node> '-' <node>
	void parse_root(string::const_iterator sit) {
		try {
			// 一つ目のノード
			e1 = parse_node(sit, this);

			// 演算子
			const char first_op = parse_operator(sit);

			// 二つ目のノード
			e2 = parse_node(sit, this);

			// 演算子
			value = parse_operator(sit);
			if (value != first_op)
				throw "mismatch operators in root";

			// 三つ目のノード
			e3 = parse_node(sit, this);
		}
		catch (const string& s) {
			cout << "wrong format: " << s << endl;
			exit(1);
		}
	}

	bool is_root_node() const {
		return parent == nullptr;
	}

	bool is_leaf() const {
		return '0' <= value && value <= '9';
	}

	void get_node_list(vector<node*>& node_list) {
		if (is_leaf())
			return;  // 葉は登録しない。

		// 登録
		node_list.push_back(this);

		// 子ノードを再帰的に登録
		e1->get_node_list(node_list);
		e2->get_node_list(node_list);

		if (is_root_node())
			e3->get_node_list(node_list);
	}

	// 単純な計算 (答を得るまでの準備)
	int calc() {
		if (is_leaf()) {
			// 葉の場合
			return value - '0';
		}
		else {
			// ノードの場合
			int v;
			if (value == '+')
				v = e1->calc() + e2->calc();
			else // c == '-'
				v = e1->calc() - e2->calc();

			if (is_root_node()) {
				v = (value == '+') ? v + e3->calc() : v - e3->calc();
			}
			return v;
		}
	}

	void raise_to_root() {
		if (is_root_node())
			return;  // もうできている。

		//   一つ上の節を根にする。
		parent->raise_to_root();

		//   左によせる。
		if (parent->e2 == this) {
			node* tp = parent->e1;
			parent->e1 = this;
			parent->e2 = tp;
		}
		else if (parent->e3 == this) {
			node* tp = parent->e1;
			parent->e1 = this;
			parent->e3 = tp;
		}

		//   根にする。

		// もともと根だった方を下に
		parent->e1 = parent->e2;
		parent->e2 = parent->e3;
		parent->e3 = nullptr;
		parent->parent = this;

		// 根の下だった方を上に
		e3 = parent;
		parent = nullptr;
	}

	// npからの木で最小・最大(max_resultでどちらか指定)となるスコア(計算値)を求める。
	int calc_minmax(bool max_result) {
		if (is_leaf()) {
			// 葉の場合
			return value - '0';
		}
		else {
			// ノードの場合
			int v;

			const int v1_max = e1->calc_minmax(true);
			const int v1_min = e1->calc_minmax(false);
			const int v2_max = e2->calc_minmax(true);
			const int v2_min = e2->calc_minmax(false);

			const int v3_max = is_root_node() ? e3->calc_minmax(true) : 0;
			const int v3_min = is_root_node() ? e3->calc_minmax(false) : 0;

			if (value == '+') {
				// 和は左右を入れ替えても変化しない。
				v = v1_max + v2_max + v3_max;
			}
			else {  // value == '-'
				if (!is_root_node()) {
					// 途中ノード
					// v1_max >= v1_min, v2_max >= v2_min
					if (max_result) {
						// max(v1 - v2, v2 - v1)
						v = max(v1_max - v2_min, v2_max - v1_min);
					}
					else {
						// min(v1 - v2, v2 - v1)
						v = min(v1_min - v2_max, v2_min - v1_max);
					}

				}
				else {  // 根ノード
					// v1_max >= v1_min, v2_max >= v2_min, v3_max >= v3_min
					if (max_result) {
						// max(v1 - v2 - v3, v2 - v3 - v1, v3 - v1 - v2)
						v = max(v1_max - v2_min - v3_min, v2_max - v3_min - v1_min);
						v = max(v, v3_max - v1_min - v2_min);
					}
					else {
						// min(v1 - v2 - v3, v2 - v3 - v1, v3 - v1 - v2)
						v = min(v1_min - v2_max - v3_max, v2_min - v3_max - v1_max);
						v = min(v, v3_min - v1_max - v2_max);
					}
				}
			}
			return v;
		}
	}

	int solve_puzzle() {
		// 節のリストを作る。
		vector<node*> node_list;
		get_node_list(node_list);

		int v = calc();

		// 各節を根にした場合を作る。
		for (const auto& np : node_list) {
			np->raise_to_root();
			v = max(v, np->calc_minmax(true));
		}
		return v;
	}

private:
	char value;
	node* e1;
	node* e2;
	node* e3;
	node* parent;

	// <operator> ::= '+' | '-'
	char parse_operator(string::const_iterator& sit) {
		if (*sit != '+' && *sit != '-')
			throw "invalid operator";
		return *(sit++);
	}

	// <non_root> ::= <node> <operator> <node> 
	void parse_non_root(string::const_iterator& sit) {
		// 一つ目のノード
		e1 = parse_node(sit, this);
		// 演算子
		value = parse_operator(sit);
		// 二つ目のノード
		e2 = parse_node(sit, this);
	}

	// <node> ::= '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9' | '(' <non_root> ')' 
	node* parse_node(string::const_iterator& sit, node* parentp) {
		node* np = new node();
		if ('0' <= *sit && *sit <= '9') {
			// 葉の場合
			np->value = *sit;
			++sit;  // 数字の次へ進める。
		}
		else if (*sit == '(') {
			// 途中ノードの場合
			++sit;  // 「(」の次へ進める。
			np->parse_non_root(sit);
			np->parent = parentp;
			if (*sit != ')')
				throw "missing )";
			++sit;  // 「)」の次へ進める。
		}
		else
			throw "invalid charactor";
		return np;
	}
};

int main() {
	for (string s; (cin >> s) && s != "-1";) {
		// 1行読み取り木構造を作る。
		node root;
		root.parse_root(s.begin());
		// 木構造をもとにパズルを解く。
		cout << root.solve_puzzle() << endl;
	}
	return 0;
}
