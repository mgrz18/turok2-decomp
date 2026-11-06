/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_004465B4.c, func_004465B4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/game.h"
#include "structs/menu.h"

typedef struct { s16 unk0; u8 pad[0x1E]; void *unk20; } O06A2;
extern s32 CPlayerPool__GetIndex(void *a, s32 b);
extern s32 Controller_MenuLeft(void *a);
extern s32 Controller_MenuRight(void *a);
extern f32 gFrameIncrement;
extern f32 D_800C1348;
s32 NameEntryMenu_UpdateName_2(s32 a, O06A2 *b) {
    s32 n = CPlayerPool__GetIndex(&D_80119870.m_PlayerList, *(s32 *)((u8 *)b + 0x1C));
    f32 v = D_800C1FB0[n].timer + gFrameIncrement;
    D_800C1FB0[n].timer = v;
    if (D_800C1348 <= v) D_800C1FB0[n].timer = v - D_800C1348;
    if (b->unk0 == 1) {
        if (Controller_MenuLeft(b->unk20) && D_800C1FB0[n].cursor > 0) {
            D_800C1FB0[n].cursor--;
            D_800C1FB0[n].timer = 0;
        } else if (Controller_MenuRight(b->unk20) && D_800C1FB0[n].cursor < D_800C1FB0[n].length) {
            D_800C1FB0[n].cursor++;
            D_800C1FB0[n].timer = 0;
        }
    }
    return 0;
}
