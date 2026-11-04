#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x3E];
    /* 0x3E */ u8 field3E;
} PlayerOptions;

typedef struct {
    /* 0x000 */ CGameObjectInstance actor;
    /* 0x288 */ u8 pad288[0x518 - 0x288];
    /* 0x518 */ PlayerOptions *options;
    /* 0x51C */ u8 pad51C[0x996 - 0x51C];
    /* 0x996 */ s16 weapon;
    /* 0x998 */ u8 pad998[0xA64 - 0x998];
    /* 0xA64 */ u32 buttons;
    /* 0xA68 */ u8 padA68[0xB2C - 0xA68];
    /* 0xB2C */ s32 unkB2C;
    /* 0xB30 */ s32 unkB30;
} Player;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 flags;
    /* 0x18 */ f32 unk18;
} WeaponDef;

s32 func_00243414(s32, s32, s32);
s32 func_0024E700(s32, s32);
s32 func_00257EA0(s32, s32);

extern f32 D_800A7298;
extern WeaponDef *D_800AF618[];

void func_00258C50(CGameObjectInstance *obj, CAI *ai) {
    s16 temp_a1;
    Player *temp_s1;

    temp_s1 = (Player *) obj->m_AI.m_pParent;
    if (ai->m_AnimRepeatedCount != 0) {
        if ((*(f32 *) &ai->m_VarBuffer[3] <= 0.0f) && (temp_a1 = temp_s1->weapon, ((D_800AF618[temp_a1]->flags & 4) != 0)) && (func_0024E700((s32) temp_s1, (s32) temp_a1) != 0) && (func_00257EA0((s32) obj, (s32) ai) != 0)) {
            if ((temp_s1->buttons & 0x2000) && (temp_s1->options->field3E == 0)) {
                ai->m_AnimRepeatedCount = 0;
                ai->m_ModeLastFrame = -1;
                ai->m_Mode = -1;
                func_00243414((s32) obj, (s32) ai, 4);
                *(f32 *) &ai->m_VarBuffer[3] = (f32) (D_800AF618[((Player *) obj->m_AI.m_pParent)->weapon]->unk18 * D_800A7298);
            } else {
                goto block_9;
            }
        } else {
block_9:
            func_00243414((s32) obj, (s32) ai, 2);
        }
        temp_s1->unkB2C = 1;
        temp_s1->unkB30 = 0;
    }
}
