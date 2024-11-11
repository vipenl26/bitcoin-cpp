//
//  utils.h
//  bitcoin-cpp
//
//  Created by Vipen Loka on 09/11/24.
//

#include <boost/multiprecision/cpp_int.hpp>
#include <tuple>
#include <limits>
#include <boost/beast/core/detail/base64.hpp>
#include <openssl/sha.h>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
using namespace boost::multiprecision;
using namespace std;


#ifndef MY_UTILS
#define MY_UTILS
//static void check_is_mul_overflow(int256_t a, int256_t b) {
//    if (a > std::numeric_limits<int256_t>::max() / b) {
//        cout << "OVERFLOW " << endl;
//    }
//}
static int256_t modmul(int256_t a, int256_t b, int256_t mod) {
    a = a % mod;
    b = b % mod;

    return static_cast<int256_t>(((int512_t)a * b) % mod);
    
}
static int256_t modadd(int256_t a, int256_t b, int256_t mod) {
    a = a % mod;
    b = b % mod;
    int512_t res =  ((int512_t)a + b) % mod;
    
    return static_cast<int256_t>(res);
}
static int256_t pow(int256_t a, int256_t b, int256_t mod) {
    a = a % mod;
    b = b % mod;
    if (a < 0)a+=mod;
    if (b < 0)b+=mod;
    int512_t res = 1, a_hat = a;
    
    while(b > 0) {
        if (b & 1) {
            res = (res * a_hat) % mod;
        }
        a_hat = (a_hat * a_hat) % mod;
        b = b >> 1;
    }
    
    return static_cast<int256_t>(res);
}

//TODO: this is not working figureout why?
static std::tuple<int256_t, int256_t, int256_t> extended_euclidean_algorithm(int256_t a, int256_t b, int256_t mod) {
    
    int256_t old_r = a, r = b;
    int256_t old_s = 1, s = 0;
    int256_t old_t = 0, t = 1;
    
    while(r != 0) {
        int256_t quotient = old_r / r;
        int256_t temp;
        
        temp = old_r;
        old_r = r;
        r = modadd(temp, -modmul(quotient, r, mod), mod);
        
        temp = old_s;
        old_s = s;
        s = modadd(temp, -modmul(quotient, s, mod), mod);

        
        temp = old_t;
        old_t = t;
        t = modadd(temp, -modmul(quotient, t, mod), mod);

        
    }
    
    return std::make_tuple(old_r, old_s, old_t);
}

static int256_t inv(int256_t n, int256_t p) {
    // returns (n * m) % p = 1
//    std::tuple<int256_t, int256_t,int256_t> tup = extended_euclidean_algorithm(n, p, p);
//    return std::get<1>(tup) % p;
    
    return pow(n, p - 2, p);
}
    
    
//std::string sha256(const std::string& input) {
//    unsigned char hash[SHA256_DIGEST_LENGTH];
//    SHA256((unsigned char*)input.c_str(), input.size(), hash);
//
//    std::stringstream ss;
//    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
//        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
//    }
//    return ss.str();
//}


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

