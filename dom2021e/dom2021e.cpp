// dom2021e.cpp - 時は金なり
// https://icpc.iisf.or.jp/past-icpc/domestic2021/contest/all_ja.html#section_E
// -----------------------------------------------------------------------------
#include <algorithm>
#include <iostream>
#include <set>
#include <tuple>
#include <vector>
using namespace std;

// 長桁計算用定義

// 問題の設定より
// n <= 20000, p <= 20000, q <= 20000, c <= 1000000
// タクシーの待ち時間はk回乗ると 2^0 + 2^1 + 2^2 + ..2^(k - 1) = 2^k - 1
// 乗る時間は最悪 1000000 * 20000 = 10^6 * 2 * 10^4 = 2 * 10^10
// 一方、                    2^10 > 10^3
//                           2^30 > 10^9
//                  2^30 * 2 * 10 > 2 * 10^10
// 2^30 * 2 * 2^4 > 2^30 * 2 * 10 > 2 * 10^10
//           2^35 >               > 2 * 10^10
// 35回以上乗る場合は待ち時間最短は乗車回数が少ない経路

constexpr long long int R = 1000000000ll + 7ll;

typedef tuple<unsigned long long int, unsigned long long int, unsigned long long int> long_time;

long_time to_long_time(unsigned long long int v) {
	return { 0, 0, v };
}

long_time operator+(long_time lt1, long_time lt2) {
	auto& [w1_2, w1_1, w1_0] = lt1;
	auto& [w2_2, w2_1, w2_0] = lt2;

	// ブロックごとの加算
	auto w0 = w1_0 + w2_0;
	auto w1 = w1_1 + w2_1;
	auto w2 = w1_2 + w2_2;

	// 桁上がり
	if (w0 >= R) {
		w1 += w0 / R;
		w0 %= R;
	}

	if (w1 >= R) {
		w2 += w1 / R;
		w1 %= R;
	}

	if (w2 >= R)
		w2 = 1;  // 正しい値ではないが、あふれるのでそれでも続けられるようにする。

	return { w2, w1, w0 };
}

long_time operator+=(long_time& lt1, long_time lt2) {
	return lt1 = lt1 + lt2;
}

long_time operator++(long_time& lt) {
	return lt = lt + lt;
}

ostream& operator<<(ostream& os, const long_time& lt) {
	auto& [w2, w1, w0] = lt;
	return os << "[" << w2 << " " << w1 << " " << w0 << "]";
}

bool less_than(int n_taxi1, long_time lt1, int n_taxi2, long_time lt2) {
	auto& [w1_2, w1_1, w1_0] = lt1;
	auto& [w2_2, w2_1, w2_0] = lt2;
	if (w1_2 == 0 && w2_2 == 0)
		return lt1 < lt2;

	if (w1_1 != 0 && w2_1 == 0)
		return false;

	if (w1_1 == 0 && w2_1 != 0)
		return true;

	//if (w1_1 != 0 && w2_1 != 0)
	if (n_taxi1 == n_taxi2)
		return lt1 < lt2;
	return n_taxi1 < n_taxi2;
}

class node;

// 枝 (道)
class edge {
public:
	enum class road_type {SIDEWALK, ROADWAY};

	edge(int a = 0, int b = 0, int c = 0, road_type t = road_type::SIDEWALK):
		node1(a),
		node2(b),
		transit_time(c),
		type(t) {
	}

	int get_transit_time() const {
		return transit_time;
	}

	int get_opposite_node(int i) const {
		return (i == node1) ? node2 : node1;
	}

	road_type get_type() const {
		return type;
	}
	
	friend ostream& operator<<(ostream& os, const edge& e);

private:
	int node1, node2, transit_time;
	road_type type;
};

// edgeクラス出力用関数 (デバッグ用)
ostream& operator<<(ostream& os, const edge& e) {
	return (e.type == edge::road_type::ROADWAY)
		? os << e.node1 << " =(" << e.get_transit_time() << ")=> " << e.node2 << endl
		: os << e.node1 << " -(" << e.get_transit_time() << ")-> " << e.node2 << endl;
}

// <回数, 全時間, 乗車中か, 前のノード>
typedef tuple<int, long_time, bool, int> state;

// state出力用関数 (デバッグ用)
ostream& operator<<(ostream& os, const state& s) {
	const auto& [n_taxi, total_time, onboard, id] = s;
	return os << "<" << n_taxi << ", " << total_time << ", " << onboard << "> " << id;
}

class node {
public:
	node():
		id(0),
		edge_list(),
		state_list() {
	}

	void set_id(int n) {
		id = n;
	}

	void add_edge(edge* p) {
		edge_list.push_back(p);
	}

	void transit(set<int>& node_id_list, vector<node>& node_list) {
		// このノードから行けるすべての枝について、
		// そこを通ったときの状態を登録する。
		for (auto& ep : edge_list) {
			const int opposite_id = ep->get_opposite_node(id);
			int si = 0;
			for (const auto& s : state_list) {
				const auto& [n, lt, o, i] = s;
				int n_taxi = n;
				auto new_lt = lt + to_long_time(ep->get_transit_time());
				bool onboard = o;

				if (ep->get_type() == edge::road_type::SIDEWALK) {  // 歩道
					onboard = false;
				}
				else {  // 車道
					if (!onboard) {
						// 新しく乗車を開始する。
						new_lt += wait_taxi(n_taxi);
						n_taxi++;
						onboard = true;
					}
				}
//				cerr << opposite_id << ": ";
				if (node_list[opposite_id].add_state(n_taxi, new_lt, onboard, id, si))
					node_id_list.insert(opposite_id);
				si++;
			}
		}
	}

