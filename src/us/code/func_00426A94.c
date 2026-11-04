/* WRONG CUT: REAL func_00426A94 size 0x144 (splat cut it into func_00426A94 + func_00426AF8 + func_00426BA8). */
#include "common.h"
#include "structs/pak.h"
extern void **D_800C2228;
extern s32 D_800C2220;
extern u8 D_800F6750;

extern void **func_00204EDC(s32, s32, s32, char *);
extern void func_002053A8(s32);
extern void func_002052D8(s32, void **);
extern void func_00285878(s32);
extern void func_002847E0(void);
extern void func_002858A4(void);
extern void *func_0029E460(void *dst, const void *src, s32 size);
extern s32 func_004258E8(s32 ch);
extern s32 osPfsReadWriteFile(OSPfs *pfs, s32 file_no, u8 flag, s32 offset, s32 size, void *data);

extern s32 func_00425E74(s32 ch, s32 index, s32 *exists, u8 *name, u8 *ext, s32 *size, u8 *company,
                         u8 *code);

/* Picks the first note extension letter ('A' + n) no note of the pak on channel ch uses. */
s32 func_00426A94(s32 ch, u8 *ext) {
    u8 company[8];
    u8 code[8];
    u8 name[16];
    s32 exists;
    s32 size;
    s32 used;
    s32 i;

    used = 0;
    if (D_80130C60[ch] != 3) {
        return -2;
    }
    for (i = 0; i < 16; i++) {
        if (func_00425E74(ch, i, &exists, name, ext, &size, company, code) != 0) {
            return -1;
        }
        if (exists) {
            used |= 1 << (ext[0] - 'A');
        }
    }
    ext[0] = 'A';
    ext[1] = 0;
    ext[2] = 0;
    ext[3] = 0;
    for (i = 0; i < 16; i++) {
        if (!((used & (1 << i)) & 0xFFFF)) {
            break;
        }
        ext[0]++;
    }
    if (i >= 16) {
        return -1;
    }
    return 0;
}
