#pragma once

#ifdef _MSC_VER
#define SOL_FORCE_INLINE __forceinline
#else
#define SOL_FORCE_INLINE [[gnu::always_inline]]
#endif
