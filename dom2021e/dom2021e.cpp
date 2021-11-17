// dom2021e.cpp - 時は金なり
// https://icpc.iisf.or.jp/past-icpc/domestic2021/contest/all_ja.html#section_E
#include <algorithm>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

class node;

class edge {
public:
	enum class road_type {SIDEWALK, ROADWAY};

	edge(int a, int b, int c, road_type t, vector<node>& nl):
		node1(a),
		node2(b),
		transit_time(c),
		type(t),
		node_list(nl) {
	}

	void set(int a, int b, int c, road_type t) {
		node1 = a;
		node2 = b;
		transit_time = c;
		type = t;
	}

	int get_transit_time() const {
		return transit_time;
	}

	node* get_nodep(int i) const {
		return &node_list[i];
	}

	node* get_opposite_nodep(int i) const {
		return (i == node1) ? &node_list[node2] : &node_list[node1];
	}

private:
	int node1, node2, transit_time;
	road_type type;
	vector<node>& node_list;
};

class transaction {
public:
	transaction(int n, long long int tt, bool ob, int nt) :
		node(n), 
		total_time(tt), 
		onboard(ob), 
		n_taxi(nt) {
	}

	long long int get_total_time() {
		return total_time;
	}

	int get_node() {
		return node;
	}

	bool get_onboard() {
		return onboard;
	}

	int get_n_taxi() {
		return n_taxi;
	}

private:
	int node;
	long long int total_time;
	bool onboard;
	int n_taxi;
};


class node {
public:
	node(): 
		sidewalk(),
		roadway(),
		transit_time_min(0) {
	}

	void add_sidewalk(int n, edge* p) {
		sidewalk.push_back(make_pair(n, p));
	}

	void add_roadway(int n, edge* p) {
		roadway.push_back(make_pair(n, p));
	}

	// for debug
	void show() {
		cout << "sidewalk:";
		for (auto& i : sidewalk) {
			cout << " " << i.first;
		}
		cout << endl;
		cout << "roadway:";
		for (auto& i : roadway) {
			cout << " " << i.first;
		}
		cout << endl;
	}

	long long int get_transit_time_min() {
		return transit_time_min;
	}

	void get_all_candidates(vector<transaction>& candidate_list, long long int total_time, bool onboard, int n_taxi) {
		// このノードから行けるすべてのノードについて、
		// まだ時間が求まっていないノードが候補となる。

		// 歩道
		for (auto& i : sidewalk) {
			const int opposite_id = i.first;
			const edge* ep = i.second;
			if (ep->get_nodep(opposite_id)->get_transit_time_min() == 0) {
				candidate_list.push_back(transaction(opposite_id, total_time + ep->get_transit_time(), false, n_taxi));
			}
		}
		// 車道
		for (auto& i : roadway) {
			const int opposite_id = i.first;
			const edge* ep = i.second;
			const auto new_total_time = total_time + i.second->get_transit_time()
				+ (onboard) ? 0 : wait_taxi(n_taxi);

			if (ep->get_nodep(opposite_id)->get_transit_time_min() == 0)
				candidate_list.push_back(transaction(opposite_id, new_total_time, true, onboard ? n_taxi : n_taxi + 1));
		}
	}

	void set_total_time(long long int tt) {
		transit_time_min = tt;
	}

private:
	vector<pair<int, edge*>> sidewalk;
	vector<pair<int, edge*>> roadway;
	long long int transit_time_min;

	long long int wait_taxi(int n_taxi) {
		long long int t = 1;

		while (n_taxi > 1)
			t *= 2;

		return t;
	}
};

class time_flies {
public:
	// nnノード数, 
	time_flies(int n) :
		sidewalk_list(),
		roadway_list(),
		node_list(n) {}

	void add_sidewalk(int a, int b, int c) {
		sidewalk_list.push_back(edge(a, b, c, edge::road_type::SIDEWALK, node_list));
		auto edgep = &sidewalk_list[sidewalk_list.size() - 1];

		node_list[a].add_sidewalk(b, edgep);
		node_list[b].add_sidewalk(a, edgep);
	}

	void add_roadway(int d, int e, int f) {
		roadway_list.push_back(edge(d, e, f, edge::road_type::ROADWAY, node_list));
		auto edgep = &roadway_list[roadway_list.size() - 1];

		node_list[d].add_roadway(e, edgep);
		node_list[e].add_roadway(d, edgep);
	}

	long long int get_min_transit_time(int begin_id, int end_id, long long int total_time = 0, bool onboard = false, int n_taxi = 0) {
		if (begin_id == end_id) {
			// 到達した。
			return node_list[begin_id].get_transit_time_min();
		}

		// 幅優先探索
		// 
		// beginのノードからとなりに行けるノードのすべて経路についての時間を求め、リストを作成する。
		// 各経路について
		//    <行き先, 全時間, 乗車中か, 乗車回数>
		// 
		vector<transaction> new_candidate_list;
		node_list[begin_id].get_all_candidates(new_candidate_list, total_time, onboard, n_taxi);


		sort(new_candidate_list.begin(), new_candidate_list.end(), [](transaction& a, transaction& b) {
			return a.get_total_time() > b.get_total_time();
			});
		// 作成したリストをこれまでのリストに追加する。
		vector<transaction> current_candidate_list = candidate_list;
		merge(current_candidate_list.begin(), current_candidate_list.end(), 
			new_candidate_list.begin(), new_candidate_list.end(), 
			candidate_list.begin(), 
			[](transaction& a, transaction& b) {
				return a.get_total_time() > b.get_total_time();
			});

		// リストの中で最短の時間について、行き先ノードの所要時間を確定する。
		transaction& t = candidate_list.back();
		const auto fixed_node = t.get_node();
		total_time = t.get_total_time();
		node_list[fixed_node].set_total_time(total_time);
		
		
		// 確定した行き先ノードを含む候補をすべてリストから除く。
		for (size_t i = 0; i < candidate_list.size(); i++) {
			if (candidate_list[i].get_node() == fixed_node)
				candidate_list.erase(candidate_list.begin() + i);
		}

		// 確定した行き先ノードから再帰的に探索を続ける。
		return get_min_transit_time(fixed_node, end_id, total_time, t.get_onboard(), t.get_n_taxi());
	}

private:
	vector<edge> sidewalk_list;
	vector<edge> roadway_list;
	vector<node> node_list;
	vector<transaction> candidate_list;
};

int main() {

	for (int n, p, q; (cin >> n >> p >> q) && (n != 0 || p != 0 || q != 0);) {
		time_flies tf(n);
		for (int i = 0; i < p; i++) {
			int a, b, c;
			cin >> a >> b >> c;
			tf.add_sidewalk(a - 1, b - 1, c);
		}
		for (int j = 0; j < q; j++) {
			int d, e, f;
			cin >> d >> e >> f;
			tf.add_roadway(d - 1, e - 1, f);
		}

		cout << tf.get_min_transit_time(0, n - 1) % (1000000000 + 7) << endl;
	}

	return 0;
}
