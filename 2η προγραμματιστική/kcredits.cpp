#include <iostream>  
#include <vector>  
#include <algorithm>  
using namespace std;  
using ll = long long;  
const int INF = 2 * (int)1e9;  
  
//i used a code from this site
//https://cs.stackexchange.com/questions/118858/how-can-ideas-like-lagrange-multipliers-and-penalty-method-be-applied-for-solvin/118859?fbclid=IwAR1dywwVHdiWk2-PBWEc3nJ9d1dahkvYNaQMaWsn0mOh6bbCL52MiIYjH6s#118859
//as ot does exactly what we want
pair<ll, pair<int, int>> combine(pair<ll, pair<int, int>> le, pair<ll, pair<int, int>> ri) {  
    if (le.first < ri.first) swap(le, ri);  
    if (ri.first == le.first) {  
        le.second.first = min(le.second.first, ri.second.first);  
        le.second.second = max(le.second.second, ri.second.second);  
    }  
    return le;  
}  
  
// Specialised range maximum segment tree  
class SegTree {  
    private:  
        vector<pair<ll, pair<int, int>>> seg;  
        int h = 1;  
  
        pair<ll, pair<int, int>> recGet(int a, int b, int i, int le, int ri) const {  
            if (ri <= a || b <= le) return {-INF, {INF, -INF}};  
            else if (a <= le && ri <= b) return seg[i];  
            else return combine(recGet(a, b, 2*i, le, (le+ri)/2), recGet(a, b, 2*i+1, (le+ri)/2, ri));  
        }  
    public:  
        SegTree(int n) {  
            while(h < n) h *= 2;  
            seg.resize(2*h, {-INF, {INF, -INF}});  
        }  
        void set(int i, pair<ll, pair<int, int>> off) {  
            seg[i+h] = combine(seg[i+h], off);  
            for (i += h; i > 1; i /= 2) seg[i/2] = combine(seg[i], seg[i^1]);  
        }  
        pair<ll, pair<int, int>> get(int a, int b) const {  
            return recGet(a, b+1, 1, 0, h);  
        }  
};  
  
// Binary searches index of v from sorted vector  
int bins(const vector<int>& vec, int v) {  
    int low = 0;  
    int high = (int)vec.size() - 1;  
    while(low != high) {  
        int mid = (low + high) / 2;  
        if (vec[mid] < v) low = mid + 1;  
        else high = mid;  
    }  
    return low;  
}  
  
// Finds longest strictly increasing subsequence with at most k exceptions in O(n log^2 n)  
vector<int> lisExc(int k, vector<int> vec) {  
    // Compress values  
    vector<int> ord = vec;  
    sort(ord.begin(), ord.end());  
    ord.erase(unique(ord.begin(), ord.end()), ord.end());  
    for (auto& v : vec) v = bins(ord, v) + 1;  
  
    // Binary search lambda  
    int n = vec.size();  
    int m = ord.size() + 1;  
    int lambda_0 = 0;  
    int lambda_1 = n;  
    while(true) {  
        int lambda = (lambda_0 + lambda_1) / 2;  
        SegTree seg(m);  
        if (lambda > 0) seg.set(0, {0, {0, 0}});  
        else seg.set(0, {0, {0, INF}});  
  
        // Calculate DP  
        vector<pair<ll, pair<int, int>>> dp(n);  
        for (int i = 0; i < n; ++i) {  
            auto off0 = seg.get(0, vec[i]-1); // previous < this  
            off0.first += 1;  
  
            auto off1 = seg.get(vec[i], m-1); // previous >= this  
            off1.first += 1 - lambda;  
            off1.second.first += 1;  
            off1.second.second += 1;  
  
            dp[i] = combine(off0, off1);  
            seg.set(vec[i], dp[i]);  
        }  
  
        // Is min_b <= k <= max_b?  
        auto off = seg.get(0, m-1);  
        if (off.second.second < k) {  
            lambda_1 = lambda - 1;  
        } else if (off.second.first > k) {  
            lambda_0 = lambda + 1;  
        } else {  
            // Construct solution  
            ll r = off.first + 1;  
            int v = m;  
            int b = k;  
            vector<int> res;  
            for (int i = n-1; i >= 0; --i) {  
                if (vec[i] < v) {  
                    if (r == dp[i].first + 1 && dp[i].second.first <= b && b <= dp[i].second.second) {  
                        res.push_back(i);  
                        r -= 1;  
                        v = vec[i];  
                    }  
                } else {  
                    if (r == dp[i].first + 1 - lambda && dp[i].second.first <= b-1 && b-1 <= dp[i].second.second) {  
                        res.push_back(i);  
                        r -= 1 - lambda;  
                        v = vec[i];  
                        --b;  
                    }  
                }  
            }  
            reverse(res.begin(), res.end());  
            return res;  
        }  
    }  
}  
  
int main() {  
    int n, k;  
    cin >> n >> k;  
  
    vector<int> vec(n);  
    for (int i = 0; i < n; ++i) cin >> vec[i];  
  
    cout<<lisExc(k, vec).size()<<'\n';
}    