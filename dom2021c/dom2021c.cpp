// dom2021c.cpp - 木変形パズル
// https://icpc.iisf.or.jp/past-icpc/domestic2021/contest/all_ja.html#section_C
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// ノードを表すクラス
class node {
public:
	// ノードの初期値を設定する。
	node(char c = 0):
		value(c),
		e1(nullptr),
		e2(nullptr),
		e3(nullptr),
		parent(nullptr),
		cached_min(false),
		cache_min(0),
		cached_max(false),
		cache_max(0) {}

	// デストラクタで子ノードを削除する。
	~node() {
		if (e1 != nullptr)
			delete e1;
		if (e2 != nullptr)
			delete e2;
		if (e3 != nullptr)
			delete e3;
	}

	// 文字列sitを根<root>として構文解析し、得られた情報をこのノードに設定する。
	// 
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

	// このノードが根かどうか
	bool is_root_node() const {
		return parent == nullptr;
	}

	// このノードが葉かどうか
	bool is_leaf() const {
		return '0' <= value && value <= '9';
	}

	// 葉の表す値を得る。
	int get_leaf_value() const {
		return value - '0';
	}

	// このノードを含めた木のすべてのノード(葉は除く)のリストを得る。
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

	// このノードが表す式の単純な(変形しない)計算を行う。
	// 計算値を返す。
	int calc() const {
		if (is_leaf())
			return get_leaf_value();

		// ノードの場合

		// 左の子ノードの値を得る。
		int v = e1->calc();

		// 右(または根の中央)の子ノードの値を得て計算する。
		v = (value == '+') ? v + e2->calc() : v - e2->calc();

		if (is_root_node()) {
			// 根の右の子ノードの値を得て計算する。
			v = (value == '+') ? v + e3->calc() : v - e3->calc();
		}
		return v;
	}

	// このノードが根になるよう変形する。
	void raise_to_root() {
		if (is_root_node())
			return;  // もうできている。

		// 一つ上の節(parent)を根にする。
		parent->raise_to_root();

		// キャッシュはクリアする。
		parent->clear_cache();
		clear_cache();

		// このノードをparentの左(e1)によせる。
		if (this == parent->e2) {
			node* tp = parent->e1;
			parent->e1 = this;
			parent->e2 = tp;
		}
		else if (this == parent->e3) {
			node* tp = parent->e1;
			parent->e1 = this;
			parent->e3 = tp;
		}

		// このノードを根にする。

		// parentを下になるよう変形
		parent->e1 = parent->e2;
		parent->e2 = parent->e3;
		parent->e3 = nullptr;
		parent->parent = this;

		// このノードをparentの親の根に
		e3 = parent;
		parent = nullptr;
	}

	// このノードの木で最小・最大(max_resultでどちらか指定)となる計算を行う。
	// 計算値を返す。
	int calc_minmax(bool max_result) {
		if (is_leaf())
			// *** 葉の場合
			return get_leaf_value();

		// キャッシュが利用できる場合
		if (max_result) {
			if (cached_max)
				return cache_max;
		}
		else {
			if (cached_min)
				return cache_min;
		}

		// *** ノードの場合

		// 子ノードごとに最大値をそれぞれ求める。
		const int v1_max = e1->calc_minmax(true);
		const int v1_min = e1->calc_minmax(false);

		const int v2_max = e2->calc_minmax(true);
		const int v2_min = e2->calc_minmax(false);

		const int v3_max = is_root_node() ? e3->calc_minmax(true) : 0;
		const int v3_min = is_root_node() ? e3->calc_minmax(false) : 0;


		if (value == '+') {
			// *** 加算の場合
			return (max_result) 
				? set_cache_max(v1_max + v2_max + v3_max) 
				: set_cache_min(v1_min + v2_min + v3_min);  // 和は左右を入れ替えても変化しない。
		}

		// *** 減算の場合 (value == '-')

		if (!is_root_node()) {
			// *** 途中ノードの場合
			// v1_max >= v1_min, v2_max >= v2_min
			return (max_result)
				? set_cache_max(max(v1_max - v2_min, v2_max - v1_min))   // max(v1 - v2, v2 - v1)
				: set_cache_min(min(v1_min - v2_max, v2_min - v1_max));  // min(v1 - v2, v2 - v1)
		}

		// *** 根ノードの場合

		// v1_max >= v1_min, v2_max >= v2_min, v3_max >= v3_min
		return (max_result)
			?  // max(v1 - v2 - v3, v2 - v3 - v1, v3 - v1 - v2)
			set_cache_max(max(max(v1_max - v2_min - v3_min, v2_max - v3_min - v1_min), v3_max - v1_min - v2_min))
			:  // min(v1 - v2 - v3, v2 - v3 - v1, v3 - v1 - v2) 
			set_cache_min(min(min(v1_min - v2_max - v3_max, v2_min - v3_max - v1_max), v3_min - v1_max - v2_max));
	}

	// このノードを根とした木に対し、パズルを解く。
	// 得られた答を返す。
	int solve_puzzle() {
		// ノードのリストを作る。
		vector<node*> node_list;
		get_node_list(node_list);

		int v = calc();

		// 各ノードを根にした場合を作る。
		int i = 0;
		for (const auto& np : node_list) {
			np->raise_to_root();
			v = max(v, np->calc_minmax(true));
		}
		return v;
	}

private:
	// 今回のノードに関する情報
	char value;  // ノードの種類(対応する文字)
	node* e1;  // 左側の子ノード
	node* e2;  // 右側(または根の中央)の子ノード
	node* e3;  // 根の右側の子ノード

	// 変形時に用いる情報
	node* parent;  // 親ノード

	// キャッシュ用
	bool cached_min;  // 最小値のキャッシュが有効か
	int cache_min;  // 最小値
	bool cached_max;  // 最大値のキャッシュが有効か
	int cache_max;  // 最大値

	// 文字列sitから、演算子<operator>の構文解析を行う。
	// 
	// <operator> ::= '+' | '-'
	// 
	// 得られたオペレータを文字で返す。
	char parse_operator(string::const_iterator& sit) {
		if (*sit != '+' && *sit != '-')
			throw "invalid operator";
		return *(sit++);
	}

	// 文字列sitから、根でないノード<non_root>の構文解析を行う。
	// 
	// <non_root> ::= <node> <operator> <node>
	// 
	// 構文解析により得られた情報で、このノードのe1, value, e2を設定する。
	void parse_non_root(string::const_iterator& sit) {
		// 一つ目のノード
		e1 = parse_node(sit, this);
		// 演算子
		value = parse_operator(sit);
		// 二つ目のノード
		e2 = parse_node(sit, this);
	}

	// 文字列sitから、ノード(葉も含む)<node>の構文解析を行う。
	// 
	// <node> ::= '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9' 
	//			| '(' <non_root> ')'
	//
	// ノードを新たに作成し、そのノードへのポインタを返す。
	// 葉の場合はvalueを設定する。
	// 根でないノードの場合は<non_root>を構文解析し、parentを引数のparentpに設定する。
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
		else {
			throw "invalid charactor";
		}
		return np;
	}

	// 最小値vをキャッシュに残す。
	// 値vをそのまま返す。
	int set_cache_min(int v) {
		cached_min = true;
		cache_min = v;
		return v;
	}

	// 最大値vをキャッシュに残す。
	// 値vをそのまま返す。
	int set_cache_max(int v) {
		cached_max = true;
		cache_max = v;
		return v;
	}

	// このノードのキャッシュを無効にする。
	void clear_cache() {
		cached_min = false;
		cached_max = false;
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
