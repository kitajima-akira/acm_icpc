// dom2021f.cpp - ������`�ȉ����l
// ��� https://icpc.iisf.or.jp/past-icpc/domestic2021/contest/all_ja.html#section_F
// ��� https://icpc.iisf.or.jp/past-icpc/domestic2021/commentaries.html#F
// ���A�����������̈Ӗ��ƃA���S���Y�� https://manabitimes.jp/math/1250
// C++17
// -----------------------------------------------------------------------------
#include <iostream>
#include <queue>
#include <stack>
#include <set>
#include <vector>
using namespace std;

constexpr int not_reached = -1;

// �L���O���t
class directed_graph {
public:
	typedef pair<int, int> edge;  // ��
	typedef set<int> subgraph;  // �T�u�O���t

	// ���_��n���w�肵�ď����ݒ肷��B
	directed_graph(int n) :
		virtex_list(n, { 0, set<int>{}, set<int>{}, false, not_reached }) {
		for (int i = 0; i < n; i++)
			virtex_list[i].id = i;
	}

	int get_virtex_count() {
		return virtex_list.size();
	}

	// n_from����n_to�ւ̕ӂ�ǉ�����B
	void add_edge(int n_from, int n_to) {
		virtex_list[n_from].from_edges.insert(n_to);
		virtex_list[n_to].to_edges.insert(n_from);
	}

	// ���A�������������s���B
	// ���������T�u�O���t��Ԃ��B
	vector<subgraph> find_strongly_connected_components() {
		stack<int> sequence;

		for (auto& v : virtex_list)
			find_sequence(v, 0, sequence);

		return find_scc(sequence);
	}

	// �e���A���������k�񂷂�B
	// �k�񂵂��O���t�ƁA���Ƃ̒��_�ɑ΂���k�񂵂����_�̃��X�g�𓾂�B
	tuple<directed_graph, vector<int>, vector<subgraph>> shrink() {
		// ���A�����������߂�B
		auto sccg = find_strongly_connected_components();
		directed_graph sg(sccg.size());  // �k�񂵂��O���t

		// �m�[�h�ԍ�����T�u�O���t�̔ԍ���������悤�\�����B
		vector<int> submap(virtex_list.size());
		for (size_t si = 0; si < sccg.size(); si++)
			for (auto& i : sccg[si])
				submap[i] = si;

		// �e�T�u�O���t�ɂ���
		for (size_t si = 0; si < sccg.size(); si++)
			// ���_���Ƃ�
			for (auto& i : sccg[si])
				// �e�ӂ�
				for (auto& v : virtex_list[i].from_edges)
					// �s����𒲂�
					if (submap[v] != si)  // ���̃T�u�O���t�ւ̕ӂ������
						// �ӂ�o�^����B
						sg.add_edge(si, submap[v]);

		return { sg, submap, sccg };
	}

	// DAG��(�ŏ��̖{����)�ӂ�ǉ����ċ��A���ɂ���B
	// �ǉ������ӂ̃��X�g��Ԃ��B
	vector<edge> to_strong_connected_graph() {
		vector<edge> added_edges;

		// �e�\�[�X����V���N�ւ̌o�H�����߂�B
		// �o�H�̓V���N���݂��ɑf�ɂȂ�悤�ɋɑ�Ɏ��B
		// �e�o�H������őS�̂���̑傫�ȗL���o�H�ƂȂ�悤�ɕӂ�ǉ�����B
		int new_from = not_reached;  // �V�����V���N����\�[�X�ւ̕ӂ̎n�_
		int first_to = not_reached;  // �傫�ȗL���o�H�ƂȂ�o�H�̎n�_
		for (auto& v : virtex_list) {
			if (!v.to_edges.empty())  // �\�[�X�ł͂Ȃ�?
				continue;

			auto s = find_sink(v, true); // �V���N���݂��ɑf�ɂȂ�悤�ɑI�ԁB
			if (s >= 0 || (s = find_sink(v, false)) >= 0) {  // �݂��ɑf�ɂł��Ȃ���΁A���B�ł���ǂꂩ���I�ԁB
				if (first_to < 0)
					first_to = v.id;

				if (new_from >= 0) {
					// ��O�̃V���N�ƍ��̃\�[�X���Ȃ���B
					add_edge(new_from, v.id);
					added_edges.push_back({ new_from, v.id });
				}
				new_from = s;
			}
		}

		// �ɑ�Ɏ�����L�����̒[�_�ԂɁC�S�̂���̑傫�ȗL���H���Ȃ��悤�ɗL���ӂ�ǉ�����D
		if (new_from >= 0 && new_from != first_to) {
			// �Ō�Ɍ��������V���N�ƍŏ��̃\�[�X���Ȃ���B
			add_edge(new_from, first_to);
			added_edges.push_back({ new_from, first_to });
		}

		// �c�����V���N����C�ӂ̃\�[�X�Ɍ������L���ӂ� 1 �{���ǉ�����D
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
		int id;  // �ʂ��ԍ�
		set<int> from_edges;  // ���̒��_����o�Ă���ӂ̌��������̒��_�̃��X�g
		set<int> to_edges;  // ���̒��_�ɓ����Ă���ӂ̌��������̒��_�̃��X�g
		bool visited;  // �������ɒʂ������ǂ���
		int order;  // ��������Ƃ��̏���
	};

