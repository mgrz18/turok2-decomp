/* WRONG CUT: REAL func_0042670C size 0x94 (splat cut it into func_0042670C + func_00426734). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/PAK/80403BCC_8.CPP, func_804050CC_de, with Turok 2's layouts. */
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

/* Counts the empty notes of the pak on channel ch. */
s32 func_0042670C(s32 ch, s32 *count) {
    s32 i;

    if (D_80130C60[ch] != 3) {
        return -2;
    }
    *count = 0;
    if (D_80130C70[ch] == 0) {
        for (i = 0; i < 16; i++) {
            if (D_800C2224[ch].notes[i].file_size == 0) {
                (*count)++;
            }
        }
    }
    return D_80130C70[ch];
}
