#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <map>
#include <string>
#include "nlohmann/json.hpp"

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

void assert_state(const string &s,uint64_t *state) {
    stringstream ss;
    ss << s;
    uint64_t buffer;
    for (auto i = 0; i!= 25; i++) {
        ss >> hex >> buffer;
        //cout << hex << buffer << " " << state[i] << endl;
        //assert(buffer == state[i]);
        uint8_t *pb = (uint8_t*)&buffer;
        uint8_t *ps = (uint8_t*)&state[i];
        assert(pb[0] == ps[7]);
        assert(pb[1] == ps[6]);
        assert(pb[2] == ps[5]);
        assert(pb[3] == ps[4]);
        assert(pb[4] == ps[3]);
        assert(pb[5] == ps[2]);
        assert(pb[6] == ps[1]);
        assert(pb[7] == ps[0]);
        
    }
}

int main() {
    string s = "9c7f75fa8b893ab7 0ad448513c0b5263 dc0c475c551526f6 "
               "733bea22f16cb57c d31f682e660ee912 824a772201ee1394 "
               "226f4afcb62d3312 93cc92e8a624acf6 e1b60095e322bbfb "
               "c845e5b26995fe7d 7c841374d1ff5898 c92ee0636b067273 "
               "21c92a6039070353 49ccbb1b92b7b005 7e8fa87fcebc7e88 "
               "656fcb45ae04bc34 cabeaebe79d91750 c0e8bf13b966504d "
               "1343597265dd8865 adf91409cc9b20d5 f47444041f97b699 "
               "ddfbdee91ea87bd0 9bf8b02da75a96e9 47f07f5b65bb4e6e "
               "fefaa16abfd9fbf6";
    uint8_t proto[] = "custom string";
    Strobe<uint64_t, 128> strobe(proto, sizeof(proto) - 1);
    assert_state(s,strobe.kf.state);
    
    s = "3031303130310e6b e02d3677fb5a70df c1f3a6f889f56169 "
        "9f053f97f705d671 383c613b13b3e418 4397827ad73b9c26 "
        "e13af703f0bf70ff 10d71516bb8a05a5 b69775f1722daf91 "
        "78b71cfca96cf36c d0fab68cac22f3f3 5afec0bf5066656d "
        "142761e3200b9e76 a9ddd6ad74932110 afd4a776fd82515e "
        "dfe028dd4858abfd a1c3b6bff17e9b82 aad3b519e51e704c "
        "ba48a6e383c8504c d1f7b13d7a6ac6e9 690ed05ae90744c5 "
        "c32a97af08d66f15 c9357bf20ea9276c ab5ac736b8a68f25 "
        "b095c409a1c48fd4";
    
    uint8_t data1[] = {0x30, 0x31, 0x30, 0x31, 0x30, 0x31};
    strobe.KEY(data1, 6);
    assert_state(s,strobe.kf.state);
    
    s = "3031303130310e69 88485a1b947650b7 ae848699fb904110 "
        "f0701ff0986ab251 4b55130413b3e418 4397827ad73b9c26 "
        "e13af703f0bf70ff 10d71516bb8a05a5 b69775f1722daf91 "
        "78b71cfca96cf36c d0fab68cac22f3f3 5afec0bf5066656d "
        "142761e3200b9e76 a9ddd6ad74932110 afd4a776fd82515e "
        "dfe028dd4858abfd a1c3b6bff17e9b82 aad3b519e51e704c "
        "ba48a6e383c8504c d1f7b13d7a6ac6e9 690ed05ae90744c5 "
        "c32a97af08d66f15 c9357bf20ea9276c ab5ac736b8a68f25 "
        "b095c409a1c48fd4";
    
    uint8_t data2[] = "hello, how are you good sir?";
    strobe.AD(data2, sizeof(data2) - 1);
    assert_state(s,strobe.kf.state);
    
    s = "0000000000000000 0000000000000000 6380b09c7f711ca1 "
        "c97eb6612cf25c2a b2abfb9cba447b27 e72134629c394728 "
        "1a0be4044d4e03c6 0564c66885407a2e 6982603b07390ab7 "
        "18c0fa70f25e857c 189caf2bdf427866 69bc2f5d43440869 "
        "e8feb7d47780383b b82c84705220c4d0 8b5df18081cb4939 "
        "f3a38d059ee18529 e85c6dade82e3c8b 3c6cd0b56203299b "
        "b7311dfcbb68d1b5 be5df6d2b8697dd6 0e89ca4fd78ffbd1 "
        "c22b939311775d91 3a8bf324b8ebac78 2e1f99605eccf03c "
        "0e59770c177812d1";
    uint8_t data3[16] = {0};
    strobe.PRF(data3,16);
    assert_state(s,strobe.kf.state);
    dump_8(data3, 16);
    
    s = "dc0c857f045d9941 fed8acbfd024f1a0 647e59b1f9c9f778 "
        "6871ce439f5bf131 2ad75b62aa59c3ca 8a38044d367ebe46 "
        "91590525637a7f45 075271dd34cd0a05 bf987d464fea1353 "
        "dbf05620dd5f1dde 57bb3d768bb52bcb a95142437bbc26ca "
        "2c0c314aebc894ba 07fea87de5ceda01 5c55f39ef8f2732b "
        "76a0d903ddb0f648 e1fef2188dfb79c3 c2ef4d323d04a81a "
        "ac8e292f156320b4 0ec7c8bf7f0f2ec2 322768999ce3ca9b "
        "5ccb8aad6b93e5db b4a356c0645e7244 5cc995fdaa42f902 "
        "eb4acf0c1bca2cb5";
    uint8_t data4[] = "hi how are you";
    strobe.send_ENC(data4, sizeof(data4) - 1);
    assert_state(s,strobe.kf.state);
    dump_8(data4, sizeof(data4) - 1);
    
    s = "686920686f772061 726520796f75d18d 928ffc684bdeaf12 "
        "976ddcb534b3f340 fb8ea4336037b41e fc715070db7d87c3 "
        "5acf29cce981dc58 6393de1e340351b5 9e8f702273f1d69b "
        "b6533343e05963f1 8018b08a6035d663 35d2c4ed4223cebc "
        "4f687c3820690b5e d1a890659b8a12a9 62b7655446635ee7 "
        "3d1ed258e732ac69 73611bd7effbbbc0 aa68042b80a97b48 "
        "007d95b220b09bb3 9713381489c63670 9397505a20c141dc "
        "70cc9d9e0292115d 05fbbb8fca1981f8 cf9f328e9e3ebddc "
        "84c88fbcb50b93d4";
    uint8_t data5[] = "hi how are you";
    strobe.recv_ENC(data5, sizeof(data5) - 1);
    assert_state(s,strobe.kf.state);
    dump_8(data5, sizeof(data5) - 1);
    
    s = "a05d94e152949460 c67d648e91bc6d53 9a4a0e9a96b42f58 "
        "0d0c4887170542c1 34c11f571e5c1a7b 6f1282dc07bf41ba "
        "08ddd9eebb49e7f1 290a6fccd5dfd7c7 2f5787d6c274013f "
        "53453652be13a01d bf390b0cc56d6800 f7fae185e77b98ca "
        "2a0a689307a228c0 26eb67ea2cf0f302 bf077a4eaf122d21 "
        "1aaa4c839cb3872f 34a1693f089997f1 984900b91c051428 "
        "b55232a158d83d78 d6d2f5175555c741 e3bd78fef961cffc "
        "7069ed5e3483b459 3a54b079fc934a73 073e7b6b0ad56b46 "
        "286202c927100704";
    uint8_t data6[16] = {0};
    strobe.send_MAC(data6,16);
    assert_state(s,strobe.kf.state);
    dump_8(data6,16);
    
    {
        string s = "686920686f772061 726520796f75dbd0 6ff7498e4fe2bc4b "
                   "d620dd2c4ddf93c8 280db4c3c2f7400f 50efdd714a6ee108 "
                   "c8db7fc425de03ef 1116b477b929c350 76f2b0e72e7517f7 "
                   "e57b51cc85d57f73 fee55a9db690240b a2f16ea11bd83cfd "
                   "e4df8a70f66c81a5 2a7e3ca21342ba74 662b42273e5129de "
                   "3c2d1ca8f0dae234 9c9a9b7cf0df634b a3112f541a036779 "
                   "f446afbeff32e642 3c0e58da979b8d87 d66de030eb89bf2e "
                   "1f95b21efccd41f7 b44b8ca5c2e8dfed 34e485a6195b3bde "
                   "6390793866772361";
        uint8_t data[] = "hi how are you";
        strobe.recv_MAC(data,sizeof(data) - 1);
        assert_state(s,strobe.kf.state);
        dump_8(data,sizeof(data) - 1);
    }
    
    return 0;
}
