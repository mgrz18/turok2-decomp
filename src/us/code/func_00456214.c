/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_004465B4.c, func_004465B4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/game.h"
#include "structs/menu.h"
#include "globals/game.h"
#include "globals/engine.h"
#include "protos/player.h"

typedef struct { s16 unk0; u8 pad[0x1E]; void *unk20; } O06A2;
extern s32 Controller_MenuLeft(void *a);
extern s32 Controller_MenuRight(void *a);
extern f32 D_800C1348;
s32 NameEntryMenu_UpdateName_2(s32 a, O06A2 *b) {
    s32 n = CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, *(s32 *)((u8 *)b + 0x1C));
    f32 v = gNameEntries[n].timer + gFrameIncrement;
    gNameEntries[n].timer = v;
    if (D_800C1348 <= v) gNameEntries[n].timer = v - D_800C1348;
    if (b->unk0 == 1) {
        if (Controller_MenuLeft(b->unk20) && gNameEntries[n].cursor > 0) {
            gNameEntries[n].cursor--;
            gNameEntries[n].timer = 0;
        } else if (Controller_MenuRight(b->unk20) && gNameEntries[n].cursor < gNameEntries[n].length) {
            gNameEntries[n].cursor++;
            gNameEntries[n].timer = 0;
        }
    }
    return 0;
}
