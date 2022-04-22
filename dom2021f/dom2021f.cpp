// dom2021f.cpp - 完璧主義な王女様
// 問題 https://icpc.iisf.or.jp/past-icpc/domestic2021/contest/all_ja.html#section_F
// 解説 https://icpc.iisf.or.jp/past-icpc/domestic2021/commentaries.html#F
// 強連結成分分解の意味とアルゴリズム https://manabitimes.jp/math/1250
// C++17
// -----------------------------------------------------------------------------
#include <iostream>
#include <queue>
#include <stack>
#include <set>
#include <vector>
using namespace std;

constexpr int not_reached = -1;

// 有向グラフ
class directed_graph {
public:
	typedef pair<int, int> edge;  // 辺
	typedef set<int> subgraph;  // サブグラフ

	// 頂点数nを指定して初期設定する。
	directed_graph(int n) :
		virtex_list(n, { 0, set<int>{}, set<int>{}, false, not_reached }) {
		for (int i = 0; i < n; i++)
			virtex_list[i].id = i;
	}

	int get_virtex_count() {
		return virtex_list.size();
	}

	// n_fromからn_toへの辺を追加する。
	void add_edge(int n_from, int n_to) {
		virtex_list[n_from].from_edges.insert(n_to);
		virtex_list[n_to].to_edges.insert(n_from);
	}

	// 強連結成分分解を行う。
	// 分解したサブグラフを返す。
	vector<subgraph> find_strongly_connected_components() {
		stack<int> sequence;

		for (auto& v : virtex_list)
			find_sequence(v, 0, sequence);

		return find_scc(sequence);
	}

	// 各強連結成分を縮約する。
	// 縮約したグラフと、もとの頂点に対する縮約した頂点のリストを得る。
	tuple<directed_graph, vector<int>, vector<subgraph>> shrink() {
		// 強連結成分を求める。
		auto sccg = find_strongly_connected_components();
		directed_graph sg(sccg.size());  // 縮約したグラフ

		// ノード番号からサブグラフの番号を引けるよう表を作る。
		vector<int> submap(virtex_list.size());
		for (size_t si = 0; si < sccg.size(); si++)
			for (auto& i : sccg[si])
				submap[i] = si;

		// 各サブグラフについて
		for (size_t si = 0; si < sccg.size(); si++)
			// 頂点ごとに
			for (auto& i : sccg[si])
				// 各辺の
				for (auto& v : virtex_list[i].from_edges)
					// 行き先を調べ
					if (submap[v] != si)  // 他のサブグラフへの辺があれば
						// 辺を登録する。
						sg.add_edge(si, submap[v]);

		return { sg, submap, sccg };
	}

	// DAGに(最小の本数の)辺を追加して強連結にする。
	// 追加した辺のリストを返す。
	vector<edge> to_strong_connected_graph() {
		vector<edge> added_edges;

		// 各ソースからシンクへの経路を求める。
		// 経路はシンクが互いに素になるように極大に取る。
		// 各経路を結んで全体が一つの大きな有向経路となるように辺を追加する。
		int new_from = not_reached;  // 新しいシンクからソースへの辺の始点
		int first_to = not_reached;  // 大きな有向経路となる経路の始点
		for (auto& v : virtex_list) {
			if (!v.to_edges.empty())  // ソースではない?
				continue;

			auto s = find_sink(v, true); // シンクが互いに素になるように選ぶ。
			if (s >= 0 || (s = find_sink(v, false)) >= 0) {  // 互いに素にできなければ、到達できるどれか一つを選ぶ。
				if (first_to < 0)
					first_to = v.id;

				if (new_from >= 0) {
					// 一つ前のシンクと今のソースをつなげる。
					add_edge(new_from, v.id);
					added_edges.push_back({ new_from, v.id });
				}
				new_from = s;
			}
		}

		// 極大に取った有向道の端点間に，全体が一つの大きな有向閉路をなすように有向辺を追加する．
		if (new_from >= 0 && new_from != first_to) {
			// 最後に見つかったシンクと最初のソースをつなげる。
			add_edge(new_from, first_to);
			added_edges.push_back({ new_from, first_to });
		}

		// 残ったシンクから任意のソースに向かう有向辺を 1 本ずつ追加する．
		for (auto& v : virtex_list) {
			if (v.from_edges.empty()) {
				add_edge(v.id, first_to);
				added_edges.push_back({ v.id, first_to });
			}
		}

		return added_edges;
	}

