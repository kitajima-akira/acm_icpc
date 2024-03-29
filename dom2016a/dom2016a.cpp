// dom2016a.cpp - ACM ICPC 2016Nเ\IA ํฑาฬI่
// https://icpc.iisf.or.jp/past-icpc/domestic2016/problems/all_ja.html#section_A
// Copyright 2023 Kitajima Akira
// Author: Kitajima Akira <kitajima@osakac.ac.jp>

#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

// z๑gradesฬทฬลฌl๐฿้B
int get_min_diff(const vector<int>& a) {
	auto copied = a;
	// ภืึฆ้B
	sort(copied.begin(), copied.end());

	// ืฏmฬท๐฿้B
	vector<int> diffs(copied.size());
	adjacent_difference(copied.begin(), copied.end(), diffs.begin());

	// ทฬลฌl๐฿้B
	return *min_element(diffs.begin() + 1, diffs.end());
}

int main() {
	for (int n; cin >> n && n > 0; ) {
		vector<int> grades(n);
		for (auto& a : grades)
			cin >> a;

		cout << get_min_diff(grades) << endl;
	}
	return 0;
}
