#include <omp.h>
#include <boost/multiprecision/cpp_int.hpp>
#include <iostream>
#include <vector>
using namespace std;
using namespace boost::multiprecision;
typedef cpp_int bigint;

pair<bigint, bigint> prime_factors_general(bigint num) {
    vector<bigint> pq_pairs;
    bigint n = num;
    while (!(n & 1)) {  // n % 2 == 0
        pq_pairs.push_back(2);
        n /= 2;
    }
    for (bigint i = 3; i * i <= n; i += 2) {
        while (n % i == 0) {
            pq_pairs.push_back(i);
            n /= i;
        }
    }
    if (n > 2) pq_pairs.push_back(n);
    return {pq_pairs[0], pq_pairs[1]};
}

pair<bigint, bigint> prime_factors(bigint num) {
    vector<bigint> pq_pairs;
    bigint n = num;
    if (!(n & 1)) {  // n % 2 == 0
        pq_pairs.push_back(2);
        n /= 2;
    }
    #pragma omp parallel for reduction(/:n)
    for (bigint i = 3; i * i <= n; i += 2) {
        if (n % i == 0){
            pq_pairs.push_back(i);
            n /= i;
        }
    }
    return {pq_pairs[0], pq_pairs[1]};
}

bigint mod_inverse(bigint a, bigint m) {
    bigint m0 = m;
    bigint y = 0, x = 1;
    if (m == 1) return 0;
    while (a > 1) {
        bigint q = a / m;  // q is quotient
        bigint t = m;
        m = a % m, a = t;  // m is remainder
        t = y;
        y = x - q * y;  // update y and x
        x = t;
    }
    if (x < 0) x += m0;  // make x positive
    return x;
}

int main() {
    bigint n = 3727977659171364769;
    bigint e = 81116269523;
    pair<bigint, bigint> pq_pairs = prime_factors(n);
    bigint p = pq_pairs.first, q = pq_pairs.second;
    bigint m = (p - 1) * (q - 1);
    bigint d = mod_inverse(e, m);
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