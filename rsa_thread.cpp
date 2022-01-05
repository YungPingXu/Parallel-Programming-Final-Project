#include <boost/multiprecision/cpp_int.hpp>
#include <iostream>
#include <pthread.h>

#include "CycleTimer.h"

#define NUM_THREAD 4

using namespace std;
typedef boost::multiprecision::cpp_int bigint;

struct Arg{
    bigint* n;
    int s;
    bigint* p;
    bigint* q;
};

bool is_find = false;

void* try_devision(void* para){
    Arg* arg = (Arg*) para;
    bigint n = *(arg->n);
    
    for(bigint i=arg->s; i*i<= n; i+=(NUM_THREAD<<1)){
        if(is_find) return NULL;
        if(n % i == 0){
            is_find = true;
            *(arg->p) = i;
            *(arg->q) = n / i;
            return NULL;
        }
    }

    return NULL;
}

pair<bigint, bigint> prime_factors(bigint n) {
    if (!(n & 1)) {  // n % 2 == 0
        return {2, n>>1};
    }

    pthread_t thread[NUM_THREAD];
	Arg args[NUM_THREAD];
    bigint p, q;
    is_find = false;

	for(int i=0; i<NUM_THREAD; i++){
		args[i].n = &n;
		args[i].s = 3+(i<<1);
		args[i].p = &p;
		args[i].q = &q;
		pthread_create(&thread[i], NULL, try_devision, (void*)&args[i]);
	}
	for(int i=0; i<NUM_THREAD; i++){
		pthread_join(thread[i], NULL);
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
