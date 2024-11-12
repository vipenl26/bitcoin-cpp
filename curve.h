//
//  curve.h
//  bitcoin-cpp
//
//  Created by Vipen Loka on 09/11/24.
//

#include <boost/multiprecision/cpp_int.hpp>

using namespace boost::multiprecision;


class Curve {
//    int256_t a;
    
public:
    uint256_t p,a,b;
    
    // by default it is a bitcoin secp256k1 curve
    // y^2 = x^3 + 7
    Curve():p("0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F") {
        a = 0;
        b = 7;
    }
    
    bool operator==(const Curve& other) {
        return p==other.p && a==other.a && b==other.b;
    }
    

};

static Curve bitcoin_curve;


