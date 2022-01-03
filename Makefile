all: rsa_serial rsa_thread rsa_omp rsa_mpi PollardRho_serial

rsa_serial: rsa_serial.cpp
	g++ rsa_serial.cpp -std=c++17 -I"." -O3 -o rsa_serial

rsa_thread: rsa_thread.cpp
	g++ rsa_thread.cpp -std=c++17 -I"." -O3 -o rsa_thread -pthread

rsa_omp: rsa_omp.cpp
	g++ rsa_omp.cpp -std=c++17 -I"." -O3 -o rsa_omp -fopenmp

rsa_mpi: rsa_mpi.cpp
	mpicxx rsa_mpi.cpp -std=c++17 -I"." -O3 -o rsa_mpi

PollardRho_serial: PollardRho_serial.cpp
	g++ PollardRho_serial.cpp -std=c++17 -I"." -O3 -o PollardRho_serial
