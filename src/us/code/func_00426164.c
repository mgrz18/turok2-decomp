/* WRONG CUT: REAL func_00426164 size 0x278 (splat cut it into func_00426164 + func_004263A0). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/PAK/80403BCC_6.CPP, func_80404958_de, with Turok 2's layouts. */
#include "common.h"

typedef struct PfsPak {
    u8 pad[0x68];
} PfsPak;

extern s32 D_80130C60[];
extern s32 D_80130C70[];
extern PfsPak D_80130C80[];
extern u8 D_800F6750;
extern char D_800C18B0[];
extern char D_800C18C8[];
extern u8 D_800C18E0[];

extern void **func_00204EDC(s32, s32, s32, char *);
extern void func_002053A8(s32);
extern void func_002052D8(s32, void **);
extern void func_00285878(s32);
extern void func_002847E0(void);
extern void func_002858A4(void);
extern void *func_0029E460(void *dst, const void *src, s32 size);
extern s32 func_00266C80(void *a, void *b, s32 size);
extern s32 func_00425BC0(s32 ch, s32 size, void *data, u8 *game_name, s32 *file_no, u8 *ext_name,
                         u8 *company, u8 *code);
extern s32 func_00426064(s32 ch, s32 index);
extern s32 osPfsReadWriteFile(PfsPak *pfs, s32 file_no, u8 flag, s32 offset, s32 size, void *data);

/* Reads note file_no of the pak on channel ch into dst. */
static __inline__ s32 readNote(s32 ch, s32 file_no, void *dst, s32 size) {
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

/* Saves data as a note on the pak on channel ch and reads it back to verify it, up to four times. */
s32 func_00426164(s32 ch, s32 size, void *data, u8 *name, u8 *ext, u8 *code) {
    s32 result;
    s32 file_no;
    void **handle;
    void *buf;
    s32 blocks;
    s32 tries;

    blocks = (size + 0xFF) & ~0xFF;
    if (D_80130C60[ch] != 3) {
        return -2;
    }
    handle = func_00204EDC(0, blocks, 0x23, D_800C18C8);
    buf = *handle;
    for (tries = 0; tries < 4; tries++) {
        result = func_00425BC0(ch, size, data, name, &file_no, ext, D_800C18E0, code);
        if (result != 0) {
            break;
        }
        if (readNote(ch, file_no, buf, blocks) == 0 && func_00266C80(buf, data, size) == 0) {
            break;
        }
        if (func_00426064(ch, file_no) != 0) {
            break;
        }
    }
    func_002053A8(0);
    if (handle != 0) {
        func_002052D8(0, handle);
    }
    return result;
}
