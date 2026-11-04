/* WRONG CUT: REAL func_004265B0 size 0x15C (splat cut it into func_004265B0 + func_0042662C + func_004266C0). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/PAK/80403BCC_6.CPP, func_80404958_read, with Turok 2's layouts. */
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

extern char D_800C18B0[];

/* Reads note file_no of the pak on channel ch into dst. */
s32 func_004265B0(s32 ch, s32 file_no, void *dst, s32 size) {
    s32 result;
    s32 blocks;
    void **handle;
    void *buf;

    blocks = (size + 0xFF) & ~0xFF;
    if (D_80130C60[ch] != 3) {
        return -2;
    }
    handle = func_00204EDC(0, blocks, 0x23, D_800C18B0);
    buf = *handle;
    func_00285878(1);
    func_002847E0();
    result = D_80130C70[ch];
    D_800F6750 = 2;
    if (result == 0) {
        if ((result = osPfsReadWriteFile(&D_80130C80[ch], file_no, 0, 0, blocks, buf)) != 0) {
            result = -1;
        }
        if (result == 0) {
            func_0029E460(dst, buf, size);
        }
    }
    func_002858A4();
    func_002053A8(0);
    if (handle != 0) {
        func_002052D8(0, handle);
    }
    return result;
}
