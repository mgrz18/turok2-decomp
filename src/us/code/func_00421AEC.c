typedef signed int s32;

extern char D_0043AC80[];
extern char D_0043ACA4[];
extern char D_0043ACC8[];
extern char D_0043ACEC[];

char *func_00421AEC(s32 unused, s32 which) {
    switch (which) {
        case 0:
        default:
            return D_0043AC80;
        case 1:
            return D_0043ACA4;
        case 2:
            return D_0043ACC8;
        case 3:
            return D_0043ACEC;
    }
}
