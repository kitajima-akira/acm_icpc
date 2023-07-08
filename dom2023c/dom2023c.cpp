// dom2023c.cpp - ACM ICPC 2023�N�����\�IC �ȑւ�
// https://icpc.iisf.or.jp/past-icpc/domestic2023/contest/all_ja.html#section_C
// Copyright 2023 Kitajima Akira
// Author: Kitajima Akira <kitajima@osakac.ac.jp>

#include <iostream>
#include <map>
#include <vector>
using namespace std;

using position = pair<int, int>;

// 2�_�̃}���n�b�^�����������߂�B
int manhattan_distance(const position& p1, const position& p2) {
	const auto row1 = p1.first;
	const auto col1 = p1.second;
	const auto row2 = p2.first;
	const auto col2 = p2.second;
	return abs(row2 - row1) + abs(col2 - col1);
}

class seat_map {
public:
	seat_map(vector<vector<int>> seats) :
		queue(),
		original_map(seats),
		n(seats.size()),
		work_map(seats.size(), vector<int>(seats.size(), 0)),
		min_distance(seats.size() / 2),
		empty_seat_list() {
		// �����I�ȏ����̂���queue��empty_seat_list�������ݒ肷��B
		for (int row = 0; row < n; row++) {
			for (int col = 0; col < n; col++) {
				const auto no = seats[row][col];
				queue.insert({ no, vector<int>() });
				get_adjacent_list({ row, col }, queue[no]);
				empty_seat_list.push_back({ row, col });
			}
		}
	}

	// �ԍ�current_no�Ƃ��̔ԍ��̈ʒu�����߂���Ő��ۂ��ċA�I�ɒ��ׂ�B
	bool explore(int current_no, const position& pos) {
		// ���߂��ԍ��ɗאڂ���ԍ��̃��X�gadjacent_list�𓾂�B
		auto adjacent_list = queue[current_no];
		vector<int> rest_adjacent_list;  // �������ĕ��̔ԍ����X�g
		for (const auto adj_no : adjacent_list) {
			if (fixed_list.find(adj_no) != fixed_list.end()) {
				// ���łɊ��蓖�Ă��Ă���̂ŁA�ԍ��ɂ��Ă�current_no�Ƃ̋������`�F�b�N����B
				if (manhattan_distance(pos, fixed_list[adj_no]) < min_distance) //   �Z�������
					return false;  // �����Ď��s
			} else {
				// �������Ăɒǉ�����B
				rest_adjacent_list.push_back(adj_no);
			}
		}

		// �󂢂Ă���Ȃ̒�����current_no��pos���猈�߂�ꂽ�����ȏ㗣�ꂽ�Ȃ̃��X�gcandidate_seat_list�𓾂�B
		vector<position> candidate_seat_list;
		for (auto& pos2 : empty_seat_list)
			if (manhattan_distance(pos, pos2) >= min_distance)
				candidate_seat_list.push_back(pos2);
	
		// rest_list�̌����candidate_seat_list�̌������Ȃ���Ί����Ď��s
		if (rest_adjacent_list.size() > candidate_seat_list.size())
			return false;

		// �אڔԍ��̔z�u���������Ă���΁A���͎c��̒����猈�߂�B
		if (candidate_seat_list.empty())
			candidate_seat_list = empty_seat_list;

		// current_no��pos�Ɋ��蓖�Ă�B
		// cout << "> " << current_no << " (" << pos.first << ", " << pos.second << ")" << endl;
		work_map[pos.first][pos.second] = current_no;
		fixed_list[current_no] = pos;
		empty_seat_list.erase(find(empty_seat_list.begin(), empty_seat_list.end(), pos));
		auto saved_list = queue[current_no];
		queue.erase(current_no);

		// ���̔ԍ������߂�B
		if (queue.empty())  // ����������ΐ���
			return true;
		int next_no = rest_adjacent_list.empty() ? queue.begin()->first : rest_adjacent_list.front();

		// ���̈ʒu�����߂�B
		for (auto& next_pos : candidate_seat_list)
			if (explore(next_no, next_pos))
				return true;

		// ���ׂĂ̈ʒu�Ŏ��s�����̂Ŋ��蓖�Ă��������B
		// cout << "NG> " << current_no << " (" << pos.first << ", " << pos.second << ")" << endl;
		work_map[pos.first][pos.second] = 0;
		fixed_list.erase(current_no);
		empty_seat_list.push_back(pos);
		queue[current_no] = saved_list;

		return false;
	}

	// �ȑւ�����B
	seat_map& rearrange() {
		// ��ڂ͎��̕��������ł悢�B�i���]������Ɠ����ɂȂ�̂Łj
		//   * * * . .
		//   . * * . .
		//   . . * . .
		//   . . . . .
		//   . . . . . 
		const auto current_no = queue.begin()->first;
		const auto limit = n / 2 + n % 2;
		for (int row = 0; row < limit; row++)
			for (int col = row; col < limit; col++)
				if (explore(current_no, { row, col })) {
					// ���������B
					original_map = work_map;
					return *this;
				}
		return *this;
	}

	friend ostream& operator<<(ostream& os, const seat_map& s) {
		for (int row = 0; row < s.n; row++) {
			os << s.original_map[row][0];
			for (int col = 1; col < s.n; col++)
				os << " " << s.original_map[row][col];
			os << endl;
		}
		return os;
	}

private:
	map<int, vector<int>> queue;  // �܂����蓖�ĂĂ��Ȃ��ԍ��Ƃ��̔ԍ��ɗאڂ���ԍ��̃��X�g
	vector<vector<int>> original_map;  // �ύX�O�̐�
	const int n;  // �c���̑傫��
	vector<vector<int>> work_map;  // �ύX���̐�
	const int min_distance;  // �ŏ�����
	vector<position> empty_seat_list;  // ��Ȃ̈ʒu���X�g
	map<int, position> fixed_list;  // ���܂����ԍ��ƈʒu�̃��X�g

	// pos�ɗאڂ���ԍ������ׂċ��߂�B
	void get_adjacent_list(const position& pos, vector<int>& adjacent_list) {
		const auto row = pos.first;
		const auto col = pos.second;
		// ��
		if (row > 0)
			adjacent_list.push_back(original_map[row - 1][col]);
		// ��
		if (col > 0)
			adjacent_list.push_back(original_map[row][col - 1]);
		// �E
		if (col < n - 1)
			adjacent_list.push_back(original_map[row][col + 1]);
		// ��
		if (row < n - 1)
			adjacent_list.push_back(original_map[row + 1][col]);
	}
};

int main() {
	for (int n; cin >> n && n > 0;) {
		vector<vector<int>> seats(n, vector<int>(n));
		for (auto& line : seats)
			for (auto& a : line)
				cin >> a;
		seat_map s(seats);
		cout << s.rearrange();
	}
	return 0;
}
