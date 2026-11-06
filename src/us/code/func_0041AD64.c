/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043CDF8_de.c, func_8043CDF8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "structs/game.h"
#include "globals/game.h"

extern int gScene_m_PreviewOptions;
extern CheatCode sCheatCodes[];
extern u8 gCheatEntryText[];
extern s32 sEnterCheatResult;
extern s32 strlen(u8 *);
extern void Sound__Play2D(s32);
s32 EnterCheatMenu_Enter(void)
{
    u32 i;
    s32 j;
    s32 match;
    s32 hasSound;
    GameSettings *settings;
    u8 *code;
    sEnterCheatResult = -1;
    for (i = 0; i < 12; i++) {
        code = sCheatCodes[i].code;
        j = 0;
        if (strlen(code) != strlen(gCheatEntryText)) {
            match = 0;
        } else {
            for (;;) {
                if (j < strlen(code)) {
                    if ((code[j] ^ j) != gCheatEntryText[j]) {
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
            gEngineApp_m_Settings.unlockedCheats |= sCheatCodes[i].flags;
            settings = &gEngineApp_m_Settings;
            do {
                gScene_m_PreviewOptions = settings->unlockedCheats;
            } while (0);
            hasSound = sCheatCodes[i].sound != 0;
            sEnterCheatResult = i;
            if (hasSound) {
                Sound__Play2D((s16)sCheatCodes[i].sound);
            }
        }
    }
    return 0;
}
