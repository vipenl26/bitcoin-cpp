//
//  utest.cpp
//  bitcoin-cpp
//
//  Created by Vipen Loka on 09/11/24.
//

// test_math_utils.cpp

#include <iostream>
#include "utils.h"  // Include the header file of the function to test
#include "config.h"
#include "point.cpp"
#include "sha256.h"
#include "ripemd160.c"

#ifdef TESTING_THIS
#define BOOST_TEST_MODULE MathUtilsTest  // Defines the test module name
#include <boost/test/included/unit_test.hpp>
BOOST_AUTO_TEST_CASE( inv_test ) {
    BOOST_CHECK(inv((uint256_t)17, (uint256_t)43) == (uint256_t)38);
}


BOOST_AUTO_TEST_CASE( point_addition_test ) {
    uint256_t sk;
    Point pk;
    
    sk = 1;
    pk = G;
    BOOST_CHECK(pk.check_point_on_curve());
    
    
    
    sk = 2;
    pk = G + G;
    BOOST_CHECK(pk.check_point_on_curve());
    
    
    sk = 3;
    pk = G + G + G;
    BOOST_CHECK(pk.check_point_on_curve());
//
}



BOOST_AUTO_TEST_CASE( point_multiplication_test ) {
    BOOST_CHECK(G == (G * 1));
    
    
    BOOST_CHECK(G + G == (G * 2));
    BOOST_CHECK(G + G + G == (G * 3));
//    
    BOOST_CHECK(G + G + G + G == (G + G) + (G + G));
    BOOST_CHECK(G + G + G + G == (G * 4));
    

    
    BOOST_CHECK(G + G + G + G + G == (G * 5));
}

BOOST_AUTO_TEST_CASE(generate_public_key) {
    uint256_t secret_key("22265090479312778178772228083027296664144");
    
    Point public_key = G * secret_key;
    
    uint256_t expected_x("83998262154709529558614902604110599582969848537757180553516367057821848015989"),
    expected_y("37676469766173670826348691885774454391218658108212372128812329274086400588247");
    public_key.polish();
    
    BOOST_CHECK(public_key.x == expected_x);
    BOOST_CHECK(public_key.y == expected_y);
    BOOST_CHECK(public_key.check_point_on_curve());
    
}

BOOST_AUTO_TEST_CASE(sha256_test) {
    
    //empty string to hash
    BOOST_CHECK(compute_sha256("")=="e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
    
    BOOST_CHECK(compute_sha256("here is a random bytes message, cool right?")=="69b9779edaa573a509999cbae415d3408c30544bad09727a1d64eff353c95b89");
    
}

BOOST_AUTO_TEST_CASE(ripemd160_test) {
    
    //empty string to hash
    BOOST_CHECK(compute_ripemd160("")=="9c1185a5c5e9fc54612808977ee8f548b2258d31");
    
    BOOST_CHECK(compute_ripemd160("hello this is a test")=="f51960af7dd4813a587ab26388ddab3b28d1f7b4");
    
    
    
}


BOOST_AUTO_TEST_CASE(string_bytes) {
    string s = "abc";
    auto v = string_to_bytes(s);
    
    for (int i = 0; i < v.size();i++) {
        BOOST_CHECK(s[i] == v[i]);
    }
    
    
    BOOST_CHECK(bytes_to_string(v)==s);
    
}


BOOST_AUTO_TEST_CASE(ripemd160_sha256_test) {
    
    //empty string to hash
    string s = "";

        
    BOOST_CHECK(uint8_to_hex_string(compute_sha256(string_to_bytes(s))) == "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
    BOOST_CHECK(bytes_to_hex(compute_ripemd160(string_to_bytes(s))) == "9c1185a5c5e9fc54612808977ee8f548b2258d31");
    
    BOOST_CHECK(uint8_to_hex_string(compute_ripemd160(compute_sha256(string_to_bytes(s)))) == "b472a266d0bd89c13706a4132ccfb16f7c3b9fcb");
    
    
    s = "only advance!";

    
    BOOST_CHECK(uint8_to_hex_string(compute_ripemd160(compute_sha256(string_to_bytes(s)))) == "71e8a0cf93adcc523a456a908c948aa842f343f8");
    
    
    
    
}





#endif
