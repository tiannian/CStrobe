#include "strobe.h"

namespace CSTORBE_NAMESPACE {
    
template <class T, size_t Sec>
Strobe<T, Sec>::Strobe(uint8_t *proto, size_t protolen):
    pos(0), posbegin(0), I0(-1), initialized(false) {
    uint8_t domain[18] = {
        1, R + 2, 1, 0, 
        1, 12 * 8,
        'S', 'T', 'R', 'O', 'B', 'E', 'v', '0' + STROBE_INTEROP_V_MAJOR, '.',
        '0' + STROBE_INTEROP_V_MINOR, '.', '0' + STROBE_INTEROP_V_PATCH,
    };
    
    _duplex(domain, sizeof(domain), false, false, true);
    
    initialized = true;
    
    operate(FLAGS_A | FLAGS_M, proto, protolen);
}

template <class T, size_t Sec>
Strobe<T, Sec>::Strobe(const Strobe& s):
    pos(s.pos), posbegin(s.posbegin), I0(s.I0),
    initialized(s.initialized), kf(s.kf) {}

template <class T, size_t Sec>
void Strobe<T, Sec>::_run_f() {
    uint8_t *state = (uint8_t *)kf.state;
    if (initialized) {
        state[pos]     ^= posbegin;
        state[pos + 1] ^= 0x04;
        state[R + 1]   ^= 0x80;
    }
    kf.permute();
    pos = 0;
    posbegin = 0;
}

template <class T, size_t Sec>
void Strobe<T, Sec>::_duplex(uint8_t *buffer, size_t buffer_len,
    bool cbefore, bool cafter, bool forcef) {
    uint8_t *state = (uint8_t *)kf.state;
    assert(!(cbefore && cafter));
    for(auto i = 0; i != buffer_len; i++) {
        if (cbefore)
            buffer[i]  ^= state[pos];
        state[pos]  ^= buffer[i];
        if (cafter)
            buffer[i]  = state[pos];
        std::cout << std::dec << "pos:" << (uint32_t)pos 
            << " " << std::hex << (uint32_t) state[i]
            << " " << (uint32_t)buffer[i] << std::endl;
        ++ pos;
        if (pos == R) 
            _run_f();
    }
    if ( forcef && pos != 0 )
        _run_f();
}
template <class T, size_t Sec>
void Strobe<T, Sec>::_begin_op(uint8_t flags) {
    if (flags & FLAGS_T) {
        if (I0 == -1)
            I0 = flags & FLAGS_I;
        flags ^= I0;
    }
    uint8_t oldbegin = posbegin;
    posbegin = pos + 1;
    
    uint8_t buffer[] = { oldbegin, flags };
    _duplex(buffer, sizeof(buffer), false, false, flags & (FLAGS_C | FLAGS_K));
}
 
template <class T, size_t Sec>
bool Strobe<T, Sec>::operate(uint8_t flags, uint8_t *buffer, size_t buffer_len, bool more) {
    assert (! (flags & (FLAGS_K | 1 << 6 | 1 << 7)));
    if (more)
        assert(this->flags == flags);
    else {
        _begin_op(flags);
        this->flags = flags;
    }
    
    bool cafter = (flags & (FLAGS_C | FLAGS_I | FLAGS_T)) == (FLAGS_C | FLAGS_T);
    bool cbefore = (flags & FLAGS_C) && (!cafter);
    bool recv_MAC = (flags & (0xF | FLAGS_I)) == ((FLAGS_T | FLAGS_C) | FLAGS_I);
    
    _duplex(buffer, buffer_len, cbefore, cafter, false);
    
    
    if (recv_MAC) {
        uint8_t MAC_verif = 0;
        MAC_verif |= *buffer;
        if (MAC_verif == 0) {
            return true;
        } else {
            return false;
        }
    }
    return true;
}

template <class T, size_t Sec>
bool Strobe<T, Sec>::AD(uint8_t *buffer, size_t buffer_len, bool more) {
    return operate(FLAGS_A, buffer, buffer_len, more);
}

template <class T, size_t Sec>
bool Strobe<T, Sec>::KEY(uint8_t *buffer, size_t buffer_len, bool more) {
    return operate((FLAGS_A | FLAGS_C), buffer, buffer_len, more);
}

template <class T, size_t Sec>
bool Strobe<T, Sec>::PRF(uint8_t *buffer, size_t buffer_len, bool more) {
    return operate((FLAGS_I | FLAGS_A | FLAGS_C), buffer, buffer_len, more);
}

template <class T, size_t Sec>
bool Strobe<T, Sec>::send_CLR(uint8_t *buffer, size_t buffer_len, bool more) {
    return operate((FLAGS_T | FLAGS_A), buffer, buffer_len, more);
}

template <class T, size_t Sec>
bool Strobe<T, Sec>::recv_CLR(uint8_t *buffer, size_t buffer_len, bool more) {
    return operate((FLAGS_I | FLAGS_T | FLAGS_A), buffer, buffer_len, more);
}

template <class T, size_t Sec>
bool Strobe<T, Sec>::send_ENC(uint8_t *buffer, size_t buffer_len, bool more) {
    return operate((FLAGS_A | FLAGS_C | FLAGS_T), buffer, buffer_len, more);
}

template <class T, size_t Sec>
bool Strobe<T, Sec>::recv_ENC(uint8_t *buffer, size_t buffer_len, bool more) {
    return operate((FLAGS_I | FLAGS_A | FLAGS_C | FLAGS_T), buffer, buffer_len, more);
}

template <class T, size_t Sec>
bool Strobe<T, Sec>::send_MAC(uint8_t *buffer, size_t buffer_len, bool more) {
    return operate((FLAGS_C | FLAGS_T), buffer, buffer_len, more);
}

template <class T, size_t Sec>
bool Strobe<T, Sec>::recv_MAC(uint8_t *buffer, size_t buffer_len, bool more) {
    return operate((FLAGS_I | FLAGS_C | FLAGS_T), buffer, buffer_len, more);
}

template <class T, size_t Sec>
bool Strobe<T, Sec>::RATCHET(uint8_t *buffer, size_t buffer_len, bool more) {
    return operate(FLAGS_C, buffer, buffer_len, more);
}

template class Strobe<uint64_t, 128>;
template class Strobe<uint64_t, 256>;
template class Strobe<uint32_t, 128>;
template class Strobe<uint32_t, 256>;
template class Strobe<uint16_t, 128>;

}
