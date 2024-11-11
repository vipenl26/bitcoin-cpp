//
//  publickey.h
//  bitcoin-cpp
//
//  Created by Vipen Loka on 11/11/24.
//


#include "point.cpp"
#include <string>
#include <boost/multiprecision/cpp_int.hpp>
using namespace boost::multiprecision;
class PublicKey:public Point{
public:
    
    PublicKey(Point pt):Point(pt.curve, pt.x, pt.y) {
        
    }
    
    static std::string to_bytes(int256_t value, int sz) {
        uint8_t byteArray[sz];

        // Copy the bytes from the int256_t value into the vector
        std::memcpy(byteArray, &value, sizeof(int256_t));

        stringstream ss;
        
        for(int i = sz - 1; i >= 0; i--) {
            ss << std::dec << "\\x";
            ss  << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byteArray[i]);
        }
        return ss.str();
    }
    
    
    void encode(bool compressed, bool hash160=false) {
        polish();
        
        string prefix, pkb;
        if (compressed) {
            if (y % 2 == 0){
                prefix = "\x02";
            }
            else {
                prefix = "\x03";
            }
//            pkb = prefix +
        }
    }
    
};
