typedef signed int s32;

s32 func_0040AD28(char *arg0, char *arg1) {
    s32 flags = *(s32 *) (arg1 + 0xC);
    s32 alt = flags & 2;

    if (flags & 0x20) {
        return alt ? 0x5640 : 0x56C2;
    }
    return alt ? 0x56B8 : 0x56A4;
}
