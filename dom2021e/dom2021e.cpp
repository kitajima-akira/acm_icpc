// dom2021e.cpp - ���͋��Ȃ�
// https://icpc.iisf.or.jp/past-icpc/domestic2021/contest/all_ja.html#section_E
// -----------------------------------------------------------------------------
#include <algorithm>
#include <iostream>
#include <set>
#include <tuple>
#include <vector>
using namespace std;

// �����v�Z�p��`

// ���̐ݒ���
// n <= 20000, p <= 20000, q <= 20000, c <= 1000000
// �^�N�V�[�̑҂����Ԃ�k����� 2^0 + 2^1 + 2^2 + ..2^(k - 1) = 2^k - 1
// ��鎞�Ԃ͍ň� 1000000 * 20000 = 10^6 * 2 * 10^4 = 2 * 10^10
// ����A                    2^10 > 10^3
//                           2^30 > 10^9
//                  2^30 * 2 * 10 > 2 * 10^10
// 2^30 * 2 * 2^4 > 2^30 * 2 * 10 > 2 * 10^10
//           2^35 >               > 2 * 10^10
// 35��ȏ���ꍇ�͑҂����ԍŒZ�͏�ԉ񐔂����Ȃ��o�H

constexpr long long int R = 1000000000ll + 7ll;

typedef tuple<unsigned long long int, unsigned long long int, unsigned long long int> long_time;

long_time to_long_time(unsigned long long int v) {
	return { 0, 0, v };
}

long_time operator+(long_time lt1, long_time lt2) {
	auto& [w1_2, w1_1, w1_0] = lt1;
	auto& [w2_2, w2_1, w2_0] = lt2;

	// �u���b�N���Ƃ̉��Z
	auto w0 = w1_0 + w2_0;
	auto w1 = w1_1 + w2_1;
	auto w2 = w1_2 + w2_2;

	// ���オ��
	if (w0 >= R) {
		w1 += w0 / R;
		w0 %= R;
	}

	if (w1 >= R) {
		w2 += w1 / R;
		w1 %= R;
	}

	if (w2 >= R)
		w2 = 1;  // �������l�ł͂Ȃ����A���ӂ��̂ł���ł���������悤�ɂ���B

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

// �} (��)
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

// edge�N���X�o�͗p�֐� (�f�o�b�O�p)
ostream& operator<<(ostream& os, const edge& e) {
	return (e.type == edge::road_type::ROADWAY)
		? os << e.node1 << " =(" << e.get_transit_time() << ")=> " << e.node2 << endl
		: os << e.node1 << " -(" << e.get_transit_time() << ")-> " << e.node2 << endl;
}

// <��, �S����, ��Ԓ���, �O�̃m�[�h>
typedef tuple<int, long_time, bool, int> state;

// state�o�͗p�֐� (�f�o�b�O�p)
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
		// ���̃m�[�h����s���邷�ׂĂ̎}�ɂ��āA
		// ������ʂ����Ƃ��̏�Ԃ�o�^����B
		for (auto& ep : edge_list) {
			const int opposite_id = ep->get_opposite_node(id);
			int si = 0;
			for (const auto& s : state_list) {
				const auto& [n, lt, o, i] = s;
				int n_taxi = n;
				auto new_lt = lt + to_long_time(ep->get_transit_time());
				bool onboard = o;

				if (ep->get_type() == edge::road_type::SIDEWALK) {  // ����
					onboard = false;
				}
				else {  // �ԓ�
					if (!onboard) {
						// �V������Ԃ��J�n����B
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
		// ����܂łɓo�^���ꂽ��ԂƔ�ׂĖ��炩�ɒx���Ȃ�ꍇ�͓o�^���Ȃ��B
		if (!is_new_state(n_taxi, total_time, onboard))
			return false;

		// ���łɓo�^����Ă����Ԃ�����̂�舫���ꍇ�͍폜����B
		for (auto it = state_list.begin(); it != state_list.end(); ) {
			const auto& [n, tt, o, i] = *it;
			if (o == onboard && n_taxi <= n && !less_than(n, tt, n_taxi, total_time)) {  // onboard�����ʂŁA�񐔁E���Ԃ����ɏ������B
//				cerr << "remove: ";
//				cerr << *it;
//				cerr << " by " << state{n_taxi, total_time, onboard} << endl;
				it = state_list.erase(it);
			}
			else {
				++it;
			}
		}

		// �o�^����B
		state_list.push_back({ n_taxi, total_time, onboard, id });
		return true;
	}

	unsigned long long int get_min_transit_time() {
		if (state_list.empty())
			return -1;

		// state_list�ɓo�^����Ă���total_time�̍ŏ��l�𓾂�B
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
	// �O���t�̂��߂̕ϐ�
	int id;
	vector<edge*> edge_list;

	// �T���p�f�[�^
	vector<state> state_list;

	long_time wait_taxi(int n_taxi) {
//		cout << "n_taxi = " << n_taxi;
		long_time t = to_long_time(1);

		while (n_taxi-- >= 1)
			++t;

//		cout << ", t = " << t << endl;
		return t;
	}

	// ����܂łɓo�^���ꂽ��ԂƔ�ׂĖ��炩�ɒx���Ȃ�Ȃ���?
	bool is_new_state(int n_taxi, long_time total_time, bool onboard) {
		for (const auto& s : state_list) {
			const auto& [n, tt, o, i] = s;
			if (o == onboard && n <= n_taxi && !less_than(n_taxi, total_time, n, tt))  // onboard�����ʂŁA�񐔁E���Ԃ����ɑ傫���B
				return false;  // �o�^���Ȃ��B
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
	// nn�m�[�h��, 
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
		// �J�n�n�_�������ݒ肷��B
		node_list[id_begin].add_state(0, to_long_time(0), false, 0, 0);

		// �T������B(�e�m�[�h�̍ŒZ���Ԃ����߂�)
		explore(id_begin);

//		cerr << *this;

		// �����n�_�̏�񂩂瓚��Ԃ��B
		return node_list[id_end].get_min_transit_time();
	}

	friend ostream& operator<<(ostream& os, const time_flies& n);

private:
	vector<edge> edge_list;  // �}�̃��X�g (�O���������A�㔼���ԓ�)
	int n_sidewalk;  // �����̐�
	vector<node> node_list;  // �m�[�h�̃��X�g

	// id����1�J�ڂ����m�[�h�ɑ΂��A��Ԃ�ǉ����A�i�߂�B
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
