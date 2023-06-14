// dom2017b.cpp - ACM ICPC 2017�N�����\�IB - �قƂ�Ǔ����v���O����
// https://icpc.iisf.or.jp/past-icpc/domestic2017/contest/all_ja.html#section_B
// Copyright 2023 Kitajima Akira
// Author: Kitajima Akira <kitajima@osakac.ac.jp>

#include <iostream>
#include <string>
using namespace std;

// ���茋�ʂ�\���^
enum class judge_result {IDENTICAL, CLOSE, DIFFERENT};

// ��̕�����̈Ⴂ�𔻒肷��B
judge_result judge(const string& s1, const string& s2) {
	// �e������̐擪���珇�Ɋm�F����B
	bool one_mistake = false;  // ��ڂ̈Ⴂ�����������ǂ����B
	auto p1 = s1.begin(), p2 = s2.begin();  // ���݊m�F���̕����̈ʒu�B
	for (bool in_string = false; p1 != s1.end() && p2 != s2.end(); ++p1, ++p2) {
		if (*p1 == '"' && *p2 == '"') {  // ������̋�؂肩?
			in_string = !in_string;
			continue;
		}
		if (*p1 != *p2) {  // �Ⴄ������?
			// ����Ă���Ɣ���ł��邩�m�F����B
			if (!in_string || one_mistake)
				return judge_result::DIFFERENT;

			// �����񒆂Ȃ̂ŁA������̏I���܂œǂݔ�΂��B
			one_mistake = true;
			while (*p1 != '"')
				++p1;
			while (*p2 != '"')
				++p2;
			in_string = false;
		}
	}

	// �ǂ��炩�̕����񂪏I����Ă���̔��菈�����s���B
	if (p1 != s1.end() || p2 != s2.end())  // ����������܂��c���Ă���?
		return judge_result::DIFFERENT;
	return one_mistake ? judge_result::CLOSE : judge_result::IDENTICAL;
}

int main() {
	for (string s1; cin >> s1 && s1 != "."; ) {
		string s2;
		cin >> s2;
		switch (judge(s1, s2)) {
		case judge_result::IDENTICAL:
			cout << "IDENTICAL\n";
			break;
		case judge_result::CLOSE:
			cout << "CLOSE\n";
			break;
		case judge_result::DIFFERENT:
			cout << "DIFFERENT\n";
			break;
		}
	}
	return 0;
}
