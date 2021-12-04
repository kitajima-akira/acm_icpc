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

constexpr long long int R = 1000000000LL + 7LL;
constexpr int wait_threshold = 35;

typedef tuple<int, long long int> taxi_time;

long long int round(long long int v) {
	return (v >= R) ? v % R : v;
}

taxi_time operator+(taxi_time tt1, taxi_time tt2) {
	auto& [nt1, et1] = tt1;
	auto& [nt2, et2] = tt2;

	auto nt = nt1 + nt2;
	auto et = et1 + et2;

	return { nt, round(et) };
}

ostream& operator<<(ostream& os, const taxi_time& tt) {
	auto& [nt, et] = tt;
	return os << "[" << nt <<  " " << et << "]";
}

// 乗車回数k回のとき、総待ち時間は 2^(k + 1) - 1
long long int total_wait_time(int n_taxi) {
	constexpr int bit_width = sizeof(long long int) * 8 - 1;

	if (n_taxi < bit_width)
		return (1LL << n_taxi) - 1;

	long long int w = round((1LL << (bit_width - 1)) - 1);
	for (int i = bit_width - 1; i < n_taxi; i++)
		w = round(2 * (w + 1) - 1);
	return w;
}

long long int total_transit_time(taxi_time tt) {
	auto& [nt, et] = tt;
	return et + round(total_wait_time(nt));
}

bool less_than(taxi_time tt1, taxi_time tt2) {
	auto& [nt1, et1] = tt1;
	auto& [nt2, et2] = tt2;

	if (nt1 == nt2)
		return et1 < et2;

	if (nt1 < wait_threshold && nt2 < wait_threshold)
		// この条件では待ち時間が長くても丸めてはいけない。
		return et1 + total_wait_time(nt1) < et2 + total_wait_time(nt2);

	if (nt1 >= wait_threshold && nt2 < wait_threshold)
		return false;

	if (nt1 < wait_threshold && nt2 >= wait_threshold)
		return true;

	// if (nt1 >= wait_threshold && nt2 >= wait_threshold && nt1 != nt2) 

	return nt1 < nt2;
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

// <時間, 乗車中か, 前のノード>
typedef tuple<taxi_time, bool, int> state;

// state出力用関数 (デバッグ用)
ostream& operator<<(ostream& os, const state& s) {
	const auto& [total_time, onboard, id] = s;
	return os << "<" << total_time << ", " << onboard << "> " << id;
}

class node {
public:
	node() :
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
			for (const auto& s : state_list) {
				const auto& [tt, o, i] = s;
				if (opposite_id == i)  // 一度行って戻る
					continue;  // 登録する必要なし
				const auto& [n, et] = tt;
				int n_taxi = n;
				auto new_et = et + ep->get_transit_time();
				bool onboard = o;

				if (ep->get_type() == edge::road_type::SIDEWALK) {  // 歩道
					onboard = false;
				}
				else {  // 車道
					if (!onboard) {
						// 新しく乗車を開始する。
						n_taxi++;
						onboard = true;
					}
				}
//				cerr << opposite_id << ": ";
				if (node_list[opposite_id].add_state({ n_taxi, new_et }, onboard, id))
					node_id_list.insert(opposite_id);
			}
		}
	}

	bool add_state(taxi_time total_time, bool onboard, int id) {
		// これまでに登録された状態と比べて明らかに遅くなる場合は登録しない。
		if (!is_new_state(total_time, onboard))
			return false;

		// すでに登録されている状態が今回のより悪い場合は削除する。
		for (auto it = state_list.begin(); it != state_list.end(); ) {
			const auto& [tt, o, i] = *it;
			if (o == onboard 
				&& get<0>(tt) >= get<0>(total_time)
				&& !less_than(tt, total_time)) {  // onboardが共通で、回数・時間が共に小さい。
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
		state_list.push_back({ total_time, onboard, id });
		return true;
	}

	long long int get_min_transit_time() {
		if (state_list.empty())
			return -1;

		// state_listに登録されているtotal_timeの最小値を得る。
		taxi_time m = get<0>(state_list[0]);

		for (const auto& s : state_list) {
			const auto& [total_time, onboard, i] = s;
//			cerr << "goal: " << total_transit_time(total_time) << endl;
			if (less_than(total_time, m)) {
				m = total_time;
			}
		}
		return total_transit_time(m);
	}

	friend ostream& operator<<(ostream& os, const node& n);

private:
	// グラフのための変数
	int id;
	vector<edge*> edge_list;

	// 探索用データ
	vector<state> state_list;

	// これまでに登録された状態と比べて明らかに遅くならないか?
	bool is_new_state(taxi_time total_time, bool onboard) {
		for (const auto& s : state_list) {
			const auto& [tt, o, i] = s;
			if (o == onboard 
				&& get<0>(total_time) >= get<0>(tt) 
				&& !less_than(total_time, tt))  // onboardが共通で、回数・時間が共に大きい。
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
		node_list[id_begin].add_state({ 0, 0 }, false, 0);

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
	for (int n, p, q; (cin >> n >> p >> q) && (n || p || q);) {
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
