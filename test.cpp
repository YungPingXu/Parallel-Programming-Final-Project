#include <omp.h>

#include <boost/multiprecision/cpp_int.hpp>
#include <iostream>
#include <vector>
using namespace std;
using namespace boost::multiprecision;
typedef cpp_int bigint;

// calculate prime divisor for n
bigint PollardRho(bigint n) {
    srand(time(NULL));
    if (n == 1) return n;
    if (!(n & 1)) return 2;             // n % 2 == 0
    bigint x = (rand() % (n - 2)) + 2;  // pick from the range [2, N)
    bigint y = x;
    bigint c = (rand() % (n - 1)) + 1;  // the constant in f(x)
    bigint d = 1;                       // initialize candidate divisor (or result)
    // if n is prime, return n, until the prime factor isn't obtained
    for (;;) {
        x = ((x * x) % n + c + n) % n;  // Tortoise Move: x(i+1) = f(x(i))
        y = ((y * y) % n + c + n) % n;  // Hare Move: y(i+1) = f(f(y(i)))
        y = ((y * y) % n + c + n) % n;
        d = gcd(abs(x - y), n);  // the gcd of |x-y| and n
        if (d > 1 && x != y) break;
    }
    return d;
}

int main() {
    bigint n1{"900559644015697"}, n2{"542050749435487"};
    bigint n = n1 * n2;
    cout << PollardRho(n) << endl;
    return 0;
}