	friend ostream& operator<<(ostream& os, directed_graph& g);

private:
	struct virtex {
		int id;  // 通し番号
		set<int> from_edges;  // この頂点から出ている辺の向こう側の頂点のリスト
		set<int> to_edges;  // この頂点に入ってくる辺の向こう側の頂点のリスト
		bool visited;  // 処理中に通ったかどうか
		int order;  // 処理するときの順番
	};

	vector<virtex> virtex_list;  // このグラフの全頂点のリスト

	// 処理の順番を得る。
	int find_sequence(virtex& v, int depth, stack<int>& sequence) {
		if (v.visited)
			return depth;
		v.visited = true;

		for (auto i : v.from_edges)
			depth = find_sequence(virtex_list[i], depth, sequence);

		if (v.order < 0) {
			v.order = depth;
			depth++;
			sequence.push(v.id);
		}

		return depth;
	}

	// 強連結成分を一つ見つける。
	bool find_an_scc(virtex& v, subgraph& g) {
		for (;;) {
			// すでに通った(visitedの論理は逆)か調べる。
			if (!v.visited)
				return false;

			v.visited = false;

			// vをgに追加する。
			g.insert(v.id);

			// vに向かう辺を順に調べる。
			for (auto i : v.to_edges)
				find_an_scc(virtex_list[i], g);
		}

		return true;
	}

	vector<subgraph> find_scc(stack<int>& sequence) {
		vector<subgraph> glist;
		for (; !sequence.empty(); sequence.pop()) {
			subgraph g;
			if (find_an_scc(virtex_list[sequence.top()], g))
				glist.push_back(g);
		}
		return glist;
	}

	int find_sink(virtex& v, bool free_sink_only) {
		if (v.from_edges.empty()) {
			if (free_sink_only && v.visited)
				return not_reached;
			v.visited = true;
			return v.id;
		}

		for (auto& i : v.from_edges) {
			int s = find_sink(virtex_list[i], free_sink_only);
			if (s >= 0)
				return s;
		}

		return not_reached;
	}
};

ostream& operator<<(ostream& os, directed_graph& g) {
	for (auto& v : g.virtex_list) {
		// 頂点の情報
		os << "node " << v.id << ":";
		if (v.visited)
			os << " *";
		if (v.order >= 0)
			os << "(" << v.order << ")";
		os << endl;

		// 辺の情報
		os << "\t>";
		for (const auto& i : v.from_edges) {
			os << " " << i;
		}
		os << "  [ <";
		for (const auto& i : v.to_edges) {
			os << " " << i;
		}
		os << " ]" << endl;
	}
	return os;
}

void test_directed_graph() {
	directed_graph g(8);
	g.add_edge(0, 4);
	g.add_edge(0, 5);
	g.add_edge(1, 5);
	g.add_edge(2, 5);
	g.add_edge(2, 6);
	g.add_edge(2, 7);
	g.add_edge(3, 7);
	g.add_edge(4, 0);
	g.add_edge(5, 1);
	g.add_edge(6, 2);
	g.add_edge(7, 3);

//	directed_graph g(8);
//	g.add_edge(0, 1);
//	g.add_edge(1, 2);
//	g.add_edge(2, 0);
	//g.add_edge(2, 3);
	//g.add_edge(3, 4);
	//g.add_edge(4, 5);
	//g.add_edge(5, 4);
	//g.add_edge(5, 6);
	//g.add_edge(5, 7);

	cout << g << endl;

	auto [sg, submap, sccg] = g.shrink();

	cout << "[scc]" << endl;
	for (size_t i = 0; i < sccg.size(); i++) {
		cout << i << ":";
		for (auto& n : sccg[i])
			cout << " " << n;
		cout << endl;
	}

	cout << sg << endl;

	sg.to_strong_connected_graph();

	cout << sg << endl;

	exit(0);
}

