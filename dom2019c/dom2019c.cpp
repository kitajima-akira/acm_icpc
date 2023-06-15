// dom2019c.cpp - ACM ICPC 2019年横浜大会国内予選Problem C 天秤
//
// @see https://storage.googleapis.com/icpcsec/2019-domestic/ja/contest/all_ja.html#section_C
//
// Copyright 2023 Kitajima Akira
// Author: Kitajima Akira <kitajima@osakac.ac.jp>

#include <algorithm>
#include <iostream>
#include <set>
#include <vector>
using namespace std;

// 天秤を解くためのクラス
class BalanceScale {
public:
    /**
     * 計りたい重さ@a a を追加する。
     * @param a 計りたい重さ
     */
    void add_amount(int a) {
        amount_list.push_back(a);
    }

    void add_weight(int w) {
        weight_list.push_back(w);
    }

    int solve() {
        multiset<int> requiredCommon;  // 共通に必要な分銅の重さのリスト

        for (int a: amount_list) {
            // 新たに必要な分銅の重さのリストを求める。
            multiset<int> required = get_additional(a);

            if (requiredCommon.empty() && !required.empty())  // これまでに必要な分銅がなかった場合
                requiredCommon = required;

            else if (!requiredCommon.empty() && !required.empty()) {  // 必要な分銅があって、新たなリストができた場合
                multiset<int> result;
  
                // requiredCommonとrequiredの積集合を作る。
                set_intersection(requiredCommon.begin(), requiredCommon.end(),
                        required.begin(), required.end(),
                        inserter(result, result.end()));
                if (result.empty())  // 共通部分がなくなれば
                    return -1;
 
                // 更新する。
                requiredCommon = result;
            }
        }

        return requiredCommon.empty() ? 0 : *requiredCommon.begin();
    }

private:
    vector<int> amount_list;  // 量りたい重さリスト
    vector<int> weight_list;  // 分銅の重さリスト

    // aを量るのに必要な追加の分銅が一つ以下ですむ分銅の重さのリストを得る。
    multiset<int> get_additional(int a) {
        multiset<int> a_list;
        try {
            a_list = get_additional(a, weight_list);
        } catch (int e) {
        }
        return a_list;
    }

    // aを量るのに必要となる分銅の重さのリストを求める。
    multiset<int> get_additional(int a, vector<int> wl) {
        if (wl.empty())  // 残りの分銅がない場合
            return {(a >= 0) ? a : -a};
        
        // 分銅を一つ選ぶ。
        // パフォーマンスを考慮して、一つ加えてバランスするのがあるか探す。
        if (find(wl.begin(), wl.end(), (a >= 0) ? a : -a) != wl.end())
            throw 0;  // 見つかればOK

        // 末尾(任意でよい)の要素を選ぶ。
        int w = wl.back();
        wl.pop_back();
        
        // 選んだ要素をどうするかで場合分けしてリストをそれぞれ求める。
        multiset<int> case_sameside = get_additional(a + w, wl);  // 同じ皿に置く。
        multiset<int> case_otherside = get_additional(a - w, wl);  // 逆の皿に置く。
        multiset<int> case_dontuse = get_additional(a, wl);  // 置かない。

        // すべてのリストをマージして返す。(C++17ではmultisetでmergeが使える。)
        for (int a: case_dontuse)
            case_otherside.emplace(a);

        for (int a: case_otherside)
            case_sameside.emplace(a);

        return case_sameside;
    }
};

int main() {
    for (int n, m;  // 計量リスト中の薬品量の数とセット中の分銅の個数
        cin >> n >> m && (n > 0 || m > 0);) {
        BalanceScale b;
        for (int i = 0; i < n; i++) {
            int a;
            cin >> a;
            b.add_amount(a);
        }
        for (int i = 0; i < m; i++) {
            int w;
            cin >> w;
            b.add_weight(w);
        }
        cout << b.solve() << endl;
    }
    return 0;
}