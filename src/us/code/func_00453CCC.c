/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU_2/80408AC4_41.CPP, func_8043CDF8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
struct IntPair;
struct IntPair {
    int field_0;
    int field_4;
};
struct IntPair;
struct CheatCode;
struct CheatCode {
    u8 *code;
    s32 unk4;
    s32 flags;
    s32 sound;
};
extern int D_80110060;
struct CheatCode;
typedef struct CheatCode CheatCode;
s32 func_00275624(s32 arg0);
extern CheatCode D_00439690[];
extern u8 D_800C1F8C[];
extern struct IntPair D_8011AAD8;
extern s32 D_80130920;
extern s32 func_0029E4C0(u8 *);
s32 func_00453CCC(void)
{
    u32 i;
    s32 j;
    s32 match;
    s32 hasSound;
    struct IntPair *settings;
    u8 *code;
    D_80130920 = -1;
    for (i = 0; i < 12; i++) {
        code = D_00439690[i].code;
        j = 0;
        if (func_0029E4C0(code) != func_0029E4C0(D_800C1F8C)) {
            match = 0;
        } else {
            for (;;) {
                if (j < func_0029E4C0(code)) {
                    if ((code[j] ^ j) != D_800C1F8C[j]) {
                        match = 0;
                        break;
                    }
                    j++;
                } else {
                    match = 1;
                    break;
                }
            }
        }
        if (match) {
            D_8011AAD8.field_4 |= D_00439690[i].flags;
            settings = &D_8011AAD8;
            do {
                D_80110060 = settings->field_4;
            } while (0);
            hasSound = D_00439690[i].sound != 0;
            D_80130920 = i;
            if (hasSound) {
                func_00275624((s16)D_00439690[i].sound);
            }
        }
    }
    return 0;
}
