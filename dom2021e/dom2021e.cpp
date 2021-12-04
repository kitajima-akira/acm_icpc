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

// ��ԉ�k��̂Ƃ��A���҂����Ԃ� 2^(k + 1) - 1
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
		// ���̏����ł͑҂����Ԃ������Ă��ۂ߂Ă͂����Ȃ��B
		return et1 + total_wait_time(nt1) < et2 + total_wait_time(nt2);

	if (nt1 >= wait_threshold && nt2 < wait_threshold)
		return false;

	if (nt1 < wait_threshold && nt2 >= wait_threshold)
		return true;

	// if (nt1 >= wait_threshold && nt2 >= wait_threshold && nt1 != nt2) 

	return nt1 < nt2;
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

// <����, ��Ԓ���, �O�̃m�[�h>
typedef tuple<taxi_time, bool, int> state;

// state�o�͗p�֐� (�f�o�b�O�p)
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
		// ���̃m�[�h����s���邷�ׂĂ̎}�ɂ��āA
		// ������ʂ����Ƃ��̏�Ԃ�o�^����B
		for (auto& ep : edge_list) {
			const int opposite_id = ep->get_opposite_node(id);
			for (const auto& s : state_list) {
				const auto& [tt, o, i] = s;
				if (opposite_id == i)  // ��x�s���Ė߂�
					continue;  // �o�^����K�v�Ȃ�
				const auto& [n, et] = tt;
				int n_taxi = n;
				auto new_et = et + ep->get_transit_time();
				bool onboard = o;

				if (ep->get_type() == edge::road_type::SIDEWALK) {  // ����
					onboard = false;
				}
				else {  // �ԓ�
					if (!onboard) {
						// �V������Ԃ��J�n����B
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
		// ����܂łɓo�^���ꂽ��ԂƔ�ׂĖ��炩�ɒx���Ȃ�ꍇ�͓o�^���Ȃ��B
		if (!is_new_state(total_time, onboard))
			return false;

		// ���łɓo�^����Ă����Ԃ�����̂�舫���ꍇ�͍폜����B
		for (auto it = state_list.begin(); it != state_list.end(); ) {
			const auto& [tt, o, i] = *it;
			if (o == onboard 
				&& get<0>(tt) >= get<0>(total_time)
				&& !less_than(tt, total_time)) {  // onboard�����ʂŁA�񐔁E���Ԃ����ɏ������B
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
		state_list.push_back({ total_time, onboard, id });
		return true;
	}

	long long int get_min_transit_time() {
		if (state_list.empty())
			return -1;

		// state_list�ɓo�^����Ă���total_time�̍ŏ��l�𓾂�B
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
	// �O���t�̂��߂̕ϐ�
	int id;
	vector<edge*> edge_list;

	// �T���p�f�[�^
	vector<state> state_list;

	// ����܂łɓo�^���ꂽ��ԂƔ�ׂĖ��炩�ɒx���Ȃ�Ȃ���?
	bool is_new_state(taxi_time total_time, bool onboard) {
		for (const auto& s : state_list) {
			const auto& [tt, o, i] = s;
			if (o == onboard 
				&& get<0>(total_time) >= get<0>(tt) 
				&& !less_than(total_time, tt))  // onboard�����ʂŁA�񐔁E���Ԃ����ɑ傫���B
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
		node_list[id_begin].add_state({ 0, 0 }, false, 0);

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
