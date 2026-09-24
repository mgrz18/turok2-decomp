#ifndef M2C_MACROS_H
#define M2C_MACROS_H

/*
 * The macros m2c emits with --valid-syntax, so its drafts compile as-is.
 *
 * M2C_FIELD(ptr, type *, offset) is a field access on a struct whose layout
 * is not recovered yet. SN64 GCC compiles the pointer arithmetic to the same
 * load or store a real struct member gives, so a function written this way
 * still matches; naming the struct is a readability pass that comes later.
 */
typedef s32 M2C_UNK;
typedef s8 M2C_UNK8;
typedef s16 M2C_UNK16;
typedef s32 M2C_UNK32;
typedef s64 M2C_UNK64;

#define M2C_FIELD(expr, type, offset) (*(type)((s8 *)(expr) + (offset)))
#define M2C_BITWISE(type, expr) ((type)(expr))
#define M2C_STRUCT_COPY(a, b) (a = b)

#endif
