//
// Created by Yuhao Wang on 2019/12/11.
//

#include <iostream>
#include <cmath>
#include <cstring>
#include "mpi_rsa.hpp"

#define FREE(x) if (x != nullptr) {\
        free(x); \
        x = nullptr; \
    }

void rsa_one_process(const char *arr, int len, RSAKey key) {
    std::cout << "process num: 1" << std::endl;
    long *encrypt_code = (long *) malloc(len * sizeof(long));
    char *decrypt_str = (char *) malloc(len);
    encrypt(key.get_pub_key(), arr, encrypt_code, len);
    decrypt(key.get_pri_key(), encrypt_code, decrypt_str, len);
    std::cout << "decrypt_str: " << decrypt_str << std::endl;
    FREE(encrypt_code)
    FREE(decrypt_str)
}

void mpi_rsa_root(MPIArg arg, const char *arr, int len, RSAKey key) {
    std::cout << "process num: " << arg.rank_size << ", ";
    long pub_key[2] = {key.get_pub_key().get_n(), key.get_pub_key().get_e()};
    long pri_key[2] = {key.get_pri_key().get_n(), key.get_pri_key().get_d()};

    MPI_Bcast(pub_key, 2, MPI_LONG, 0, arg.comm);
    MPI_Bcast(pri_key, 2, MPI_LONG, 0, arg.comm);

    int part_len = (int)ceil(len / (double)arg.rank_size);
    std::cout << "part len: " << part_len << std::endl;
    char *send_buf = (char *) malloc(arg.rank_size * part_len);
    char *recv_buf = (char *) malloc(part_len);
    memset(recv_buf, 0, part_len);
    memset(send_buf, 0, arg.rank_size * part_len);
    memcpy(send_buf, arr, len);
    MPI_Bcast(&part_len, 1, MPI_INT, 0, arg.comm);
    MPI_Scatter(send_buf, part_len, MPI_CHAR, recv_buf, part_len, MPI_CHAR, 0, arg.comm);

    long *encrypt_code_part = (long *) malloc(part_len * sizeof(long));
    char *decrypt_str_part = (char *) malloc(part_len);

    long *encrypt_code_all = (long *) malloc(arg.rank_size * part_len * sizeof(long));
    char *decrypt_str_all = (char *) malloc(arg.rank_size * part_len);

    encrypt(key.get_pub_key(), recv_buf, encrypt_code_part, part_len);

    MPI_Gather(encrypt_code_part, part_len, MPI_LONG, encrypt_code_all, part_len, MPI_LONG, 0, arg.comm);

    decrypt(key.get_pri_key(), encrypt_code_part, decrypt_str_part, part_len);

    MPI_Gather(decrypt_str_part, part_len, MPI_CHAR, decrypt_str_all, part_len, MPI_CHAR, 0, arg.comm);

    char *decrypt_str = (char *) malloc(len);

    memcpy(decrypt_str, decrypt_str_all, len);

    std::cout << "decrypt_str: " << decrypt_str << std::endl;

    FREE(send_buf)
    FREE(recv_buf)
    FREE(encrypt_code_part)
    FREE(decrypt_str_part)
    FREE(encrypt_code_all)
    FREE(decrypt_str_all)
    FREE(decrypt_str)
}

void mpi_rsa_rank(MPIArg arg) {
    long pub_key[2];
    long pri_key[2];

    MPI_Bcast(pub_key, 2, MPI_LONG, 0, arg.comm);
    MPI_Bcast(pri_key, 2, MPI_LONG, 0, arg.comm);

    RSAPubKey pub = RSAPubKey(pub_key[0], pub_key[1]);
    RSAPriKey pri = RSAPriKey(pri_key[0], pri_key[1]);

    // std::cout << "Rank: " << arg.rank << " " << pub << std::endl;
    // std::cout << "Rank: " << arg.rank << " " << pri << std::endl;

    int part_len;
    MPI_Bcast(&part_len, 1, MPI_INT, 0, arg.comm);
    char *recv_buf = (char *) malloc(part_len);
    memset(recv_buf, 0, part_len);
    MPI_Scatter(recv_buf, part_len, MPI_CHAR, recv_buf, part_len, MPI_CHAR, 0, arg.comm);
    // std::cout << "Rank: " << arg.rank << ", recv: " << recv_buf << std::endl;

    long *encrypt_code_part = (long *) malloc(part_len * sizeof(long));
    char *decrypt_str_part = (char *) malloc(part_len);

    encrypt(pub, recv_buf, encrypt_code_part, part_len);

    MPI_Gather(encrypt_code_part, part_len, MPI_LONG, encrypt_code_part, part_len, MPI_LONG, 0, arg.comm);

    decrypt(pri, encrypt_code_part, decrypt_str_part, part_len);

    MPI_Gather(decrypt_str_part, part_len, MPI_CHAR, decrypt_str_part, part_len, MPI_CHAR, 0, arg.comm);

    FREE(recv_buf)
    FREE(encrypt_code_part)
    FREE(decrypt_str_part)
}






