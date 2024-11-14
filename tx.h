//
//  tx.h
//  bitcoin-cpp
//
//  Created by Vipen Loka on 12/11/24.
//

#include <vector>
#include <boost/multiprecision/cpp_int.hpp>
#include "point.cpp"
#include "publickey.h"
using namespace std;
using namespace boost::multiprecision;

class Script {
public:
    
    struct item {
        bool is_int = true;
        uint256_t x;
        vector<uint8_t> y;
        item(uint256_t x) {
            this->x = x;
        }
        item(vector<uint8_t> y) {
            this->y = y;
            is_int = false;
            
        }
    };
    vector<item> cmds;
    Script() {
        
    }
    
    Script(vector<item> cmds) {
        this->cmds = cmds;
    }
    
    
    static vector<uint8_t> encode_int(uint256_t i, int nbytes, string encoding="little") {
        return  encoding=="little" ? Point::to_bytes_little(i, nbytes): Point::to_bytes(i, nbytes);
    }
    
    
    static vector<uint8_t> encode_variant(uint256_t i) {
        vector<uint8_t> out,t;
        uint256_t mx("0x10000000000000000");
        if (i < 0xfd) {
            return Point::to_bytes_little(i, 1);
        }
        else if(i < 0x10000) {
            out.push_back('\xfd');
            t = Point::to_bytes_little(i, 2);
        }
        else if(i < 0x100000000) {
            out.push_back('\xfe');
            t = Point::to_bytes_little(i, 4);
        }
        else if (i < mx){
            out.push_back('\xff');
            t = Point::to_bytes_little(i, 8);
        }
        else {
            cout << "ERROR!: value too large for decode_variant" << endl;
        }
        
        out.insert(out.end(), t.begin(), t.end());
        return out;
        
        
    }
    
    
    vector<uint8_t> encode() {
        vector<uint8_t> out;
        for (auto &cmd: cmds) {
            if (cmd.is_int) {
                auto t = Point::to_bytes_little(cmd.x, 1);
                out.insert(out.end(), t.begin(), t.end());
            }
            else {
                int length = cmd.y.size();
                assert(length < 75);
                auto t = Point::to_bytes_little((uint256_t)length, 1);
                out.insert(out.end(), t.begin(), t.end());
                out.insert(out.end(), cmd.y.begin(), cmd.y.end());
                
            }
        }
        
        auto res = encode_variant(out.size());
        res.insert(res.end(), out.begin(), out.end());
        
        
        
        return res;
    }
};
class TxIn {
    vector<uint8_t> prev_tx;
    uint256_t prev_index;
//    Script script_seg;
    uint256_t sequence = 0xffffffff;
    
    void txin_encode() {
        
    }
};

class TxOut{
public:
    uint256_t amount;
    Script script_pubkey;

    TxOut(uint256_t amount) {
        this->amount=amount;
    }
    TxOut(uint256_t amount, Script script_pubkey) {
        this->amount=amount;
        this->script_pubkey = script_pubkey;
    }

    
};

class Tx{
public:
    uint256_t version, locktime=0;
    vector<TxIn> tx_ins, tx_outs;
    
    void encode(int sig_index=-1) {
        vector<uint8_t> out, t;
        
        out = Script::encode_int(version, 4);
        
        t = Script::encode_variant(tx_ins.size());
        out.insert(out.end(), t.begin(), t.end());
        
        if (sig_index == -1) {
//            t =
        }
    }
};




static PublicKey pk2(public_key2);

static auto out1_pkb_hash = pk2.encode(true, true);

static Script out1_script({Script::item(118), Script::item(169), Script::item(out1_pkb_hash), Script::item(136), Script::item(172)});

static PublicKey pk1(public_key);

static auto out2_pkb_hash = pk1.encode(true, true);

static Script out2_script({Script::item(118), Script::item(169), Script::item(out2_pkb_hash), Script::item(136), Script::item(172)});


static TxOut tx_out1 = TxOut(50000, out1_script);
static TxOut tx_out2 = TxOut(47500, out2_script);

