#include <boost/multiprecision/cpp_int.hpp>
#include <iostream>
using namespace std;
using namespace boost::multiprecision;

void prime_factors(cpp_int num) {
    cpp_int n = num;
    while (n % 2 == 0) {
        cout << "2 ";
        n /= 2;
    }
    for (cpp_int i = 3; i * i <= n; i += 2) {
        while (n % i == 0) {
            cout << i << " ";
            n /= i;
        }
    }
    if (n > 2) cout << n;
    cout << endl;
}

int main() {
    cpp_int n1{"567629137"}, n2{"6567629137"};
    cpp_int n = n1 * n2;
    cout << n << endl;
    // prime_factors(n + 1);
    // prime_factors(2 * n + 1);
    return 0;
}