//
// Created by Yuhao Wang on 2019/12/11.
//
#include "rsa.hpp"
#include <iostream>
#include <vector>

int main() {
    // 生成密钥对
    RSAKey key = RSAKeyGen(727, 937).generate_key();
    std::cout << key;
    string s = "RSA (Rivest–Shamir–Adleman) is one of the first public-key cryptosystems and is widely used for secure data transmission.";
    std::cout << "origin: " << s << std::endl;

    // 加密
    vector<long> a = encrypt(key.get_pub_key(), s);
    std::cout << "encrypt: ";
    for (long i:a) {
        std::cout << std::hex << i << " ";
    }
    std::cout << std::endl;

    // 解密
    string b = decrypt(key.get_pri_key(), a);
    std::cout << "decrypt: ";
    std::cout << b << std::endl;
}