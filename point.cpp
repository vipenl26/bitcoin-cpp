//
//  point.cpp
//  bitcoin-cpp
//
//  Created by Vipen Loka on 09/11/24.
//

#include "curve.h"
#include "utils.h"
#include <boost/multiprecision/cpp_int.hpp>

using namespace boost::multiprecision;
using namespace std;

#ifndef MY_POINT
#define MY_POINT
#define deb(x)                                                                 \
    { cout << #x << ':' << x << '\n'; }
class Point {
  public:
    Curve curve;
    uint256_t x, y;

    bool is_inf = false;

    // default starting point
    Point()
        : x("0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F8179"
            "8"),
          y("0x483ada7726a3c4655da4fbfc0e1108a8fd17b448a68554199c47d08ffb10d4b"
            "8") {
        is_inf = false;
    }
    Point(Curve curve, uint256_t x, uint256_t y) : curve(curve), x(x), y(y) {}

    static vector<uint8_t> to_bytes(uint256_t input, int nbytes=32) {
        vector<uint8_t> res(nbytes);
        for (int i = nbytes-1; i >= 0; i--) {
            res[i] = static_cast<uint8_t>(input % 256);
            input /= 256;
        }
        return res;
    }
    static vector<uint8_t> to_bytes_little(uint256_t input, int nbytes=32) {
        vector<uint8_t> res = to_bytes(input, nbytes);
        
        reverse(res.begin(), res.end());
        
        return res;
        
    }

    static uint256_t from_bytes(vector<uint8_t> input) {
        uint256_t res = 0;
        for (int i = 0; i < input.size(); i++) {
            res = res * 256 + input[i];
        }
        return res;
    }

    Point(bool is_inf) { this->is_inf = is_inf; }
    bool eq(const uint256_t a, const uint256_t b) const {
        uint256_t x = a, y = b;
        if (x < 0)
            x += curve.p;
        if (y < 0)
            y += curve.p;
        x = x % curve.p;
        y = y % curve.p;
        return x == y;
    }

    // this is not used due to multiplication overflow
    //    inline Point elliptic_curve_addition(Point other) {
    //        if (this->is_inf) {
    //            return other;
    //        }
    //        if (other.is_inf) {
    //            return *this;
    //        }
    //
    //        if (this->x == other.x && this->y != other.y) {
    //            return Point(true);
    //        }
    //        //this is slope
    //        uint256_t m;
    //        if (this->x == other.x) {
    //            m = (3 * x + curve.a) * inv(2 * y, curve.p);
    //        }
    //        else {
    //            m = (y - other.y) * inv(x - other.x, curve.p);
    //        }
    //
    //        uint256_t rx = (m * m - x - other.x) % curve.p;
    //        uint256_t ry = (-(m * (rx - x) + y)) % curve.p;
    //
    //        return Point(curve, rx, ry);
    //
    //    }

    Point operator+(const Point &other) const {
        if (is_inf) {
            return other;
        }
        if (other.is_inf) {
            return *this;
        }

        if (eq(x, other.x) && !eq(y, other.y)) {
            return Point(true);
        }
        // this is slope
        uint256_t m;
        if (eq(x, other.x)) {
            //            m = (3 * x * x + curve.a) * inv(2 * y, curve.p);
            m = modmul(modadd(modmul(3, pow(x, 2, curve.p), curve.p), curve.a,
                              curve.p),

                       inv(modmul(2, y, curve.p), curve.p), curve.p);

        } else {
            //            m = (y - other.y) * inv(x - other.x, curve.p);
            m = modmul(modsub(y, other.y, curve.p),
                       inv(modsub(x, other.x, curve.p), curve.p), curve.p);
        }

        //        int256_t rx = (m * m - x - other.x) % curve.p;

        uint256_t rx =
            modsub(modsub(pow(m, 2, curve.p), x, curve.p), other.x, curve.p);

        //        int256_t ry = (-(m * (rx - x) + y)) % curve.p;

        uint256_t ry =
            neg(modadd(modmul(m, modsub(rx, x, curve.p), curve.p), y, curve.p),
                curve.p) %
            curve.p;

        return Point(curve, rx, ry);
    }

    Point operator*(uint256_t x) {
        Point res(true); // INF
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

    bool operator==(const Point &other) {
        if (is_inf || other.is_inf) {
            return is_inf && other.is_inf;
        }

        return eq(x, other.x) && eq(y, other.y) && curve == other.curve;
    }

    bool check_point_on_curve() {
        return eq(pow(y, 2, curve.p),
                  modadd(modadd(pow(x, 3, curve.p), modmul(x, curve.a, curve.p),
                                curve.p),
                         curve.b, curve.p));
    }
};

static Point G;
// using same string as Andrej to verifty the public_key
static string private_key_string = "Andrej is cool :P";

static uint256_t
    secret_key(Point::from_bytes(string_to_bytes(private_key_string)));
static auto public_key = G * secret_key;


static auto secret_key2(Point::from_bytes(string_to_bytes("Andrej's Super Secret 2nd Wallet")));
static auto public_key2 = G * secret_key2;

#endif
