#include <iostream>
#include <vector>
typedef long long int ll;
using namespace std;

pair<ll, ll> prime_factors(ll num) {
    vector<ll> pq_pairs;
    ll n = num;
    while (n % 2 == 0) {
        pq_pairs.push_back(2);
        n /= 2;
    }
    for (ll i = 3; i * i <= n; i += 2) {
        while (n % i == 0) {
            pq_pairs.push_back(i);
            n /= i;
        }
    }
    if (n > 2) pq_pairs.push_back(n);
    return {pq_pairs[0], pq_pairs[1]};
}

ll mod_inverse(ll a, ll m) {
    ll m0 = m;
    ll y = 0, x = 1;
    if (m == 1) return 0;
    while (a > 1) {
        ll q = a / m;  // q is quotient
        ll t = m;
        m = a % m, a = t;  // m is remainder
        t = y;
        y = x - q * y;  // update y and x
        x = t;
    }
    if (x < 0) x += m0;  // make x positive
    return x;
}

int main() {
    ll n = 515951951364769;
    ll e = 35683;
    pair<ll, ll> pq_pairs = prime_factors(n);
    ll p = pq_pairs.first, q = pq_pairs.second;
    ll m = (p - 1) * (q - 1);
    ll d = mod_inverse(e, m);
    cout << "Public key:" << endl;
    cout << "n = " << n << endl;
    cout << "e = " << e << endl;
    cout << "\nCrack result (private key):" << endl;
    cout << "p = " << p << endl;
    cout << "q = " << q << endl;
    cout << "(p - 1) * (q - 1) = " << m << endl;
    cout << "d = " << d << endl;
    return 0;
}