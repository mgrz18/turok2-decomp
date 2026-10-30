/* WRONG CUT: REAL func_00426C14 size 0x74 (splat cut it into func_00426C14 + func_00426C74). */
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

extern u8 D_80130AA0[];
extern u8 D_800F6778[][4];
extern u8 D_800F6798[];
extern s32 osMotorInit(void *queue, void *pfs, s32 channel);

/* Probes slot ch for a Rumble Pak; returns whether one answered. */
s32 func_00426C14(s32 ch) {
    u8 *cur;
    s32 ready;

    cur = D_80130AA0 + ch * 112;
    if (D_800F6778[ch][3] == 0) {
        cur[0] = osMotorInit(D_800F6798, cur + 8, ch) == 0;
        ready = cur[0];
    } else {
        cur[0] = 0;
        ready = 0;
    }
    return ready;
}
