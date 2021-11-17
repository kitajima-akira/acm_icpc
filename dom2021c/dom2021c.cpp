// dom2021c.cpp - �ؕό`�p�Y��
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
			// ��ڂ̃m�[�h
			e1 = parse_node(sit, this);

			// ���Z�q
			const char first_op = parse_operator(sit);

			// ��ڂ̃m�[�h
			e2 = parse_node(sit, this);

			// ���Z�q
			value = parse_operator(sit);
			if (value != first_op)
				throw "mismatch operators in root";

			// �O�ڂ̃m�[�h
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
			return;  // �t�͓o�^���Ȃ��B

		// �o�^
		node_list.push_back(this);

		// �q�m�[�h���ċA�I�ɓo�^
		e1->get_node_list(node_list);
		e2->get_node_list(node_list);

		if (is_root_node())
			e3->get_node_list(node_list);
	}

	// �P���Ȍv�Z (���𓾂�܂ł̏���)
	int calc() {
		if (is_leaf()) {
			// �t�̏ꍇ
			return value - '0';
		}
		else {
			// �m�[�h�̏ꍇ
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
			return;  // �����ł��Ă���B

		//   ���̐߂����ɂ���B
		parent->raise_to_root();

		//   ���ɂ悹��B
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

		//   ���ɂ���B

		// ���Ƃ��ƍ���������������
		parent->e1 = parent->e2;
		parent->e2 = parent->e3;
		parent->e3 = nullptr;
		parent->parent = this;

		// ���̉��������������
		e3 = parent;
		parent = nullptr;
	}

	// np����̖؂ōŏ��E�ő�(max_result�łǂ��炩�w��)�ƂȂ�X�R�A(�v�Z�l)�����߂�B
	int calc_minmax(bool max_result) {
		if (is_leaf()) {
			// �t�̏ꍇ
			return value - '0';
		}
		else {
			// �m�[�h�̏ꍇ
			int v;

			const int v1_max = e1->calc_minmax(true);
			const int v1_min = e1->calc_minmax(false);
			const int v2_max = e2->calc_minmax(true);
			const int v2_min = e2->calc_minmax(false);

			const int v3_max = is_root_node() ? e3->calc_minmax(true) : 0;
			const int v3_min = is_root_node() ? e3->calc_minmax(false) : 0;

			if (value == '+') {
				// �a�͍��E�����ւ��Ă��ω����Ȃ��B
				v = v1_max + v2_max + v3_max;
			}
			else {  // value == '-'
				if (!is_root_node()) {
					// �r���m�[�h
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
				else {  // ���m�[�h
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
		// �߂̃��X�g�����B
		vector<node*> node_list;
		get_node_list(node_list);

		int v = calc();

		// �e�߂����ɂ����ꍇ�����B
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
		// ��ڂ̃m�[�h
		e1 = parse_node(sit, this);
		// ���Z�q
		value = parse_operator(sit);
		// ��ڂ̃m�[�h
		e2 = parse_node(sit, this);
	}

	// <node> ::= '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9' | '(' <non_root> ')' 
	node* parse_node(string::const_iterator& sit, node* parentp) {
		node* np = new node();
		if ('0' <= *sit && *sit <= '9') {
			// �t�̏ꍇ
			np->value = *sit;
			++sit;  // �����̎��֐i�߂�B
		}
		else if (*sit == '(') {
			// �r���m�[�h�̏ꍇ
			++sit;  // �u(�v�̎��֐i�߂�B
			np->parse_non_root(sit);
			np->parent = parentp;
			if (*sit != ')')
				throw "missing )";
			++sit;  // �u)�v�̎��֐i�߂�B
		}
		else
			throw "invalid charactor";
		return np;
	}
};

int main() {
	for (string s; (cin >> s) && s != "-1";) {
		// 1�s�ǂݎ��؍\�������B
		node root;
		root.parse_root(s.begin());
		// �؍\�������ƂɃp�Y���������B
		cout << root.solve_puzzle() << endl;
	}
	return 0;
}
