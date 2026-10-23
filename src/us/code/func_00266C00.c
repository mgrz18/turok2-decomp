typedef signed int s32;
typedef unsigned int u32;

void func_00266C00(u32 *dst, u32 *src, s32 len) {
    len >>= 2;
    if (dst < src) {
        do {
            *dst++ = *src++;
        } while (--len != 0);
    } else {
        dst += len - 1;
        src += len - 1;
        do {
            *dst-- = *src--;
        } while (--len != 0);
    }
}
