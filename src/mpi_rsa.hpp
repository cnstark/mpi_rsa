//
// Created by Yuhao Wang on 2019/12/12.
//

#ifndef _MPI_RSA_HPP_
#define _MPI_RSA_HPP_

#include "rsa.hpp"
#include "mpi_arg.hpp"

void rsa_one_process(const char *arr, int len, RSAKey key);

void mpi_rsa_root(MPIArg arg, const char *arr, int len, RSAKey key);

void mpi_rsa_rank(MPIArg arg);

#endif
