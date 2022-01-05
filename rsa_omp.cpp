#include <omp.h>

#include <boost/multiprecision/cpp_int.hpp>
#include <iostream>

#include "CycleTimer.h"

#define NUM_THREAD 8

using namespace std;
typedef boost::multiprecision::cpp_int bigint;

pair<bigint, bigint> prime_factors(bigint n) {
    if (!(n & 1))  // n % 2 == 0
        return {2, n >> 1};
    bigint p, q;

    int increase = NUM_THREAD << 1;
    bool is_find = false;
    #pragma omp parallel num_threads(NUM_THREAD)
    {
        int id = omp_get_thread_num();
        for (bigint i = 3 + (id << 1); i * i <= n; i += increase) {
            if (is_find) break;
            if (n % i == 0) {
                is_find = true;
                p = i;
                q = n / i;
                break;
            }
        }
    }
    return {p, q};
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
    bigint n("14282098901120359061");
    bigint e = 65537;

    double start = CycleTimer::currentSeconds();
    pair<bigint, bigint> pq_pairs = prime_factors(n);
    bigint p = pq_pairs.first, q = pq_pairs.second;
    bigint m = (p - 1) * (q - 1);
    bigint d = mod_inverse(e, m);
    double end = CycleTimer::currentSeconds();

    cout << "Public key:" << endl;
    cout << "n = " << n << endl;
    cout << "e = " << e << endl;
    cout << "\nCrack result (private key):" << endl;
    cout << "p = " << p << endl;
    cout << "q = " << q << endl;
    cout << "(p - 1) * (q - 1) = " << m << endl;
    cout << "d = " << d << endl;
    cout << "Cracked in " << end - start << " seconds." << endl;
    return 0;
}
