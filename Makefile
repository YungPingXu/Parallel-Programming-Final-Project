all: rsa_serial rsa_thread rsa_omp rsa_mpi

rsa_serial:
	g++ rsa_serial.cpp -std=c++17 -I"." -O3 -o rsa_serial

rsa_thread:
	g++ rsa_thread.cpp -std=c++17 -I"." -O3 -o rsa_thread -pthread

rsa_omp:
	g++ rsa_omp.cpp -std=c++17 -I"." -O3 -o rsa_omp -fopenmp

rsa_mpi:
	mpicxx rsa_mpi.cpp -std=c++17 -I"." -O3 -o rsa_mpi
