//
// Created by Yuhao Wang on 2019/12/10.
//

#include <cassert>
#include <iostream>
#include <cstring>
#include <ctime>
#include "rsa.hpp"

using std::cout;
using std::endl;
using std::string;
using std::vector;

RSAKeyGen::RSAKeyGen(long p, long q) {
    assert(is_prime(p));
    assert(is_prime(q));
    n = p * q;
    m = (p - 1) * (q - 1);
}

/**
 * 欧几里得算法，寻找两个数最大公因数
 * 若两个数最大公因数为1，则两个数互质
 */
long RSAKeyGen::gcd(long a, long b) {
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

/**
 * 扩展欧几里得算法
 * (e * d) % m = 1，求d
 */
long RSAKeyGen::ex_gcd(long a, long b, long &x, long &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    long r = ex_gcd(b, a % b, x, y);
    long temp = y;
    y = x - (a / b) * y;
    x = temp;
    return r;
}

/**
 * 判断一个数是否为质数
 */
bool RSAKeyGen::is_prime(long n) {
    if (n == 1)
        return false;
    if (n == 2)
        return true;
    for (long i = 2; i * i <= n; i++) {
        if (n % i == 0)
            return false;
    }
    return true;
}

/**
 * 生成公钥(e为随机值)
 */
RSAPubKey RSAKeyGen::generate_pub_key() {
    e = m - 1;
    srand(time(nullptr));
    // e从一个小于m/2的随机数开始递增，直到满足与m互质
    for (long i = rand() % (long) (m / 2); i < m - 1; i++) {
        if (gcd(m, i) == 1) {
            e = i;
            break;
        }
    }
    return {n, e};
}

/**
 * 生成私钥
 */
RSAPriKey RSAKeyGen::generate_pri_key() {
    long y;
    ex_gcd(e, m, d, y);
    if (d < 0) {
        d = d + m;
    }
    return {n, d};
}

/**
 * 生成一对密钥
 */
RSAKey RSAKeyGen::generate_key() {
    RSAPubKey pub = generate_pub_key();
    RSAPriKey pri = generate_pri_key();
    return {pub, pri};
}

/**
 * 计算幂模，即a ^ e % n
 */
long pow_mod(long a, long e, long n) {
    long x = 1;
    for (long i = 0; i < e; i++) {
        x = (x * a) % n;
    }
    return x;
}

long encrypt(RSAPubKey key, long value) {
    return pow_mod(value, key.get_e(), key.get_n());
}

long decrypt(RSAPriKey key, long value) {
    return pow_mod(value, key.get_d(), key.get_n());
}

vector<long> encrypt(RSAPubKey key, const string &value) {
    vector<long> code = vector<long>();
    for (char i : value) {
        code.push_back(encrypt(key, i));
    }
    return code;
}

string decrypt(RSAPriKey key, const vector<long> &code) {
    char *c_arr = (char *) malloc(code.size() + 1);
    memset(c_arr, '\0', code.size() + 1);
    char *j = c_arr;
    for (long i:code) {
        *(j++) = (char) decrypt(key, i);
    }
    return string(c_arr);
}

void encrypt(RSAPubKey key, char *value, long *code, long len) {
    for (int i = 0; i < len; i++) {
        code[i] = encrypt(key, value[i]);
    }
}

void decrypt(RSAPriKey key, long *code, char *value, long len) {
    for (int i = 0; i < len; i++) {
        value[i] = (char) decrypt(key, code[i]);
    }
}
