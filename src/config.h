#ifndef CSTROBE_CONFIG_H
#define CSTROBE_CONFIG_H

#define CSTORBE_NAMESPACE cstrobe

#define CSTORBE_DEBUG_OPTION 1

/*
#include <cstring>
using std::memcpy;
using std::memset;
*/

#include <cassert>

#if CSTORBE_DEBUG_OPTION == 1

#include <iostream>

#define CSTORBE_DEBUG(x) std::cout << x << std::endl

#else

#define CSTORBE_DEBUG(x) ((void)0)
    
#endif

#endif

