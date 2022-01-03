#include <algorithm>
#include <boost/multiprecision/cpp_int.hpp>
#include <iostream>

#include "CycleTimer.h"

using namespace std;
typedef boost::multiprecision::cpp_int bigint;

/* method to return prime divisor for n */
pair<bigint, bigint> PollardRho(bigint n) {
    /* initialize random seed */
    srand(time(NULL));

    /* no prime divisor for 1 */
    if (n == 1) return {0, 0};

    /* even number means one of the divisors is 2 */
    if (n % 2 == 0) return {2, n >> 1};

    /* we will pick from the range [2, N) */
    bigint x = (rand() % (n - 2)) + 2, y = x;

    /* the constant in f(x).
     * Algorithm can be re-run with a different c
     * if it throws failure for a composite. */
    bigint c = (rand() % (n - 1)) + 1;

    /* Initialize candidate divisor (or result) */
    bigint d = 1;

    /* until the prime factor isn't obtained.
       If n is prime, return n */
    while (d == 1) {
        /* Tortoise Move: x(i+1) = f(x(i)) */
        x = (x * x % n + c + n) % n;

        /* Hare Move: y(i+1) = f(f(y(i))) */
        y = (y * y % n + c + n) % n;
        y = (y * y % n + c + n) % n;

        /* check gcd of |x-y| and n */
        d = gcd(abs(x - y), n);

        /* retry if the algorithm fails to find prime factor
         * with chosen x and c */
        if (d == n) return PollardRho(n);
    }

    return {d, n / d};
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

/* driver function */
int main() {
    bigint n = 3727977659171364769;
    bigint e = 81116269523;

    double start = CycleTimer::currentSeconds();
    pair<bigint, bigint> pq_pairs = PollardRho(n);
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