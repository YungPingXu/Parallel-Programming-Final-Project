#include <mpi.h>
#include <boost/multiprecision/cpp_int.hpp>
#include <iostream>

#include "CycleTimer.h"

using namespace std;
typedef boost::multiprecision::cpp_int bigint;

pair<bigint, bigint> prime_factors(bigint n) {
    if (!(n & 1)) {  // n % 2 == 0
        return {2, n>>1};
    }

    bigint p, q;

    MPI_Init(NULL, NULL);

    int mpi_size;
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

    int mpi_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

    MPI_Win win;

    int sync = 100000000;
    if(mpi_rank == 0){
        int *nfound; // rank n found the solution
        bool is_find = false;
        MPI_Alloc_mem(sizeof(int), MPI_INFO_NULL, &nfound);
        *nfound = -1;
        MPI_Win_create(nfound, sizeof(int), sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &win);

        for (bigint i = 3+(mpi_rank<<1); i * i <= n; i += (mpi_size<<1)){
            if(0 > sync--){
                sync += 100000000;
                MPI_Win_lock(MPI_LOCK_SHARED, 0, 0, win);
                is_find = *nfound > 0;
                MPI_Win_unlock(0, win);
                if(is_find) break;
            }

            if (n % i == 0){
                MPI_Win_lock(MPI_LOCK_EXCLUSIVE, 0, 0, win);
                *nfound = 0;
                MPI_Win_unlock(0, win);
                is_find = true;

                p = i;
                q = n / i;
                break;
            }
        }

        while(!is_find){ // wait until someone find tha answer
            MPI_Win_lock(MPI_LOCK_SHARED, 0, 0, win);
            is_find = *nfound > 0;
            MPI_Win_unlock(0, win);
        }
        if(*nfound > 0){
            int from = *nfound;
            int len;
            MPI_Status status;
            // get string length
            MPI_Recv(&len, 1, MPI_INT, from, 0, MPI_COMM_WORLD, &status);

            // get p
            char *str;
            str = new char[len+1];
            MPI_Recv(str, len, MPI_CHAR, from, 0, MPI_COMM_WORLD, &status);
            str[len] = '\0';
            p = bigint(str);
            delete [] str;
            
            q = n / p;
        }

        MPI_Win_free(&win);
        MPI_Free_mem(nfound);
    } else {
        int nfound = 0;
        MPI_Win_create(NULL, 0, 1, MPI_INFO_NULL, MPI_COMM_WORLD, &win);

        for (bigint i = 3+(mpi_rank<<1); i * i <= n; i += (mpi_size<<1)) {
            if(0 > sync--){
                sync += 100000000;
                MPI_Win_lock(MPI_LOCK_SHARED, 0, 0, win);
                MPI_Get(&nfound, 1, MPI_INT, 0, 0, 1, MPI_INT, win);
                MPI_Win_unlock(0, win);
                if(nfound >= 0) break;
            }


            if (n % i == 0){
                MPI_Win_lock(MPI_LOCK_EXCLUSIVE, 0, 0, win);
                MPI_Put(&mpi_rank, 1, MPI_INT, 0, 0, 1, MPI_INT, win);
                MPI_Win_unlock(0, win);

                string str = i.str();
                int len = str.length();
                // send len & str to rank 0
                MPI_Send(&len, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
                MPI_Send(str.c_str(), len, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
                break;
            }
        }

        MPI_Win_free(&win);
    }

    MPI_Finalize();
    if(mpi_rank > 0) exit(0);
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
