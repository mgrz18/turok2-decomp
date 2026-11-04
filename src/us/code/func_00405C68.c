#include "common.h"
#include "structs/game_object.h"

/* A spawn slot of the boss module (0x64 bytes). */
typedef struct {
    /* 0x00 */ u8 data[0x64];
} BossSlot;

typedef struct {
    /* 0x0 */ s32 mode;
    /* 0x4 */ s8 unk4;
    /* 0x5 */ s8 unk5;
} BossMode;

/* Boss module state (0x8012F608 on). The ROM also reaches its start through
 * D_8012F9DC (the mode block at 0x3D4), so MODULE rebuilds it from that symbol. */
typedef struct {
    /* 0x000 */ s32 stage;
    /* 0x004 */ u8 pad4[0x18 - 0x4];
    /* 0x018 */ s32 spawnStep;
    /* 0x01C */ u8 pad1C[0x24 - 0x1C];
    /* 0x024 */ BossSlot slots[7];
    /* 0x2E0 */ u8 pad2E0[0x3D0 - 0x2E0];
    /* 0x3D0 */ s32 unk3D0;
    /* 0x3D4 */ BossMode mode;
} BossModule;

s32 func_00236314(void *, s32 *);           /* extern */
extern s32 D_00431B54;
extern f32 D_800C0488;
extern f32 D_800C048C;
extern f32 D_800C0490;
extern BossModule D_8012F608;
extern BossMode D_8012F9DC;

#define MODULE ((BossModule *) ((u8 *) &D_8012F9DC - 0x3D4))

void func_00405C68(CGameObjectInstance *arg0) {
    BossModule *temp_s2;

    if (D_8012F9DC.mode == 2) {
        D_8012F9DC.unk4 = 1;
        D_8012F9DC.unk5 = 1;
        MODULE->unk3D0 = 2;
    }
    temp_s2 = MODULE;
    if (MODULE->stage == 1) {
        arg0->m_AI.m_AnimSpeed = (f32) D_800C0488;
    }
    if (MODULE->stage == 2) {
        arg0->m_AI.m_AnimSpeed = (f32) D_800C048C;
    }
    if (MODULE->stage == 3) {
        arg0->m_AI.m_AnimSpeed = (f32) D_800C0490;
    }
    if ((MODULE->stage == 1) && (temp_s2->spawnStep == 0)) {
        func_00236314(&MODULE->slots[3], &D_00431B54);
        func_00236314(&MODULE->slots[4], &D_00431B54);
        func_00236314(&MODULE->slots[6], &D_00431B54);
        func_00236314(&MODULE->slots[5], &D_00431B54);
        temp_s2->spawnStep = (s32) (temp_s2->spawnStep + 1);
    }
    if ((D_8012F608.stage == 2) && (D_8012F608.spawnStep == 1)) {
        func_00236314(&D_8012F608.slots[0], &D_00431B54);
        func_00236314(&D_8012F608.slots[2], &D_00431B54);
        D_8012F608.spawnStep = (s32) (D_8012F608.spawnStep + 1);
    }
    if ((D_8012F608.stage == 3) && (D_8012F608.spawnStep == 2)) {
        func_00236314(&D_8012F608.slots[1], &D_00431B54);
        D_8012F608.spawnStep = (s32) (D_8012F608.spawnStep + 1);
    }
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2000);
}
