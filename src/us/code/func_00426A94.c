/* WRONG CUT: REAL func_00426A94 size 0x144 (splat cut it into func_00426A94 + func_00426AF8 + func_00426BA8). */
#include "common.h"

typedef struct PfsPak {
    u8 pad[0x68];
} PfsPak;

/* One note of the cached Controller Pak directory (an OSPfsState). */
typedef struct PakNote {
    /* 0x00 */ u32 file_size;
    /* 0x04 */ u32 game_code;
    /* 0x08 */ u16 company_code;
    /* 0x0A */ u8 ext_name[4];
    /* 0x0E */ u8 game_name[16];
    /* 0x1E */ u8 pad1E[2];
} PakNote;

typedef struct PakDirectory {
    /* 0x000 */ s32 free;
    /* 0x004 */ PakNote notes[16];
} PakDirectory; /* 0x204 */

extern s32 D_80130C60[];
extern s32 D_80130C70[];
extern PfsPak D_80130C80[];
extern PakDirectory *D_800C2224;
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
extern s32 osPfsReadWriteFile(PfsPak *pfs, s32 file_no, u8 flag, s32 offset, s32 size, void *data);

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