// 2部グラフ
class bipartite_graph {
public:
	typedef pair<int, int> edge;  // 辺
	typedef vector<edge> edge_list;  // 辺のリスト

	// 二つのノード集合のノード数で初期化する。
	bipartite_graph(int nu, int nv):
		set_u(nu, { 0, &v_dummy, 0, set<virtex*>() }),
		set_v(nv, { 0, &u_dummy, 0, set<virtex*>() }),
		u_dummy(),
		v_dummy() {
		// idを設定する。
		for (size_t i = 0; i < set_u.size(); i++)
			set_u[i].id = i;
		for (size_t i = 0; i < set_v.size(); i++)
			set_v[i].id = i;
	}

	// uからvへの辺を追加する。
	void add_edge(int u, int v) {
		set_u[u].adjacent.insert(&set_v[v]);
		set_v[v].adjacent.insert(&set_u[u]);
	}

	// 最大マッチングを求める。
	// Hopcroft-Karp algorithm
	// https://en.wikipedia.org/wiki/Hopcroft%E2%80%93Karp_algorithm
	edge_list get_maximum_cardinality_matching() {
		hopcroft_karp();

		// 見つかった辺をすべて取り出す。
		edge_list matched_edges;
		for (const auto& u : set_u)
			if (u.pair_nodep != &v_dummy)
				matched_edges.push_back({ u.id, u.pair_nodep->id });

		return matched_edges;
	}

	friend ostream& operator<<(ostream& os, bipartite_graph& g);

protected:
	struct virtex {
		int id;
		virtex* pair_nodep;
		int distance;
		set<virtex*> adjacent;
	};
	vector<virtex> set_u;
	vector<virtex> set_v;
	virtex u_dummy, v_dummy;

private:
	// distanceを更新する。
	bool update_distance(int infinity) {
		queue<virtex*> candidate;  // 調べる頂点のキュー

		// set_uの起点 (まだペアが見つかっていない頂点) を見つける。
		for (auto& u : set_u) {
			if (u.pair_nodep == &v_dummy) {
				u.distance = 0;
				candidate.push(&u);
			}
			else {
				u.distance = infinity;
			}
		}
		u_dummy.distance = infinity;

		// 起点からの距離を更新する。
		while (!candidate.empty()) {
			// 次の候補を得る。
			const virtex* up = candidate.front();
			candidate.pop();

			// vを経由して行ける、次のuを見つける。
			if (up->distance < u_dummy.distance) {
				for (auto vp : up->adjacent) {
					const auto aup = vp->pair_nodep;
					if (aup->distance == infinity) {
						aup->distance = up->distance + 1;
						candidate.push(aup);
					}
				}
			}
		}
		return u_dummy.distance != infinity;
	}

	// 伸ばせるときに伸ばす。
	bool extend_path(virtex* up, int infinity) {
		if (up != &u_dummy) {
			for (auto vp : up->adjacent) {
				if (vp->pair_nodep->distance == up->distance + 1
					&& extend_path(vp->pair_nodep, infinity)) {
					// ペアを追加する。
					vp->pair_nodep = up;
					up->pair_nodep = vp;
					return true;
				}
			}
			up->distance = infinity;
			return false;
		}
		return true;
	}

	// マッチング(各頂点のpair_nodep)を求める。
	int hopcroft_karp() {
		int infinity = max(set_u.size(), set_v.size()) + 1;
		int matching = 0;
		while (update_distance(infinity))
			for (auto& u : set_u)
				if (u.pair_nodep == &v_dummy && extend_path(&u, infinity))
					matching++;
		return matching;
	}
};

