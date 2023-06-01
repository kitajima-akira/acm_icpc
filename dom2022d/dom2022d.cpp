// dom2022d.cpp - [ACM ICPC 2022 D] 入場待ちの行列
// https://icpc.iisf.or.jp/past-icpc/domestic2022/contest/all_ja.html#section_D

// Sample Inputが138分程度で終了

#include <iostream>
#include <vector>
using namespace std;

class queue {
public:
	// 最初の並び順s、最終的な入場順t、計数のときの基数を用いて初期設定する。
	queue(const vector<int>& s, const vector<int>& t, int b) :
		before(), after(), base(b) {
		before = s;
		after = t;
	}

	// 与えられた分割partition (大きさsize)に対し、
	// 行列beforeを通したときにafter順になるかどうかを判定する。
	bool check_condition(const vector<int>& partition, size_t size) const {
		// シミュレーションする。
		auto current_partition = partition;  // 各列の先頭位置

		// 正解afterの先頭から順にシミュレーションした順と一致するか調べる。
		for (const auto& t : after) {
			// beforeを区切った各列の先頭の中での最小値(とそれはどの列か)を得る。
			int next_ticket_no = before.size() + 1;  // 最小のチケット番号
			int next_queue;  // 最小のチケット番号所有者がいる列
			for (size_t i = 0; i < size; i++) {
				const int current_position = current_partition[i];
				if (current_position < partition[i + 1]  // その列が空でない
					&& before[current_position] < next_ticket_no) {  // チケット番号が最小
					// 最小情報を更新する。
					next_ticket_no = before[current_position];
					next_queue = i;
				}
			}

			// afterの順と一致していなければ不一致と判定
			if (next_ticket_no != t)
				return false;

			// キューを更新する。(一人入場させる。)
			current_partition[next_queue]++;
		}
		return true;
	}

	// 分割数がkまでで指定された結果と一致する分け方の総数を求める。
	int num_combination(size_t k) const {
		//  分割は次のデータ形式とする。
		// [要素番号]			0 1 2 3 4 5
		// [並びと区切り]		4 2|3 5|1
		// [データ形式]		{ 0, 2, 4, 5 }
		// 「分割の最初の要素の要素番号」の並び +「最後の要素の次の要素番号」
		vector<int> current_partition(k + 1);  // 速度重視のため、最大数用意し、途中で大きさは変えない。
		current_partition[0] = 0;
		int r = 0;
		for (int i = 1; i <= k; i++) {
			r += explore_all(i, 0, current_partition, 1);
			if (r >= base)
				r -= base;
		}
		return r;
	}

private:
	vector<int> before;  // ゲート前の順 (チケット番号の列)
	vector<int> after;  // ゲード後の順 (チケット番号の列)
	int base;  // 計数のときの基数 (問題で指定)

	// beforeをcurrent_positionからちょうどk分割する全パターンを列挙し、条件を満たす数を数える。
	int explore_all(int k, int current_position, vector<int>& current_partition, size_t current_partition_index) const {
		if (k == 1) {
			// 分割完了
			// 最後の要素は全体の長さにしておく。
			current_partition[current_partition_index] = before.size();

			// 分割が一つ決まったので、それに従い条件を判定する。
			const auto r = check_condition(current_partition, current_partition_index) ? 1 : 0;
			return r;
		}

		// 分割途中の先頭の大きさを変えて深さ優先探索
		int r = 0;
		const int rest_size = before.size() - current_position;  // 残り人数
		for (int current_size = 1; current_size <= rest_size - (k - 1); current_size++) {
			const int next_partition = current_position + current_size;
			current_partition[current_partition_index] = next_partition;
			r += explore_all(k - 1, next_partition, current_partition, current_partition_index + 1);
			if (r >= base)
				r -= base;
		}
		return r;
	}
};

int main() {
	for (int n, k; (cin >> n >> k) && n > 0 && k > 0; ) {
		vector<int> s(n);
		for (auto& e : s)
			cin >> e;
		vector<int> t(n);
		for (auto& e : t)
			cin >> e;
		queue q(s, t, 998244353);
		cout << q.num_combination(k) << endl;
	}
	return 0;
}
