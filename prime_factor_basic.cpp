#include <iostream>
typedef long long int ll;
using namespace std;

void prime_factors(ll num) {
    ll n = num;
    while (n % 2 == 0) {
        cout << "2 ";
        n /= 2;
    }
    for (ll i = 3; i * i <= n; i += 2) {
        while (n % i == 0) {
            cout << i << " ";
            n /= i;
        }
    }
    if (n > 2) cout << n;
    cout << endl;
}

int main() {
    ll n = 515951951364769;
    prime_factors(n);
    return 0;
}