	vector<virtex> virtex_list;  // ���̃O���t�̑S���_�̃��X�g

	// �����̏��Ԃ𓾂�B
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

	// ���A���������������B
	bool find_an_scc(virtex& v, subgraph& g) {
		for (;;) {
			// ���łɒʂ���(visited�̘_���͋t)�����ׂ�B
			if (!v.visited)
				return false;

			v.visited = false;

			// v��g�ɒǉ�����B
			g.insert(v.id);

			// v�Ɍ������ӂ����ɒ��ׂ�B
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
		// ���_�̏��
		os << "node " << v.id << ":";
		if (v.visited)
			os << " *";
		if (v.order >= 0)
			os << "(" << v.order << ")";
		os << endl;

		// �ӂ̏��
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

// 2���O���t
class bipartite_graph {
public:
	typedef pair<int, int> edge;  // ��
	typedef vector<edge> edge_list;  // �ӂ̃��X�g

	// ��̃m�[�h�W���̃m�[�h���ŏ���������B
	bipartite_graph(int nu, int nv):
		set_u(nu, { 0, &v_dummy, 0, set<virtex*>() }),
		set_v(nv, { 0, &u_dummy, 0, set<virtex*>() }),
		u_dummy(),
		v_dummy() {
		// id��ݒ肷��B
		for (size_t i = 0; i < set_u.size(); i++)
			set_u[i].id = i;
		for (size_t i = 0; i < set_v.size(); i++)
			set_v[i].id = i;
	}

	// u����v�ւ̕ӂ�ǉ�����B
	void add_edge(int u, int v) {
		set_u[u].adjacent.insert(&set_v[v]);
		set_v[v].adjacent.insert(&set_u[u]);
	}

	// �ő�}�b�`���O�����߂�B
	// Hopcroft-Karp algorithm
	// https://en.wikipedia.org/wiki/Hopcroft%E2%80%93Karp_algorithm
	edge_list get_maximum_cardinality_matching() {
		hopcroft_karp();

		// ���������ӂ����ׂĎ��o���B
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
	// distance���X�V����B
	bool update_distance(int infinity) {
		queue<virtex*> candidate;  // ���ׂ钸�_�̃L���[

		// set_u�̋N�_ (�܂��y�A���������Ă��Ȃ����_) ��������B
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

		// �N�_����̋������X�V����B
		while (!candidate.empty()) {
			// ���̌��𓾂�B
			const virtex* up = candidate.front();
			candidate.pop();

			// v���o�R���čs����A����u��������B
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

	// �L�΂���Ƃ��ɐL�΂��B
	bool extend_path(virtex* up, int infinity) {
		if (up != &u_dummy) {
			for (auto vp : up->adjacent) {
				if (vp->pair_nodep->distance == up->distance + 1
					&& extend_path(vp->pair_nodep, infinity)) {
					// �y�A��ǉ�����B
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

	// �}�b�`���O(�e���_��pair_nodep)�����߂�B
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
		// ���̓O���t�̊��S�}�b�`���O�����߂�B
		auto matched_edges = get_maximum_cardinality_matching();

		if (matched_edges.size() != this->set_u.size()) {
			cout << "wrong matching" << endl;
			exit(1);
		}
//		cerr << *this << endl;

		// �L���O���tg�����B
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

		// g�����A�������������ďk�񂵂�DAGsg�����B
		auto [sg, submap, sccg] = g.shrink();

//		cerr << sg << endl;

		// sg�����A��������B
		auto added_edges = sg.to_strong_connected_graph();
		if (sg.get_virtex_count() <= 1)
			return {};
//		cerr << sg << endl;

		// �ǉ����ꂽ�ӂ����̃O���t�̕ӂɒ����B
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
			//const int au = *(sccg[su].lower_bound(offset));  // lower_bound()����������?
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