ostream& operator<<(ostream& os, bipartite_graph& g) {
	const size_t n = max(g.set_u.size(), g.set_v.size());
	for (size_t i = 0; i < n; i++) {
		if (i < g.set_u.size()) {
			os << "node " << i << ":" << endl;
			os << " >";
			const auto up = &g.set_u[i];
			for (auto& vp : up->adjacent) {
				os << " " << vp->id;
				if (vp->pair_nodep == up)
					os << "*";
			}
			os << endl;
		}
		if (i < g.set_v.size()) {
			const string prefix = "\t\t\t";
			const auto& edges_v = g.set_v[i].adjacent;
			os << prefix + "node " << i << endl;
		}
	}
	return os;
}

void test_bipartite_graph() {
	bipartite_graph g(9, 9);
	g.add_edge(0, 0);
	g.add_edge(0, 1);
	g.add_edge(0, 2);
	g.add_edge(1, 2);
	g.add_edge(2, 2);
	g.add_edge(2, 3);
	g.add_edge(3, 3);
	g.add_edge(3, 4);
	g.add_edge(3, 5);
	g.add_edge(4, 5);
	g.add_edge(5, 5);
	g.add_edge(5, 6);
	g.add_edge(6, 6);
	g.add_edge(6, 7);
	g.add_edge(6, 8);
	g.add_edge(7, 8);
	g.add_edge(8, 8);

	cout << g << endl;

	auto el = g.get_maximum_cardinality_matching();

	cout << g << endl;

	exit(0);
}


class perfectionist_princess: public bipartite_graph {
public:
	typedef pair<int, int> training_record;

	perfectionist_princess(int n, int m): bipartite_graph(n, n) {}

	vector<training_record> solve() {
		// 入力グラフの完全マッチングを求める。
		auto matched_edges = get_maximum_cardinality_matching();

		if (matched_edges.size() != this->set_u.size()) {
			cout << "wrong matching" << endl;
			exit(1);
		}
//		cerr << *this << endl;

		// 有向グラフgを作る。
		directed_graph g(set_u.size() + set_v.size());
		const size_t offset = set_u.size();
		for (const auto& u : set_u) {
			if (u.pair_nodep != &v_dummy) {
				const auto vp = u.pair_nodep;
				g.add_edge(offset + vp->id, u.id);
				g.add_edge(u.id, offset + vp->id);
			}
			for (const auto& vp : u.adjacent)
				g.add_edge(u.id, offset + vp->id);
		}

//		cerr << "offset = " << offset << endl;
//		cerr << g << endl;

		// gを強連結成分分解して縮約したDAGsgを作る。
		auto [sg, submap, sccg] = g.shrink();

//		cerr << sg << endl;

		// sgを強連結化する。
		auto added_edges = sg.to_strong_connected_graph();
		if (sg.get_virtex_count() <= 1)
			return {};
//		cerr << sg << endl;

		// 追加された辺を元のグラフの辺に直す。
		vector<training_record> trlist;
		for (auto& e : added_edges) {
			const int su = e.first, sv = e.second;
			int au;
			for (auto& u : sccg[su]) {
				if (u < offset) {
					au = u;
					break;
				}
			}
			//const int au = *(sccg[su].lower_bound(offset));  // lower_bound()がおかしい?
			const int av = *(sccg[sv].upper_bound(offset - 1)) - offset;
			trlist.push_back({ au, av });
		}
		return trlist;
	}

private:
};

ostream& operator<<(ostream& os, const perfectionist_princess::training_record& tr) {
	auto& [spy, task] = tr;
	return os << spy + 1 << " " << task + 1;
}


int main() {
//	test_directed_graph();
	test_bipartite_graph();

	for (int n, m; (cin >> n >> m) && (n && m); ) {
		perfectionist_princess pp(n, m);

		for (int i = 0; i < m; i++) {
			int s, t;
			cin >> s >> t;
			pp.add_edge(s - 1, t - 1);
		}

		auto training_list = pp.solve();
		cout << training_list.size() << endl;
		for (const auto& t : training_list)
			cout << t << endl;
	}
	return 0;
}
