// dom2017a.cpp
// Copyright 2017 Akira Kitajima
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class choice {
 public:
	void add(int a) {
		list.push_back(a);
	}

	bool explore(int limit) {
		result = -1;
		sort(list.begin(), list.end());
		for (auto p = list.begin(); p != list.end(); ++p)
			for (auto q = p + 1; q != list.end(); ++q) {
				int r = *p + *q;
				if (r > limit)
					break;
				if (result < r)
					result = r;
			}
		return result >= 0;
	}

	int total() {
		return result;
	}

 private:
	vector<int> list;
	int result;
};

int main() {
	for (int n, m; cin >> n >> m, n != 0 || m != 0; ) {
		choice c;
		for (int i = 0; i < n; i++) {
			int a;
			cin >> a;
			c.add(a);
		}
		if (c.explore(m))
			cout << c.total() << endl;
		else
			cout << "NONE" << endl;
	}
	return 0;
}
