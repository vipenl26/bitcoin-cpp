//
//  main.cpp
//  bitcoin-cpp
//
//  Created by Vipen Loka on 09/11/24.
//

#include <iostream>
#include "publickey.h"
using namespace std;



#include "config.h"


#ifndef TESTING_THIS
int main(int argc, const char * argv[]) {
    

    cout << "this is main" << endl;
    
    uint256_t x("2020");
    cout << PublicKey::to_bytes(x, 8) << endl;
    
    
    
    
    return 0;
}
#endif
