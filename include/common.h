#ifndef COMMON_H
#define COMMON_H

/* Basic N64 SDK integer and float types (PR/ultratypes.h). */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;

/* Integers that carry a pointer (docs/port/64BIT.md, "Decisions"): the m2c-era
 * code moves addresses through s32/u32. On the N64 these are exactly s32/u32,
 * so the matching build compiles the same code; the native build
 * (TARGET_PORT) makes them pointer-sized. Never use them in an asset struct
 * (those keep 4-byte slots). */
#ifdef TARGET_PORT
typedef __INTPTR_TYPE__ sptr;
typedef __UINTPTR_TYPE__ uptr;
#else
typedef s32 sptr;
typedef u32 uptr;
#endif

#define NULL 0

#endif