	bool add_state(int n_taxi, long_time total_time, bool onboard, int id, int sid) {
		// これまでに登録された状態と比べて明らかに遅くなる場合は登録しない。
		if (!is_new_state(n_taxi, total_time, onboard))
			return false;

		// すでに登録されている状態が今回のより悪い場合は削除する。
		for (auto it = state_list.begin(); it != state_list.end(); ) {
			const auto& [n, tt, o, i] = *it;
			if (o == onboard && n_taxi <= n && !less_than(n, tt, n_taxi, total_time)) {  // onboardが共通で、回数・時間が共に小さい。
//				cerr << "remove: ";
//				cerr << *it;
//				cerr << " by " << state{n_taxi, total_time, onboard} << endl;
				it = state_list.erase(it);
			}
			else {
				++it;
			}
		}

		// 登録する。
		state_list.push_back({ n_taxi, total_time, onboard, id });
		return true;
	}

	unsigned long long int get_min_transit_time() {
		if (state_list.empty())
			return -1;

		// state_listに登録されているtotal_timeの最小値を得る。
		long_time m = get<1>(state_list[0]);
		int n = get<0>(state_list[0]);

		for (const auto& s : state_list) {
			const auto& [n_taxi, total_time, onboard, i] = s;
			if (less_than(n_taxi, total_time, n, m)) {
				m = total_time;
				n = n_taxi;
			}
		}
		return get<2>(m);
	}

	friend ostream& operator<<(ostream& os, const node& n);

private:
	// グラフのための変数
	int id;
	vector<edge*> edge_list;

	// 探索用データ
	vector<state> state_list;

	long_time wait_taxi(int n_taxi) {
//		cout << "n_taxi = " << n_taxi;
		long_time t = to_long_time(1);

		while (n_taxi-- >= 1)
			++t;

//		cout << ", t = " << t << endl;
		return t;
	}

	// これまでに登録された状態と比べて明らかに遅くならないか?
	bool is_new_state(int n_taxi, long_time total_time, bool onboard) {
		for (const auto& s : state_list) {
			const auto& [n, tt, o, i] = s;
			if (o == onboard && n <= n_taxi && !less_than(n_taxi, total_time, n, tt))  // onboardが共通で、回数・時間が共に大きい。
				return false;  // 登録しない。
		}
		return true;
	}
};

ostream& operator<<(ostream& os, const node& n) {
	os << "node " << n.id << endl;
	os << "states: " << endl;
	for (const auto& s : n.state_list) {
		os << "\t" << s << endl;
	}
	os << "edges: " << endl;
	for (const auto& e : n.edge_list) {
		os << "\t" << * e;
	}
	return os;
}

class time_flies {
public:
	// nnノード数, 
	time_flies(int n, int p, int q) :
		node_list(n),
		n_sidewalk(p),
		edge_list(p + q) {

		int i = 0;
		for (auto& n : node_list) {
			n.set_id(i++);
		}
	}

	void add_sidewalk(int id, int a, int b, int c) {
		edge_list[id] = { a, b, c, edge::road_type::SIDEWALK };
		auto edgep = &edge_list[id];

		node_list[a].add_edge(edgep);
		node_list[b].add_edge(edgep);
	}

	void add_roadway(int id, int d, int e, int f) {
		edge_list[n_sidewalk + id] = { d, e, f, edge::road_type::ROADWAY };
		auto edgep = &edge_list[n_sidewalk + id];

		node_list[d].add_edge(edgep);
		node_list[e].add_edge(edgep);
	}

	long long int get_min_transit_time(int id_begin, int id_end) {
		// 開始地点を初期設定する。
		node_list[id_begin].add_state(0, to_long_time(0), false, 0, 0);

		// 探索する。(各ノードの最短時間を求める)
		explore(id_begin);

//		cerr << *this;

		// 到着地点の情報から答を返す。
		return node_list[id_end].get_min_transit_time();
	}

	friend ostream& operator<<(ostream& os, const time_flies& n);

private:
	vector<edge> edge_list;  // 枝のリスト (前半が歩道、後半が車道)
	int n_sidewalk;  // 歩道の数
	vector<node> node_list;  // ノードのリスト

	// idから1遷移したノードに対し、状態を追加し、進める。
	void explore(int id) {
		set<int> node_id_list;
		node_id_list.insert(id);

		do {
			id = *node_id_list.begin();
			node_id_list.erase(node_id_list.begin());

			node_list[id].transit(node_id_list, node_list);
		} while (!node_id_list.empty());
	}
};

ostream& operator<<(ostream& os, const time_flies& tf) {
	for (const auto& n : tf.node_list) {
		os << n << endl;
	}
	return os;
}

int main() {

	for (int n, p, q; (cin >> n >> p >> q) && (n != 0 || p != 0 || q != 0);) {
		time_flies tf(n, p, q);
		for (int i = 0; i < p; i++) {
			int a, b, c;
			cin >> a >> b >> c;
			tf.add_sidewalk(i, a - 1, b - 1, c);
		}
		for (int j = 0; j < q; j++) {
			int d, e, f;
			cin >> d >> e >> f;
			tf.add_roadway(j, d - 1, e - 1, f);
		}

		cout << tf.get_min_transit_time(0, n - 1) % R << endl;
	}

	return 0;
}
