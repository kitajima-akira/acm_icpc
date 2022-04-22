// dom2021c.cpp - �ؕό`�p�Y��
// https://icpc.iisf.or.jp/past-icpc/domestic2021/contest/all_ja.html#section_C
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// �m�[�h��\���N���X
class node {
public:
	// �m�[�h�̏����l��ݒ肷��B
	node(char c = 0):
		left_child(nullptr),
		middle_child(nullptr),
		right_child(nullptr),
		parent(nullptr),
		value(c),
		cached_min(false),
		cache_min(0),
		cached_max(false),
		cache_max(0) {}

	// �ċA�I�ɖ؂̃m�[�h�����ׂč폜����B
	~node() {
		if (left_child != nullptr)
			delete left_child;
		if (middle_child != nullptr)
			delete middle_child;
		if (right_child != nullptr)
			delete right_child;
	}

	// ���̃m�[�h�������ǂ���
	bool is_root() const {
		return parent == nullptr;
	}

	// ���̃m�[�h���t���ǂ���
	bool is_leaf() const {
		return !left_child && !middle_child && !right_child;
	}

	// ������sit����<root>�Ƃ��č\����͂��A����ꂽ�������̃m�[�h�ɐݒ肷��B
	// 
	// <root> ::= <node> '+' <node> '+' <node>
	//			| <node> '-' <node> '-' <node>
	void parse_root(string::const_iterator sit) {
		try {
			// ��ڂ̃m�[�h
			left_child = parse_node(sit, this);

			// ���Z�q
			const char first_op = parse_operator(sit);

			// ��ڂ̃m�[�h
			middle_child = parse_node(sit, this);

			// ���Z�q
			value = parse_operator(sit);
			if (value != first_op)
				throw "mismatch operators in root";

			// �O�ڂ̃m�[�h
			right_child = parse_node(sit, this);
		}
		catch (const string& s) {
			cout << "wrong format: " << s << endl;
			exit(1);
		}
	}

	// �t�̕\���l�𓾂�B
	int get_leaf_value() const {
		return value - '0';
	}

	// ���̃m�[�h���܂߂��؂̂��ׂẴm�[�h(�t�͏���)�̃��X�g�𓾂�B
	void get_node_list(vector<node*>& node_list) {
		if (is_leaf())
			return;  // �t�͓o�^���Ȃ��B

		// �o�^
		node_list.push_back(this);

		// �q�m�[�h���ċA�I�ɓo�^
		left_child->get_node_list(node_list);
		middle_child->get_node_list(node_list);

		if (is_root())
			right_child->get_node_list(node_list);
	}

	// ���̃m�[�h���\�����̒P����(�ό`���Ȃ�)�v�Z���s���B
	// �v�Z�l��Ԃ��B
	int calc() const {
		if (is_leaf())
			return get_leaf_value();

		// �m�[�h�̏ꍇ

		// ���̎q�m�[�h�̒l�𓾂�B
		int v = left_child->calc();

		// �E(�܂��͍��̒���)�̎q�m�[�h�̒l�𓾂Čv�Z����B
		v = (value == '+') ? v + middle_child->calc() : v - middle_child->calc();

		if (is_root()) {
			// ���̉E�̎q�m�[�h�̒l�𓾂Čv�Z����B
			v = (value == '+') ? v + right_child->calc() : v - right_child->calc();
		}
		return v;
	}

	// ���̃m�[�h�����ɂȂ�悤�ό`����B
	void raise_to_root() {
		if (is_root())
			return;  // �����ł��Ă���B

		// ���̐�(parent)�����ɂ���B
		parent->raise_to_root();

		// �L���b�V���̓N���A����B
		parent->clear_cache();
		clear_cache();

		// ���̃m�[�h��parent�̍�(left_child)�ɂ悹��B
		if (this == parent->middle_child) {
			auto tp = parent->left_child;
			parent->left_child = this;
			parent->middle_child = tp;
		}
		else if (this == parent->right_child) {
			auto tp = parent->left_child;
			parent->left_child = this;
			parent->right_child = tp;
		}

		// ���̃m�[�h�����ɂ���B

		// parent�����ɂȂ�悤�ό`
		parent->left_child = parent->middle_child;
		parent->middle_child = parent->right_child;
		parent->right_child = nullptr;
		parent->parent = this;

		// ���̃m�[�h��parent�̐e�̍���
		right_child = parent;
		parent = nullptr;
	}

