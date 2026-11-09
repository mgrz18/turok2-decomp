/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00203B60.c, func_00203B60.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/game_object.h"
#include "globals/scene.h"
#include "protos/scene.h"
#include "protos/game.h"
typedef union B01FH { f32 f; s32 i; s16 h; } B01FH;
typedef union B01W4 { s32 i; f32 f; s16 h[2]; u8 b[4]; } B01W4;
typedef struct B01Info {
                u32 flags;
                s32 unk4;
                s16 unk8;
                s16 unkA;
                B01W4 unkC;
                B01FH unk10;
                s16 unk14;
                s16 unk16;
                s16 unk18;
                u8 unk1A;
                u8 unk1B;
                u8 pad1C[0x8];
                u32 flags24;
                u8 pad28[0x4];
                s32 unk2C;
                s32 unk30;
                B01W4 unk34;
                s32 unk38;
                s32 unk3C;
                B01FH unk40;
                u8 pad44[0x4];
                f32 unk48;
                f32 unk4C;
                u8 pad50[0x1C];
                f32 unk6C;
} B01Info;
void AI_DropPickupsOnce(CGameObjectInstance *actor, CAI *state, s32 kind, s32 a, s32 b);
void Turret_DeathEnter(CGameObjectInstance *actor, CAI *state) {
    B01Info *info = ((B01Info *)((u8 *)(actor->header.m_pIntelligence) + 0x14));
    AI_DropPickupsOnce(actor, state, 4, info->unkC.i, info->unk10.i);
    state->m_pfHurt = 0;
    CScene__SetInstanceSavedBit(&gScene, actor, 1);
    GameObject_FireTriggers(actor, 1, actor);
    actor->m_dwFlags &= ~0x10000;
    if (gScene.m_Mode != 4) {
        actor->m_dwFlags |= 0x8000000;
    }
}
