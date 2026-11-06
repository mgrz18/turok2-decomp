#include "structs/vector.h"
#include "structs/game_object.h"
#include "globals/game.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00205F18.c, func_00205F18.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef struct ActorLink {
               u8 pad0[8];
               void (*cb)();
} ActorLink;
typedef union B01FH { f32 f; s32 i; s16 h; } B01FH;
typedef struct B01Vec4 { f32 x, y, z, w; } B01Vec4;
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
typedef struct B01Seg {
                u8 b[0x18];
} B01Seg;
f32 Random_Blend(f32 a, f32 b);
void CAI__SetMode(CGameObjectInstance *actor, CAI *state, s32 event);
void Generator_Advance(CGameObjectInstance *actor, CAI *state) {
    CGameObjectInstance *a2;
    B01Info *new_var;
    B01Info *info;
    a2 = actor;
    new_var = (B01Info *)(((u8 *)a2->header.m_pIntelligence) + 0x14);
    if (!(a2->header.m_pIntelligence->m_dwTypeFlags & 0x10) || (gEngineApp_m_Settings.multiplayer != 0 && gEngineApp_m_Settings.playerCount == 1)) {
        if (state) {
            state->m_ConditionTimer = Random_Blend(45.0f, 60.0f);
            info = new_var;
            state->m_VarS32[0] = 0;
            state->m_VarS32[1] = info->unk8;
        } else {
            info = new_var;
            state->m_ConditionTimer = Random_Blend(45.0f, 60.0f);
            state->m_VarS32[0] = 0;
            state->m_VarS32[1] = info->unk8;
        }
        if (a2->m_nTypeFlag == 0x644 || (info->flags & 8)) {
            a2->m_dwFlags &= ~0x2000;
        }
        CAI__SetMode(a2, state, 0);
        if ((info->flags & 0x10) && gEngineApp_m_Settings.playerCount != 1) {
            goto clear;
        }
    } else {
clear:
        a2->m_dwFlags &= ~0x100;
    }
}
