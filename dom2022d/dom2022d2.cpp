// dom2022d2.cpp - [ACM ICPC 2022 D] 入場待ちの行列
// https://icpc.iisf.or.jp/past-icpc/domestic2022/contest/all_ja.html#section_D

// 多小の枝苅りはしているものの、基本的に総当たりのため、大きい数値では制限時間内に終わらない。

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


	// 分割数がkまでで指定された結果と一致する分け方の総数を求める。
	int num_combination(int k) const {
		// 分割不可能な場合は先に除く。
		if (!is_valid_goal())
			return 0;

		int p = 0;
		for (int i = 0; i < before.size() - 1; i++) {
			if (!is_same_order(before[i], before[i + 1]))
				p++;
		}
		if (p >= k)
			return 0;

		return explore(0, 1, k);
	}

private:
	vector<int> before;  // ゲート前の順 (チケット番号の列)
	vector<int> after;  // ゲート後の順 (チケット番号の列)
	int base;  // 計数のときの基数 (問題で指定)

	// そもそも実現可能か(0以上か)をチェックする。
	bool is_valid_goal() const {
		// afterの最後の要素から見ていく。
		for (int i = after.size() - 1; i > 0; i--) {
			const auto last = after[i];
			// 要素lastより前にある、lastに一番近い、largeより大きい値を見つける。
			auto large = 0;  // lastより大きい値 (初期値はすべての値より小さい0)
			auto large_pos = 0;
			for (int j = i - 1; j >= 0; j--) {
				if (after[j] > last) {
					large = after[j];
					large_pos = j;
					break;
				}
			}
			if (large > 0) {  // 見つかった場合
				// beforeの中からlargeを探す。
				int j;
				for (j = 0; j < before.size(); j++)
					if (before[j] == large)
						break;

				// largeを含めたlastまでの個数(last含まず)
				const auto prefix_size = i - large_pos;

				// largeより後の要素数が足りない場合
				if (j + prefix_size >= before.size())
					return false;

				// beforeのlargeからの並びとafterのlargeからの並びを比較する。
				for (int d = 1; large_pos + d <= i; d++)
					if (before[j + d] != after[large_pos + d])
						return false;

				// 一致していたらlargeからlastまではOK
				// largeから続けるようiを設定
				i = large_pos + 1;
			}
			// 見つからなかった場合はそのlastはOK

		}
		return true;
	}


	// a, bがafter中でもその順番かどうか
	bool is_same_order(int a, int b) const {
		for (int t : after) {
			if (t == a)
				return true;  // 先にaが見つかった。
			if (t == b)
				return false;  // 先にbが見つかった。
		}
		return false;
	}

	// 全探索で枝刈りしながら数える。
	// beforeをi番目から順に分割する。num_partitionが現在の分割数で、k分割まで考える。
	// 
	// 問題解説より
	// https://icpc.iisf.or.jp/past-icpc/domestic2022/commentaries.html#D
	// 
	// 元 4 2!3?5!1       
	// 後 1 3 4 2 5
	// (ケース1) 任意の元の2連続要素について、後の2連続要素の順が異なるとき、元の2連続要素間は必ず区切らないといけない。 (!)
	// (ケース2) ある番号が前にいるすべての人の番号よりも大きいとき、その番号の前で区切ってもよい。 (?)
	// * 「!の合計 n1 + ?の選択(区切った)数 n2 < k」を満たす組合せ(各?に対する0 or 1)の数を計算する。
	// * 0 ～ k - n1の各rに対する n2 C r (Combination(n2,r) = n2 * (n2 - 1) * ... * r / r * ...* 1)の和
	int explore(int i, int num_partition, int k) const {
		// 分割数がkを超えたらそれ以上探索する必要はない。
		if (num_partition > k)
			return 0;

		// 分割できるところを探す。
		int local_max = 0;
		for (size_t j = i; j < before.size() - 1; j++) {
			// j番目とj + 1番目との間が区切れるかどうかを調べる。
			if (!is_same_order(before[j], before[j + 1]))  // ケース1
				// j と j + 1の間で区切る。
				return explore(j + 1, num_partition + 1, k);

			local_max = max(local_max, before[j]);
			if (local_max < before[j + 1]) {  // ケース2
				int r = explore(j + 1, num_partition + 1, k)  // 区切った場合の数
					+ explore(j + 1, num_partition, k);
				if (r > base)
					r -= base;
				return r;
			}
		}

		// 最後まで行った場合
		return 1;
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
