//
// Created by Yuhao Wang on 2019/12/12.
//

#include <ctime>
#include <iostream>
#include <cstring>
#include "mpi_rsa.hpp"

int main(int argc, char *argv[]) {
    MPIArg mpi_arg = MPIArg(&argc, &argv);
    if (mpi_arg.rank == 0) {
        std::string s = "RSA (Rivest–Shamir–Adleman) is one of the first public-key cryptosystems and is widely used for secure data transmission. In such a cryptosystem, the encryption key is public and it is different from the decryption key which is kept secret (private). In RSA, this asymmetry is based on the practical difficulty of the factorization of the product of two large prime numbers, the \"factoring problem\".";
        RSAKey key = RSAKeyGen(727, 937).generate_key(true, 1000l);
        std::cout << key;
        clock_t start, end;
        start = clock();
        if (mpi_arg.rank_size == 1) {
            rsa_one_process(s.c_str(), s.length(), key);
        } else {
            mpi_rsa_root(mpi_arg, s.c_str(), s.length(), key);
        }
        end = clock();
        std::cout << "time: " << end - start << std::endl;
    } else {
        mpi_rsa_rank(mpi_arg);
    }
    MPI_Finalize();
    return 0;
}

