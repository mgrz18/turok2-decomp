/* WRONG CUT: REAL func_00426064 size 0x100 (splat cut it into func_00426064 + func_00426084). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/PAK/80403BCC_6.CPP, func_80404858_de, with Turok 2's layouts. */
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
extern u8 D_800F6750;

extern void func_00285878(s32);
extern void func_002847E0(void);
extern void func_002858A4(void);
extern s32 func_004258E8(s32 ch);
extern s32 osPfsDeleteFile(PfsPak *pfs, u16 company_code, u32 game_code, u8 *game_name, u8 *ext_name);

s32 func_00426064(s32 ch, s32 index) {
    s32 result;

    if (D_80130C60[ch] != 3) {
        return -2;
    }
    func_00285878(1);
    func_002847E0();
    result = D_80130C70[ch];
    D_800F6750 = 2;
    if (result == 0) {
        result = osPfsDeleteFile(&D_80130C80[ch], D_800C2224[ch].notes[index].company_code,
                                 D_800C2224[ch].notes[index].game_code,
                                 D_800C2224[ch].notes[index].game_name,
                                 D_800C2224[ch].notes[index].ext_name);
        if (result != 0) {
            result = -1;
        }
        func_004258E8(ch);
    }
    func_002858A4();
    return result;
}