	// ���̃m�[�h�̖؂ōŏ��E�ő�(max_result�łǂ��炩�w��)�ƂȂ�v�Z���s���B
	// �v�Z�l��Ԃ��B
	int calc_minmax(bool max_result) {
		if (is_leaf())
			// *** �t�̏ꍇ
			return get_leaf_value();

		// �L���b�V�������p�ł���ꍇ
		if (max_result) {
			if (cached_max)
				return cache_max;
		}
		else {
			if (cached_min)
				return cache_min;
		}

		// *** �m�[�h�̏ꍇ

		// �q�m�[�h���Ƃɍő�l�����ꂼ�ꋁ�߂�B
		const int v1_max = left_child->calc_minmax(true);
		const int v1_min = left_child->calc_minmax(false);

		const int v2_max = middle_child->calc_minmax(true);
		const int v2_min = middle_child->calc_minmax(false);

		const int v3_max = is_root() ? right_child->calc_minmax(true) : 0;
		const int v3_min = is_root() ? right_child->calc_minmax(false) : 0;


		if (value == '+') {
			// *** ���Z�̏ꍇ
			return (max_result) 
				? set_cache_max(v1_max + v2_max + v3_max) 
				: set_cache_min(v1_min + v2_min + v3_min);  // �a�͍��E�����ւ��Ă��ω����Ȃ��B
		}

		// *** ���Z�̏ꍇ (value == '-')

		if (!is_root()) {
			// *** �r���m�[�h�̏ꍇ
			// v1_max >= v1_min, v2_max >= v2_min
			return (max_result)
				? set_cache_max(max(v1_max - v2_min, v2_max - v1_min))   // max(v1 - v2, v2 - v1)
				: set_cache_min(min(v1_min - v2_max, v2_min - v1_max));  // min(v1 - v2, v2 - v1)
		}

		// *** ���m�[�h�̏ꍇ

		// v1_max >= v1_min, v2_max >= v2_min, v3_max >= v3_min
		return (max_result)
			?  // max(v1 - v2 - v3, v2 - v3 - v1, v3 - v1 - v2)
			set_cache_max(max(max(v1_max - v2_min - v3_min, v2_max - v3_min - v1_min), v3_max - v1_min - v2_min))
			:  // min(v1 - v2 - v3, v2 - v3 - v1, v3 - v1 - v2) 
			set_cache_min(min(min(v1_min - v2_max - v3_max, v2_min - v3_max - v1_max), v3_min - v1_max - v2_max));
	}

	// ���̃m�[�h�����Ƃ����؂ɑ΂��A�p�Y���������B
	// ����ꂽ����Ԃ��B
	int solve_puzzle() {
		// �m�[�h�̃��X�g�����B
		vector<node*> node_list;
		get_node_list(node_list);

		int v = calc();

		// �e�m�[�h�����ɂ����ꍇ�����B
		int i = 0;
		for (const auto& np : node_list) {
			np->raise_to_root();
			v = max(v, np->calc_minmax(true));
		}
		return v;
	}

private:
	node* left_child;  // �q�m�[�h1
	node* middle_child;  // �q�m�[�h2
	node* right_child;  // �q�m�[�h3
	node* parent;  // �e�m�[�h

	// ����̃m�[�h�Ɋւ�����
	char value;  // �m�[�h�̎��(�Ή����镶��)

	// �L���b�V���p
	bool cached_min;  // �ŏ��l�̃L���b�V�����L����
	int cache_min;  // �ŏ��l
	bool cached_max;  // �ő�l�̃L���b�V�����L����
	int cache_max;  // �ő�l

	// ������sit����A���Z�q<operator>�̍\����͂��s���B
	// 
	// <operator> ::= '+' | '-'
	// 
	// ����ꂽ�I�y���[�^�𕶎��ŕԂ��B
	char parse_operator(string::const_iterator& sit) {
		if (*sit != '+' && *sit != '-')
			throw "invalid operator";
		return *(sit++);
	}

	// ������sit����A���łȂ��m�[�h<non_root>�̍\����͂��s���B
	// 
	// <non_root> ::= <node> <operator> <node>
	// 
	// �\����͂ɂ�蓾��ꂽ���ŁA���̃m�[�h��e1, value, middle_child��ݒ肷��B
	void parse_non_root(string::const_iterator& sit) {
		// ��ڂ̃m�[�h
		left_child = parse_node(sit, this);
		// ���Z�q
		value = parse_operator(sit);
		// ��ڂ̃m�[�h
		middle_child = parse_node(sit, this);
	}

	// ������sit����A�m�[�h(�t���܂�)<node>�̍\����͂��s���B
	// 
	// <node> ::= '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9' 
	//			| '(' <non_root> ')'
	//
	// �m�[�h��V���ɍ쐬���A���̃m�[�h�ւ̃|�C���^��Ԃ��B
	// �t�̏ꍇ��value��ݒ肷��B
	// ���łȂ��m�[�h�̏ꍇ��<non_root>���\����͂��Aparent��������parentp�ɐݒ肷��B
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
		else {
			throw "invalid charactor";
		}
		return np;
	}

	// �ŏ��lv���L���b�V���Ɏc���B
	// �lv�����̂܂ܕԂ��B
	int set_cache_min(int v) {
		cached_min = true;
		cache_min = v;
		return v;
	}

	// �ő�lv���L���b�V���Ɏc���B
	// �lv�����̂܂ܕԂ��B
	int set_cache_max(int v) {
		cached_max = true;
		cache_max = v;
		return v;
	}

	// ���̃m�[�h�̃L���b�V���𖳌��ɂ���B
	void clear_cache() {
		cached_min = false;
		cached_max = false;
	}
};

int main() {
	for (string s; (cin >> s) && (s != "-1");) {
		// 1�s�ǂݎ��؍\�������B
		node root;
		root.parse_root(s.begin());

		// �ό`���Ȃ��ꍇ�̌v�Z���ʂ��o�͂���B(�r���̊m�F�p)
		// cout << root.calc() << endl;

		// �؍\�������ƂɃp�Y���������B
		cout << root.solve_puzzle() << endl;
	}
	return 0;
}
