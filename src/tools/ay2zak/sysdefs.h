#pragma once

#ifdef _WIN32
#include "wchar.h"
#else
typedef unsigned short wchar_t;
#endif

#define inline __inline
#define forceinline __forceinline
#define fastcall __fastcall             // parameters in registers

#ifdef _MSC_VER
typedef signed __int8 int8_t;
typedef signed __int16 int16_t;
typedef signed __int32 int32_t;
typedef signed __int64 int64_t;
typedef unsigned __int8 uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;
#endif

#ifdef __GNUC__
#include <stdint.h>
#define HANDLE_PRAGMA_PACK_PUSH_POP
#endif
