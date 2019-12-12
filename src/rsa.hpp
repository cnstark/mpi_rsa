//
// Created by Yuhao Wang on 2019/12/10.
//

#ifndef PARALEL_RSA_MPI_MASTER_RSA_HPP
#define PARALEL_RSA_MPI_MASTER_RSA_HPP

#include <string>
#include <vector>
#include <iostream>

using std::string;
using std::vector;

/**
 * rsa公钥
 */
class RSAPubKey {
private:
    long n;
    long e;
public:
    RSAPubKey(long n, long e) : n(n), e(e) {};

    long get_n() const {
        return n;
    }

    long get_e() const {
        return e;
    }

    friend std::ostream &operator<<(std::ostream &os, const RSAPubKey &k) {
        os << "pri key: (" << k.get_n() << ", " << k.get_e() << ")";
        return os;
    }
};

/**
 * rsa私钥
 */
struct RSAPriKey {
private:
    long n;
    long d;
public:
    RSAPriKey(long n, long d) : n(n), d(d) {};

    long get_n() const {
        return n;
    };

    long get_d() const {
        return d;
    };

    friend std::ostream &operator<<(std::ostream &os, const RSAPriKey &k) {
        os << "pub key: (" << k.get_n() << ", " << k.get_d() << ")";
        return os;
    }
};

/**
 * rsa密钥，包含一对公钥和私钥
 */
class RSAKey {
private:
    RSAPubKey pub_key;
    RSAPriKey pri_key;
public:
    RSAKey(RSAPubKey pub, RSAPriKey pri) : pub_key(pub), pri_key(pri) {};

    const RSAPubKey &get_pub_key() const {
        return pub_key;
    }

    const RSAPriKey &get_pri_key() const {
        return pri_key;
    }

    friend std::ostream &operator<<(std::ostream &os, const RSAKey &key) {
        os << "==========rsa key: ==========" << std::endl;
        os << "pub_key: " << key.pub_key << std::endl;
        os << "pri_key: " << key.pri_key << std::endl;
        os << "=============================" << std::endl;
        return os;
    }
};

/**
 * RSA密钥生成器
 * 使用两个质数p, q生成一对密钥
 */
class RSAKeyGen {
private:
    long n = 0, m = 0, e = 0, d = 0;

    /**
     * 欧几里得算法，寻找两个数最大公因数
     * 若两个数最大公因数为1，则两个数互质
     */
    static long gcd(long a, long b);

    /**
     * 扩展欧几里得算法
     * (e * d) % m = 1，求d
     */
    static long ex_gcd(long a, long b, long &x, long &y);

    /**
     * 判断一个数是否为质数
     */
    static bool is_prime(long n);

    /**
     * 生成公钥(e为随机值)
     */
    RSAPubKey generate_pub_key();

    /**
     * 生成私钥
     */
    RSAPriKey generate_pri_key();

public:
    RSAKeyGen(long p, long q);

    /**
     * 生成一对密钥
     */
    RSAKey generate_key();
};

/**
 * 计算幂模，即a ^ e % n
 */
long pow_mod(long a, long e, long n);

/**
 * 加密数字
 */
long encrypt(RSAPubKey key, long value);

/**
 * 解密数字
 */
long decrypt(RSAPriKey key, long code);

/**
 * 加密字符串（结果保存于vector）
 */
vector<long> encrypt(RSAPubKey key, const string &value);

/**
 * 解密字符串（从vector）
 */
string decrypt(RSAPriKey key, const vector<long> &code);

/**
 * 加密字符串（char数组，结果保存于long数组）
 */
void encrypt(RSAPubKey key, char *value, long *code, long len);

/**
 * 解密字符串（从long数组，结果保存于char数组）
 */
void decrypt(RSAPriKey key, long *code, char *value, long len);

#endif //PARALEL_RSA_MPI_MASTER_RSA_HPP
