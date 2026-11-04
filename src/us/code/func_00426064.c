/* WRONG CUT: REAL func_00426064 size 0x100 (splat cut it into func_00426064 + func_00426084). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/PAK/80403BCC_6.CPP, func_80404858_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/pak.h"
extern u8 D_800F6750;

extern void func_00285878(s32);
extern void func_002847E0(void);
extern void func_002858A4(void);
extern s32 func_004258E8(s32 ch);
extern s32 osPfsDeleteFile(OSPfs *pfs, u16 company_code, u32 game_code, u8 *game_name, u8 *ext_name);

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
