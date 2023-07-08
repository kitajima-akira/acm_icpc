// dom2023c.cpp - ACM ICPC 2023年国内予選C 席替え
// https://icpc.iisf.or.jp/past-icpc/domestic2023/contest/all_ja.html#section_C
// Copyright 2023 Kitajima Akira
// Author: Kitajima Akira <kitajima@osakac.ac.jp>

#include <iostream>
#include <map>
#include <vector>
using namespace std;

using position = pair<int, int>;

// 2点のマンハッタン距離を求める。
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
		// 効率的な処理のためqueueとempty_seat_listを初期設定する。
		for (int row = 0; row < n; row++) {
			for (int col = 0; col < n; col++) {
				const auto no = seats[row][col];
				queue.insert({ no, vector<int>() });
				get_adjacent_list({ row, col }, queue[no]);
				empty_seat_list.push_back({ row, col });
			}
		}
	}

	// 番号current_noとその番号の位置を決めた上で成否を再帰的に調べる。
	bool explore(int current_no, const position& pos) {
		// 決めた番号に隣接する番号のリストadjacent_listを得る。
		auto adjacent_list = queue[current_no];
		vector<int> rest_adjacent_list;  // 未割当て分の番号リスト
		for (const auto adj_no : adjacent_list) {
			if (fixed_list.find(adj_no) != fixed_list.end()) {
				// すでに割り当てられているので、番号についてはcurrent_noとの距離をチェックする。
				if (manhattan_distance(pos, fixed_list[adj_no]) < min_distance) //   短かければ
					return false;  // 割当て失敗
			} else {
				// 未割当てに追加する。
				rest_adjacent_list.push_back(adj_no);
			}
		}

		// 空いている席の中からcurrent_noのposから決められた距離以上離れた席のリストcandidate_seat_listを得る。
		vector<position> candidate_seat_list;
		for (auto& pos2 : empty_seat_list)
			if (manhattan_distance(pos, pos2) >= min_distance)
				candidate_seat_list.push_back(pos2);
	
		// rest_listの個数よりcandidate_seat_listの個数が少なければ割当て失敗
		if (rest_adjacent_list.size() > candidate_seat_list.size())
			return false;

		// 隣接番号の配置が完了していれば、次は残りの中から決める。
		if (candidate_seat_list.empty())
			candidate_seat_list = empty_seat_list;

		// current_noをposに割り当てる。
		// cout << "> " << current_no << " (" << pos.first << ", " << pos.second << ")" << endl;
		work_map[pos.first][pos.second] = current_no;
		fixed_list[current_no] = pos;
		empty_seat_list.erase(find(empty_seat_list.begin(), empty_seat_list.end(), pos));
		auto saved_list = queue[current_no];
		queue.erase(current_no);

		// 次の番号を決める。
		if (queue.empty())  // 次が無ければ成功
			return true;
		int next_no = rest_adjacent_list.empty() ? queue.begin()->first : rest_adjacent_list.front();

		// 次の位置を決める。
		for (auto& next_pos : candidate_seat_list)
			if (explore(next_no, next_pos))
				return true;

		// すべての位置で失敗したので割り当てを取り消す。
		// cout << "NG> " << current_no << " (" << pos.first << ", " << pos.second << ")" << endl;
		work_map[pos.first][pos.second] = 0;
		fixed_list.erase(current_no);
		empty_seat_list.push_back(pos);
		queue[current_no] = saved_list;

		return false;
	}

	// 席替えする。
	seat_map& rearrange() {
		// 一つ目は次の部分だけでよい。（反転等すると同じになるので）
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
					// 見つかった。
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
	map<int, vector<int>> queue;  // まだ割り当てていない番号とその番号に隣接する番号のリスト
	vector<vector<int>> original_map;  // 変更前の席
	const int n;  // 縦横の大きさ
	vector<vector<int>> work_map;  // 変更中の席
	const int min_distance;  // 最小距離
	vector<position> empty_seat_list;  // 空席の位置リスト
	map<int, position> fixed_list;  // 決まった番号と位置のリスト

	// posに隣接する番号をすべて求める。
	void get_adjacent_list(const position& pos, vector<int>& adjacent_list) {
		const auto row = pos.first;
		const auto col = pos.second;
		// 上
		if (row > 0)
			adjacent_list.push_back(original_map[row - 1][col]);
		// 左
		if (col > 0)
			adjacent_list.push_back(original_map[row][col - 1]);
		// 右
		if (col < n - 1)
			adjacent_list.push_back(original_map[row][col + 1]);
		// 下
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
