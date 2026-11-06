#include "common.h"
#include "structs/game.h"
#include "globals/game.h"
#include "globals/menu.h"
#include "globals/scene.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU_2/80408AC4_41.CPP, func_8043CDF8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

GameSettings;
GameSettings;
struct CheatCode;
struct CheatCode {
    u8 *code;
    s32 unk4;
    s32 flags;
    s32 sound;
};
struct CheatCode;
typedef struct CheatCode CheatCode;
s32 Sound__Play2D(s32 arg0);
extern CheatCode sCheatCodes_2[];
extern u8 gCheatEntryText[];
extern s32 strlen(u8 *);
s32 EnterCheatMenu_Enter_2(void)
{
    u32 i;
    s32 j;
    s32 match;
    s32 hasSound;
    GameSettings *settings;
    u8 *code;
    sEnterCheatResult = -1;
    for (i = 0; i < 12; i++) {
        code = sCheatCodes_2[i].code;
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
            gEngineApp_m_Settings.unlockedCheats |= sCheatCodes_2[i].flags;
            settings = &gEngineApp_m_Settings;
            do {
                gScene_m_PreviewOptions = settings->unlockedCheats;
            } while (0);
            hasSound = sCheatCodes_2[i].sound != 0;
            sEnterCheatResult = i;
            if (hasSound) {
                Sound__Play2D((s16)sCheatCodes_2[i].sound);
            }
        }
    }
    return 0;
}
