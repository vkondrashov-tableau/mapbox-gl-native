#pragma once

// Polyfill needed by Windows because MSVC STL
// is not compatible with our IndexedTuple code
#if 0

#include <tao/tuple/tuple.hpp>

#define get_polyfill tao::get
#define tuple_polyfill tao::tuple
#define tuple_cat tao::tuple_cat

#else

#include <tuple>

#define get_polyfill std::get
#define tuple_polyfill std::tuple
#define tuple_cat std::tuple_cat

#endif
