// dom2019d.cpp - ACM ICPC 2019年横浜大会国内予選Problem D 計数カウンタ
//
// @see https://storage.googleapis.com/icpcsec/2019-domestic/ja/contest/all_ja.html#section_D
//
// Copyright 2023 Kitajima Akira
// Author: Kitajima Akira <kitajima@osakac.ac.jp>

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

class Counters {
public:
    // 最大値を指定する。
    // @param m カウンタの最大値
    Counters(int m): value_max(m) {}

    // 新しいカウンタの初期値を設定する。
    // @param a カウンタの初期値
    void add_initial(int a) {
        count_list.push_back(a);
    }

    // 新しいカウンタの目標値を設定する。
    // @param b カウンタの目標値
    void add_final(int b) {
        fin_list.push_back(b);
        diff_list.push_back(0);  // とりあえず0で初期化し、あとでupdate()により更新する。
    }

    // 解を見つけて解く。
    // @return 答
    int solve() {
        int c = 0;  // 必要なカウント回数
        update();
        for (;;) {
            // 0以外の目的の値までの差が一番小さいカウンタを一つ見つける。
            auto p = min_non_zero_element(diff_list);

            int current_d = *p;  // 残りカウント
			print_list("count", count_list);
			print_list("final", fin_list);
			print_list("diff ", diff_list);
			cout << current_d << endl;
            if (current_d <= 0)
                return c;  // すべて目的の値まで到達したので答が求まった。

            // 一つ前のカウンタの残りカウント
            int prev_d = (p == diff_list.begin()) ? 0 : *(p - 1);
            // 同じcurent_dが続く間、読み飛ばす。
            auto next_p = p;
            while (++next_p != diff_list.end() && *next_p == current_d)
                ;
            // 次の残りカウント
            int next_d = (next_p == diff_list.end()) ? 0 : *next_p;
            // 残りカウントの差分
            int d = *p - max(prev_d, next_d);
            // となり合う同じcurrend_dのカウンタすべてにdを加える。
            c += d;
            count(p - diff_list.begin(), next_p - p, d);
			cout << "total" << c << endl;
		}

        return 0;
    }

private:
    int value_max;  // カウンタの最大値
    vector<int> count_list;  // 初期値のリスト
    vector<int> fin_list;  // 目的の値のリスト
    vector<int> diff_list;  // 差の値のリスト

    // 残りカウント数のリスト(diff_list)を更新する。
    // @return 残りカウントの合計値
    int update() {
        int sum = 0;
        for (size_t i = 0; i < count_list.size(); i++) {
            diff_list[i] = fin_list[i] - count_list[i];
            if (diff_list[i] < 0)
                diff_list[i] += value_max;
            sum += diff_list[i];
        }
        return sum;
    }

    // カウンタを進める。
    // @param c_begin 左端のカウンタ位置
    // @param n 同時にカウントするカウンタの個数
    // @param count いくつカウンタを進めるか。
    void count(int c_begin, int n, int count) {
		cout << "count " << c_begin << ", " << n << ", " << count << endl;
		for (int i = c_begin; i < c_begin + n; i++) {
            count_list[i] += count;
            diff_list[i] -= count;
            while (count_list[i] > value_max) {
                count_list[i] -= value_max;
            }
        }
    }

    // 指定した範囲内で解を見つける。
    // @param c_begin 範囲の左端
    // @param n 範囲内のカウンタ数
    // @param upperbound カウントの上限
    // @return カウント数 (上限内に見つからなければ負の値)
    int explore(int c_begin, int n, int upperbound) {
        if (upperbound < 0)
            return -1;
        return 0;
    }

	// リストの内容を表示する。(デバッグ用)
    void print_list(const char* prefix, vector<int>& list) {
        cout << prefix << " [";
        for (int d: list) {
            cout << " " << d;
        }
        cout << " ]\n";
    }

    vector<int>::iterator min_non_zero_element(vector<int>& list) {
        int min = value_max;
        int min_i = -1;
        for (int i = 0; i < list.size(); i++) {
            if (list[i] != 0 && list[i] < min) {
                min = list[i];
                min_i = i;
            }
        }
        return (min_i < 0) ? list.end() : list.begin() + min_i;
    }
};

int main() {
    for (int n, m;  // カウンタの個数、カウンタの最大値 
        cin >> n >> m, (n != 0 || m != 0);) {
        // カウンタの最大値を設定する。
        Counters c(m);

        // 各カウンタの初期値を読み込む。
        for (int i = 0; i < n; i++) {
            int a;  // 初期値
            cin >> a;
            c.add_initial(a);
        }

        // 各カウンタの目標値を読み込む。
        for (int i = 0; i < n; i++) {
            int b;  // 目標値
            cin >> b;
            c.add_final(b);
        }

        // 解を求め、出力する。
        cout << c.solve() << endl;
    }
    return 0;
}
