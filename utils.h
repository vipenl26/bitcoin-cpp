//
//  utils.h
//  bitcoin-cpp
//
//  Created by Vipen Loka on 09/11/24.
//

#include <boost/beast/core/detail/base64.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <iomanip>
#include <limits>
#include <openssl/sha.h>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
using namespace boost::multiprecision;
using namespace std;

#ifndef MY_UTILS
#define MY_UTILS
// static void check_is_mul_overflow(uint256_t a, uint256_t b) {
//     if (a > std::numeric_limits<uint256_t>::max() / b) {
//         cout << "OVERFLOW " << endl;
//     }
// }
static uint256_t modmul(uint256_t a, uint256_t b, uint256_t mod) {
    a = a % mod;
    b = b % mod;

    return static_cast<uint256_t>(((uint512_t)a * b) % mod);
}
static uint256_t modadd(uint256_t a, uint256_t b, uint256_t mod) {
    a = a % mod;
    b = b % mod;
    int512_t res = ((int512_t)a + b) % mod;

    return static_cast<uint256_t>(res);
}
static uint256_t modsub(uint256_t a, uint256_t b, uint256_t mod) {
    a = a % mod;
    b = b % mod;
    int512_t res = (a - (int512_t)b) % mod;
    if (res < 0)
        res += mod;

    return static_cast<uint256_t>(res);
}
static uint256_t neg(uint256_t a, uint256_t mod) {
    a = a % mod;
    return static_cast<uint256_t>(mod - (int512_t)a);
}
static uint256_t pow(uint256_t a, uint256_t b, uint256_t mod) {
    a = a % mod;
    b = b % mod;
    if (a < 0)
        a += mod;
    if (b < 0)
        b += mod;
    int512_t res = 1, a_hat = a;

    while (b > 0) {
        if (b & 1) {
            res = (res * a_hat) % mod;
        }
        a_hat = (a_hat * a_hat) % mod;
        b = b >> 1;
    }

    return static_cast<uint256_t>(res);
}

// TODO: this is not working figureout why?
static std::tuple<uint256_t, uint256_t, uint256_t>
extended_euclidean_algorithm(uint256_t a, uint256_t b, uint256_t mod) {

    uint256_t old_r = a, r = b;
    uint256_t old_s = 1, s = 0;
    uint256_t old_t = 0, t = 1;

    while (r != 0) {
        uint256_t quotient = old_r / r;
        uint256_t temp;

        temp = old_r;
        old_r = r;
        r = modsub(temp, modmul(quotient, r, mod), mod);

        temp = old_s;
        old_s = s;
        s = modsub(temp, modmul(quotient, s, mod), mod);

        temp = old_t;
        old_t = t;
        t = modsub(temp, modmul(quotient, t, mod), mod);
    }

    return std::make_tuple(old_r, old_s, old_t);
}

static uint256_t inv(uint256_t n, uint256_t p) {
    // returns (n * m) % p = 1
    //    std::tuple<uint256_t, uint256_t,uint256_t> tup =
    //    extended_euclidean_algorithm(n, p, p); return std::get<1>(tup) % p;

    return pow(
        n, modsub(p, (uint256_t)2, std::numeric_limits<uint256_t>::max()), p);
}

// std::string sha256(const std::string& input) {
//     unsigned char hash[SHA256_DIGEST_LENGTH];
//     SHA256((unsigned char*)input.c_str(), input.size(), hash);
//
//     std::stringstream ss;
//     for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
//         ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
//     }
//     return ss.str();
// }

static vector<uint8_t> string_to_bytes(string &input) {
    vector<uint8_t> res(input.size());
    std::memcpy(res.data(), input.data(), input.size());

    return res;
}

static string bytes_to_string(vector<uint8_t> &input) {
    string s(input.size(), 'x');
    std::memcpy(s.data(), input.data(), input.size());

    return s;
}

static string bytes_to_hex(vector<uint8_t> input) {

    stringstream ss;
    for (int i = 0; i < input.size(); i++) {
        ss << std::hex << std::setfill('0') << std::setw(2) << (int)input[i];
    }

    return ss.str();
}

#endif
