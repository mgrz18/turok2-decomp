typedef signed int s32;

s32 func_0044FC54(char *obj) {
    s32 v = *(s32 *) (obj + 0x1C8);
    if (v < 24) {
        return v;
    }
    return 47 - v;
}
