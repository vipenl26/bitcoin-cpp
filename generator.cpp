//
//  generator.cpp
//  bitcoin-cpp
//
//  Created by Vipen Loka on 09/11/24.
//
#include <boost/multiprecision/cpp_int.hpp>
#include "point.cpp"
using namespace boost::multiprecision;

class Generator {
public:
    Point G;
    uint256_t n;
    
    // need to figure out/derive who to find the order
    // 0 * G == n * G = INF
    // 'n' is the order
    Generator():n("0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141") {
        
    }
    
};
