# CStrobe

> Strobe Protocol Framework in C++.

## Usage

Initialize `Strobe` statement.

``` c++
#include "cstrobe/strobe.h"

cstrobe::Strobe<uint64_t, 256> strobe; // Strobe-256/1600.
```

## Feature

- [x] Basic implementation of the protocol framework.
- [ ] Test basic operator. (Need test vector.) (Init, KEY, AD, PRF, send_ENV, recv_ENV, send_MAC tested.)
- [ ] Test all paremeters.
- [ ] Add meta operate support.
- [ ] Test meta operator.

