/// @file dom2011b.cpp @brief ���E�̓V��
/// @author Kitajima Akira
/// @date 2011/07/05
// 20min
#include <iostream>
#include <string>
#include <stack>
using namespace std;


// sit����n�܂镔�������񂪐������Ή����Ă��邩���肷��B
bool is_balanced_paren_recursive(const string& s, string::const_iterator& s_it, const string& parens) {
	// sit���w���������`�F�b�N
	const size_t open_pos = parens.find(*s_it);
	if (open_pos == string::npos)   // ���ʈȊO�̕���
		return true;
	else if (open_pos % 2 == 1)  // ������
		return false;

	// �J������
	// ���̕����ȍ~�̑Ή���������ʂ�������B
	for (++s_it; s_it != s.end(); ++s_it) {
		const size_t pos = parens.find(*s_it);
		if (pos == string::npos)   // ���ʈȊO�̕���
			continue;

		if (pos % 2 == 1)  // ������
			return (open_pos + 1 == pos);   // �Ή����Ă��邩�ǂ���

		// �J������
		if (!is_balanced_paren_recursive(s, s_it, parens))
			return false;
		// sit�͑Ή���������ʂ܂ňړ����Ă���B
		// �c��̕����͎��̃��[�v�Œ��ׂ�B
	}
	// �Ή��������炸�ɏI��
	return false;
}


bool match_parens2(const string& s, const string& parens) {
	for (auto s_it = s.begin(); s_it != s.end(); ++s_it)
		if (!is_balanced_paren_recursive(s, s_it, parens))
			return false;
	return true;
}

/// �����񒆂̊��ʂ��Ή����Ă��邩�`�F�b�N����B
/// @param s �Ώە�����
/// @param parens �`�F�b�N�Ώۂ̊���
///               (�Ή�����J�����ʂƕ����ʂ����ɕ��ԁB)
/// @returns ���܂��Ή����Ă�����^�ƂȂ�B

bool match_parens(const string& s, const string& parens) {
	stack<char> st;
	for (const auto& c : s) {
		size_t pos = parens.find(c);
		if (pos != string::npos) {
			if (pos % 2 == 0) {
				// �J������
				st.push(c);
			}
			else {
				// ������
				if (st.empty() || st.top() != parens[pos - 1])
					return false;
				st.pop();
			}
		}
	}
	return st.empty();
}


int main() {
	for (string s; getline(cin, s), s != ".";) {
		cout << (match_parens(s, "()[]") ? "yes" : "no") << endl;
		cout << (match_parens2(s, "()[]") ? "yes" : "no") << endl;
	}
return 0;
}
