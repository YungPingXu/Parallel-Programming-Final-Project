#include <boost/multiprecision/cpp_int.hpp>
#include <iostream>
using namespace std;
using namespace boost::multiprecision;

int main() {
    cpp_int n{"999999999999999999999123"};           // if we don't know how many bits
    int1024_t n1{"999999999999999999999999999999"};  // the biggest int type (1024 bits)
    int1024_t n2{"10000000000000000000000000000"};
    int1024_t add = n1 + n2;
    int1024_t minus = n1 - n2;
    int1024_t multiply = n1 * n2;
    int1024_t divide = n1 / (int1024_t)1111111111111111;
    int1024_t mod = n1 % (int1024_t)12345678901234;
    cout << add << endl;
    cout << minus << endl;
    cout << multiply << endl;
    cout << divide << endl;
    cout << mod << endl;
    return 0;
}