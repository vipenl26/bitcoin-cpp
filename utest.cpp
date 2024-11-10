//
//  utest.cpp
//  bitcoin-cpp
//
//  Created by Vipen Loka on 09/11/24.
//

// test_math_utils.cpp
#define BOOST_TEST_MODULE MathUtilsTest  // Defines the test module name
#include <boost/test/included/unit_test.hpp>
#include <iostream>
#include "utils.h"  // Include the header file of the function to test
#include "main.cpp"
#include "point.cpp"

#ifdef TESTING_THIS

BOOST_AUTO_TEST_CASE( inv_test ) {
    BOOST_CHECK(inv((int256_t)17, (int256_t)43) == (int256_t)38);
}


BOOST_AUTO_TEST_CASE( point_addition_test ) {
    int256_t sk;
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
    int256_t secret_key("22265090479312778178772228083027296664144");
    
    Point public_key = G * secret_key;
    
    int256_t expected_x("83998262154709529558614902604110599582969848537757180553516367057821848015989"),
    expected_y("37676469766173670826348691885774454391218658108212372128812329274086400588247");
    public_key.polish();
    
    BOOST_CHECK(public_key.x == expected_x);
    BOOST_CHECK(public_key.y == expected_y);
    BOOST_CHECK(public_key.check_point_on_curve());
    
}



#endif
