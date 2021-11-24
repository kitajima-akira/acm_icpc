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
		value(c),
		e1(nullptr),
		e2(nullptr),
		e3(nullptr),
		parent(nullptr),
		cached_min(false),
		cache_min(0),
		cached_max(false),
		cache_max(0) {}

	// �f�X�g���N�^�Ŏq�m�[�h���폜����B
	~node() {
		if (e1 != nullptr)
			delete e1;
		if (e2 != nullptr)
			delete e2;
		if (e3 != nullptr)
			delete e3;
	}

	// ������sit����<root>�Ƃ��č\����͂��A����ꂽ�������̃m�[�h�ɐݒ肷��B
	// 
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

	// ���̃m�[�h�������ǂ���
	bool is_root_node() const {
		return parent == nullptr;
	}

	// ���̃m�[�h���t���ǂ���
	bool is_leaf() const {
		return '0' <= value && value <= '9';
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
		e1->get_node_list(node_list);
		e2->get_node_list(node_list);

		if (is_root_node())
			e3->get_node_list(node_list);
	}

	// ���̃m�[�h���\�����̒P����(�ό`���Ȃ�)�v�Z���s���B
	// �v�Z�l��Ԃ��B
	int calc() const {
		if (is_leaf())
			return get_leaf_value();

		// �m�[�h�̏ꍇ

		// ���̎q�m�[�h�̒l�𓾂�B
		int v = e1->calc();

		// �E(�܂��͍��̒���)�̎q�m�[�h�̒l�𓾂Čv�Z����B
		v = (value == '+') ? v + e2->calc() : v - e2->calc();

		if (is_root_node()) {
			// ���̉E�̎q�m�[�h�̒l�𓾂Čv�Z����B
			v = (value == '+') ? v + e3->calc() : v - e3->calc();
		}
		return v;
	}

	// ���̃m�[�h�����ɂȂ�悤�ό`����B
	void raise_to_root() {
		if (is_root_node())
			return;  // �����ł��Ă���B

		// ���̐�(parent)�����ɂ���B
		parent->raise_to_root();

		// �L���b�V���̓N���A����B
		parent->clear_cache();
		clear_cache();

		// ���̃m�[�h��parent�̍�(e1)�ɂ悹��B
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

		// ���̃m�[�h�����ɂ���B

		// parent�����ɂȂ�悤�ό`
		parent->e1 = parent->e2;
		parent->e2 = parent->e3;
		parent->e3 = nullptr;
		parent->parent = this;

		// ���̃m�[�h��parent�̐e�̍���
		e3 = parent;
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
		const int v1_max = e1->calc_minmax(true);
		const int v1_min = e1->calc_minmax(false);

		const int v2_max = e2->calc_minmax(true);
		const int v2_min = e2->calc_minmax(false);

		const int v3_max = is_root_node() ? e3->calc_minmax(true) : 0;
		const int v3_min = is_root_node() ? e3->calc_minmax(false) : 0;


		if (value == '+') {
			// *** ���Z�̏ꍇ
			return (max_result) 
				? set_cache_max(v1_max + v2_max + v3_max) 
				: set_cache_min(v1_min + v2_min + v3_min);  // �a�͍��E�����ւ��Ă��ω����Ȃ��B
		}

		// *** ���Z�̏ꍇ (value == '-')

		if (!is_root_node()) {
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
	// ����̃m�[�h�Ɋւ�����
	char value;  // �m�[�h�̎��(�Ή����镶��)
	node* e1;  // �����̎q�m�[�h
	node* e2;  // �E��(�܂��͍��̒���)�̎q�m�[�h
	node* e3;  // ���̉E���̎q�m�[�h

	// �ό`���ɗp������
	node* parent;  // �e�m�[�h

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
	// �\����͂ɂ�蓾��ꂽ���ŁA���̃m�[�h��e1, value, e2��ݒ肷��B
	void parse_non_root(string::const_iterator& sit) {
		// ��ڂ̃m�[�h
		e1 = parse_node(sit, this);
		// ���Z�q
		value = parse_operator(sit);
		// ��ڂ̃m�[�h
		e2 = parse_node(sit, this);
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
	for (string s; (cin >> s) && s != "-1";) {
		// 1�s�ǂݎ��؍\�������B
		node root;
		root.parse_root(s.begin());
		// �؍\�������ƂɃp�Y���������B
		cout << root.solve_puzzle() << endl;
	}
	return 0;
}
