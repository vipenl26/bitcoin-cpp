//
//  point.cpp
//  bitcoin-cpp
//
//  Created by Vipen Loka on 09/11/24.
//

#include <boost/multiprecision/cpp_int.hpp>
#include "curve.h"
#include "utils.h"
using namespace boost::multiprecision;
using namespace std;
#define deb(x) {cout << #x << ':' << x << '\n';}

#ifndef MY_POINT
#define MY_POINT
class Point {
public:
    Curve curve;
    int256_t x, y;
    
    bool is_inf = false;
    
    // default starting point
    Point():x("0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798"),y("0x483ada7726a3c4655da4fbfc0e1108a8fd17b448a68554199c47d08ffb10d4b8") {
        is_inf = false;

    
    }
    Point(Curve curve, int256_t x, int256_t y):curve(curve), x(x),y(y){
    }
    
    //TODO: understand why polish doesn't work
    void polish(){
        if(x<0)x = (x + curve.p) % curve.p;
        if(y<0)y = (y + curve.p) % curve.p;
    }
    
    Point(bool is_inf) {
        this->is_inf = is_inf;
    }
    void show(int256_t x)const {
        x = x % curve.p;
        if (x < 0)x += curve.p;
        
        cout << x << endl;
    }
    bool eq(const int256_t a, const int256_t b) const{
        int256_t x = a, y = b;
        if (x < 0)x += curve.p;
        if (y < 0)y += curve.p;
        x = x % curve.p;
        y = y % curve.p;
        return x==y;
    }

    // this is not used due to multiplication overflow
    inline Point elliptic_curve_addition(Point other) {
        if (this->is_inf) {
            return other;
        }
        if (other.is_inf) {
            return *this;
        }
        
        if (this->x == other.x && this->y != other.y) {
            return Point(true);
        }
        //this is slope
        int256_t m;
        if (this->x == other.x) {
            m = (3 * x + curve.a) * inv(2 * y, curve.p);
        }
        else {
            m = (y - other.y) * inv(x - other.x, curve.p);
        }
       
        int256_t rx = (m * m - x - other.x) % curve.p;
        int256_t ry = (-(m * (rx - x) + y)) % curve.p;
        
        return Point(curve, rx, ry);
        
    }
    
    Point operator+(const Point& other)const {
        if (is_inf) {
            return other;
        }
        if (other.is_inf) {
            return *this;
        }
        
        if (eq(x, other.x) && !eq(y, other.y)) {
            return Point(true);
        }
        //this is slope
        int256_t m;
        if (eq(x, other.x)) {
//            m = (3 * x * x + curve.a) * inv(2 * y, curve.p);
            m = modmul(modadd(modmul(3, pow(x, 2, curve.p), curve.p), curve.a, curve.p),
                       
                       inv(modmul(2, y, curve.p), curve.p), curve.p);
            
        }
        else {
//            m = (y - other.y) * inv(x - other.x, curve.p);
            m = modmul(modadd(y, -other.y, curve.p),
                       inv(modadd(x, -other.x, curve.p), curve.p), curve.p);
        }
       
        
//        int256_t rx = (m * m - x - other.x) % curve.p;
        
        int256_t rx = modadd(modadd(pow(m, 2, curve.p), -x, curve.p), -other.x, curve.p);
        
        
//        int256_t ry = (-(m * (rx - x) + y)) % curve.p;
        
        int256_t ry = (-modadd(modmul(m, modadd(rx, -x, curve.p), curve.p), y, curve.p)) % curve.p;
        
        
        
        return Point(curve, rx, ry);
    }
    
    Point operator*(int256_t x){
        Point res(true);// INF
        Point G(*this);
        
        while (x > 0) {
            if (x & 1) {
                res = res + G;
            }
            G = G + G;
            x = x >> 1;
        }
        
        return res;
        
    }
    

    bool operator==(const Point& other) {
        if (is_inf || other.is_inf) {
            return is_inf && other.is_inf;
        }

        return eq(x, other.x) && eq(y, other.y) && curve==other.curve;
    }
    
    bool check_point_on_curve() {
        return eq(pow(y, 2, curve.p) , modadd(modadd(pow(x, 3, curve.p) , modmul(x, curve.a, curve.p),curve.p) , curve.b, curve.p));
    }
    
    
};


static Point G;

#endif

