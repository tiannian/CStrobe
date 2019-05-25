#ifndef CSTROBE_STROBE_H
#define CSTROBE_STROBE_H

#include "config.h"
#include "ckeccpp/keccakf.h"

namespace CSTORBE_NAMESPACE {

template <class T, size_t Sec>
class Strobe {
    public:
        ckeccpp::KeccakF<T> kf;
    public:
        Strobe(uint8_t *proto, size_t protolen);
        Strobe(const Strobe&);
        ~Strobe() = default;
    public:
        static const size_t sec = Sec;
        static const size_t N = ckeccpp::KeccakF<T>::nbytes;
        static const size_t R = N - ( 2* sec ) / 8 - 2;
    public:
        static const uint8_t FLAGS_I = 1 << 0;
        static const uint8_t FLAGS_A = 1 << 1;
        static const uint8_t FLAGS_C = 1 << 2;
        static const uint8_t FLAGS_T = 1 << 3;
        static const uint8_t FLAGS_M = 1 << 4;
        static const uint8_t FLAGS_K = 1 << 5;
    public:
        static const uint8_t STROBE_INTEROP_V_MAJOR = 1;
        static const uint8_t STROBE_INTEROP_V_MINOR = 0;
        static const uint8_t STROBE_INTEROP_V_PATCH = 2;
    public:
        bool operate(uint8_t flags, uint8_t *buffer, size_t buffer_len, bool more=false);
        bool AD(uint8_t *buffer, size_t buffer_len, bool more=false);
        bool KEY(uint8_t *buffer, size_t buffer_len, bool more=false);
        bool PRF(uint8_t *buffer, size_t buffer_len, bool more=false);
        bool send_CLR(uint8_t *buffer, size_t buffer_len, bool more=false);
        bool recv_CLR(uint8_t *buffer, size_t buffer_len, bool more=false);
        bool send_ENC(uint8_t *buffer, size_t buffer_len, bool more=false);
        bool recv_ENC(uint8_t *buffer, size_t buffer_len, bool more=false);
        bool send_MAC(uint8_t *buffer, size_t buffer_len, bool more=false);
        bool recv_MAC(uint8_t *buffer, size_t buffer_len, bool more=false);
        bool RATCHET(uint8_t *buffer, size_t buffer_len, bool more=false);
    private:
        void _run_f();
        void _duplex(uint8_t *buffer, size_t buffer_len,
            bool cbefore, bool cafter, bool forcef);
        void _begin_op(uint8_t flags);
    private:
        uint8_t pos, posbegin;
        uint8_t I0;
        bool initialized;
        uint8_t flags;
};

}

#endif
