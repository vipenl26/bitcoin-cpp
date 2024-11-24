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
public:
    vector<uint8_t> prev_tx;
    uint256_t prev_index;
    Script script_sig;
    uint256_t sequence = 0xffffffff;
    
    TxIn() {
        
    }
    TxIn(vector<uint8_t> prev_tx, uint256_t prev_index) {
        
    }
    
    vector<uint8_t> encode(int script_override=-1) {
        vector<uint8_t> out, t;
        
        for (int i = (int)prev_tx.size() - 1; i>=0;i--) {
            out.push_back(prev_tx[i]);
        }
        t = Point::to_bytes_little(prev_index, 4);
        out.insert(out.end(), t.begin(), t.end());
        
        if (script_override == -1) {
            t = script_sig.encode();
        }
        else if(script_override == 1) {
            
        }
        else if(script_override == 0) {
            Script script;
            t = script.encode();
        }
        
        out.insert(out.end(), out.begin(), out.end());
        
        t = Point::to_bytes_little(sequence, 4);
        
        out.insert(out.end(), out.begin(), out.end());
        return out;
        
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
    
    vector<uint8_t> encode() {
        vector<uint8_t> out, t;
        
        t = Point::to_bytes_little(amount, 8);
        out.insert(out.end(), t.begin(), t.end());
        
        t = script_pubkey.encode();
        
        out.insert(out.end(), t.begin(), t.end());
        
        return out;
    }

    
};

class Tx{
public:
    
    uint256_t version, locktime=0;
    vector<TxIn> tx_ins;
    vector<TxOut> tx_outs;
    
    Tx() {
        
    }
    Tx(uint256_t version, vector<TxIn> a, vector<TxOut> b) {
        this->version = version;
        tx_ins = a;
        tx_outs = b;
    }
    
    vector<uint8_t> encode(int sig_index=-1) {
        vector<uint8_t> out, t;
        
        out = Script::encode_int(version, 4);
        
        t = Script::encode_variant(tx_ins.size());
        out.insert(out.end(), t.begin(), t.end());
        
        if (sig_index == -1) {
            for (auto tx_in: tx_ins) {
                t = tx_in.encode();
                out.insert(out.begin(), t.begin(), t.end());
            }
            
        }
        else {
            for (int i = 0; i < tx_ins.size();i++) {
                t = tx_ins[i].encode();
                out.insert(out.begin(), t.begin(), t.end());
            }
        }
        
        t = Script::encode_variant(tx_outs.size());
        out.insert(out.begin(), t.begin(), t.end());
        
        for (auto &tx_out: tx_outs) {
            t = tx_out.encode();
            out.insert(out.begin(), t.begin(), t.end());
        }
        
        t = Script::encode_int(locktime, 4);
        out.insert(out.begin(), t.begin(), t.end());
        
        
        if (sig_index != -1) {
            t = Script::encode_int(1, 4);
            out.insert(out.end(), t.begin(), t.end());
        }
        return out;
        
    }
};

static auto tx_in = TxIn(string_to_bytes("46325085c89fb98a4b7ceee44eac9b955f09e1ddc86d8dad3dfdcba46b4d36b2"), 1);



static PublicKey pk2(public_key2);

static auto out1_pkb_hash = pk2.encode(true, true);

static Script out1_script({Script::item(118), Script::item(169), Script::item(out1_pkb_hash), Script::item(136), Script::item(172)});

static PublicKey pk1(public_key);

static auto out2_pkb_hash = pk1.encode(true, true);

static Script out2_script({Script::item(118), Script::item(169), Script::item(out2_pkb_hash), Script::item(136), Script::item(172)});


static TxOut tx_out1 = TxOut(50000, out1_script);
static TxOut tx_out2 = TxOut(47500, out2_script);


static auto tx = Tx(1, {tx_in}, {tx_out1, tx_out2});

