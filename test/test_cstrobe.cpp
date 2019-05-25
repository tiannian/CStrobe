#include <iostream>
#include <iomanip>

#include "strobe.h"

using namespace cstrobe;
using namespace std;

void dump(uint64_t *s) {
    uint8_t *state = (uint8_t*)s;
    for(auto i = 0; i != 200; i++) {
        cout << setw(2) << setfill('0') << hex << (uint64_t) state[i] << "";
    }
    cout << endl;
}

void dump_8(uint8_t *s, size_t len) {
    for(auto i = 0; i != len; i++) {
        cout << setw(2) << setfill('0') << hex << (uint64_t) s[i] << "";
    }
    cout << endl;
}

int main() {
    uint8_t proto[] = "custom string";
    Strobe<uint64_t, 128> strobe(proto, sizeof(proto) - 1);
    dump(strobe.kf.state);
    
    cout << "-----------" << endl;
    
    uint8_t data1[] = {0x30, 0x31, 0x30, 0x31, 0x30, 0x31};
    strobe.KEY(data1,6);
    dump(strobe.kf.state);
    dump_8(data1,6);
    
    uint8_t data2[] = {0x68,0x65,0x6c,0x6c,0x6f,0x2c,0x20,0x68,
    0x6f,0x77,0x20,0x61,0x72,0x65,0x20,0x79,0x6f,0x75,0x20,
    0x67,0x6f,0x6f,0x64,0x20,0x73,0x69,0x72,0x3f};
    strobe.AD(data2,sizeof(data2));
    dump(strobe.kf.state);
    return 0;
}
