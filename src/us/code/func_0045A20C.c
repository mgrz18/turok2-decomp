/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/PAK/80405DC0_16.CPP, func_80409884_de, with Turok 2's layouts. */
typedef signed int s32;

extern char *D_800F70A8;

char *func_0045A20C(s32 block) {
    if (block == 0) {
        return D_800F70A8 + 0x688;
    }
    return (char *) (block << 8);
}
