//
//  publickey.h
//  bitcoin-cpp
//
//  Created by Vipen Loka on 11/11/24.
//

#include "point.cpp"
#include "ripemd160.c"
#include "sha256.h"
#include <boost/multiprecision/cpp_int.hpp>
#include <map>
#include <string>
using namespace boost::multiprecision;
#ifndef PUBLIC_KEY
#define PUBLIC_KEY
class PublicKey : public Point {
  public:
    PublicKey(Point pt) : Point(pt.curve, pt.x, pt.y) {}

    vector<uint8_t> encode(bool compressed, bool hash160 = false) {

        uint8_t prefix;
        vector<uint8_t> pkb;
        if (compressed) {
            if (y % 2 == 0) {
                prefix = 0x02;
            } else {
                prefix = 0x03;
            }
            pkb.push_back(prefix);
            auto xb = to_bytes(x);
            pkb.insert(pkb.end(), xb.begin(), xb.end());

        } else {
            prefix = 0x04;
            auto xb = to_bytes(x);
            auto yb = to_bytes(y);
            pkb.push_back(prefix);
            pkb.insert(pkb.end(), xb.begin(), xb.end());
            pkb.insert(pkb.end(), yb.begin(), yb.end());
        }

        return hash160 ? compute_ripemd160(compute_sha256(pkb))
                       : pkb;
    }

    string address(string net, bool compressed) {
        auto pkb_hash = encode(compressed, true);

        map<string, uint8_t> mp = {{"main", 0x00}, {"test", 0x6f}};

        vector<uint8_t> ver_pkb_hash;
        ver_pkb_hash.push_back(mp[net]);
        ver_pkb_hash.insert(ver_pkb_hash.end(), pkb_hash.begin(),
                            pkb_hash.end());

        auto checksum = compute_sha256(compute_sha256(ver_pkb_hash));

        auto byte_address = ver_pkb_hash;
        byte_address.insert(byte_address.end(), checksum.begin(),
                            checksum.begin() + 4);

        auto b58check_address = b58encode(byte_address);

        return b58check_address;
    }

    string b58encode(vector<uint8_t> &input) {

        string alphabet =
            "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
        assert(input.size() == 25);
        assert(alphabet.size() == 58);
        stringstream ss;

        uint256_t num = from_bytes(input);

        while (num > 0) {
            int idx = static_cast<int>(num % 58);
            ss << alphabet[idx];
            num /= 58;
        }

        // for leading zeros
        string prefix = "";
        for (int i = 0; i < input.size() && input[i] == 0; i++) {
            prefix += '1';
        }

        string res = ss.str();
        reverse(res.begin(), res.end());
        res = prefix + res;
        return res;
    }
};

#endif
