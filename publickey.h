//
//  publickey.h
//  bitcoin-cpp
//
//  Created by Vipen Loka on 11/11/24.
//


#include "point.cpp"
#include <string>
#include <boost/multiprecision/cpp_int.hpp>
#include "ripemd160.c"
#include "sha256.h"
#include <map>
using namespace boost::multiprecision;
class PublicKey:public Point{
public:
    
    PublicKey(Point pt):Point(pt.curve, pt.x, pt.y) {
        
    }
    
    static vector<uint8_t> to_bytes(uint256_t input) {
        vector<uint8_t> res(32);
        for (int i = 31;i >= 0;i--) {
            res[i] = static_cast<uint8_t>(input % 256);
            input %= 256;
        }
        return res;
    }
    
//    static vector<uint8_t> from_bytes(vector<uint8_t> input) {
//        uint256_t
//        for (int i = 31;i >= 0;i--) {
//            res[i] = static_cast<uint8_t>(input % 256);
//            input %= 256;
//        }
//        return res;
//    }
    
    
    vector<uint8_t> encode(bool compressed, bool hash160=false) {
        polish();
        
        uint8_t prefix;
        vector<uint8_t> pkb;
        if (compressed) {
            if (y % 2 == 0){
                prefix = '\x02';
            }
            else {
                prefix = '\x03';
            }
            pkb.push_back(prefix);
            auto xb = to_bytes(x);
            pkb.insert(pkb.end(), xb.begin(), xb.end());
            
        }
        else {
            prefix = 4;
            auto xb = to_bytes(x);
            auto yb = to_bytes(y);
            pkb.insert(pkb.end(), xb.begin(), xb.end());
            pkb.insert(pkb.end(), yb.begin(), yb.end());
        }
        
        
        return hash160 ? compute_ripemd160(compute_sha256(pkb)) : compute_sha256(pkb);
    }
    
    vector<uint8_t> address(string net, bool compressed) {
        auto pkb_hash = encode(compressed, true);
        
        map<string, uint8_t> mp = {{"main", '\x00'}, {"test", '\x6f'}};
        
        
        vector<uint8_t> ver_pkb_hash;
        ver_pkb_hash.push_back(mp["net"]);
        ver_pkb_hash.insert(ver_pkb_hash.end(), pkb_hash.begin(), pkb_hash.end());
        
        
        auto checksum = compute_sha256(compute_sha256(ver_pkb_hash));
        
        
        auto byte_address = ver_pkb_hash;
        byte_address.insert(byte_address.end(), checksum.begin(), checksum.begin()+4);
        
        
        
        
        return pkb_hash;
    }
    
    
    string b58encode(vector<uint8_t> &input) {
        
        string alphabet = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
        assert(input.size() == 25);
        stringstream ss;
        
        return "";
        
        
    }
    
};
