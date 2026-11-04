/* D_80110060 is declared as an array so its store keeps its place after the save buffer reads (a scalar is assumed not to alias them and is hoisted). */
#include "common.h"
#include "structs/player.h"

typedef struct {
    /* 0x000 */ s32 data[0x184 / 4];
} SetupBlob;

typedef struct {
    /* 0x000 */ s32 size;
    /* 0x004 */ s32 extra;
    /* 0x008 */ f32 match[4];
    /* 0x018 */ CPlayerLoadSaveData player;
    /* 0x500 */ SetupBlob setup;
    /* 0x684 */ u8 text[4];
} SaveBuffer;

typedef struct {
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ s32 textLen;
} Level;

typedef struct {
    /* 0x000 */ s32 unk0;
    /* 0x004 */ s32 warpId;
    /* 0x008 */ u8 pad8[0x180 - 0x8];
    /* 0x180 */ u8 unk180;
    /* 0x181 */ u8 pad181[0x1D8 - 0x181];
    /* 0x1D8 */ f32 match[4];
} Game;

extern SaveBuffer *D_800C201C;
extern Level D_800F7078;
extern Game D_8011AAD8;
extern s32 D_80110060[];
extern u8 D_800EBAC0[];

void func_00421528(CPlayer *p) {
    Level *level = &D_800F7078;
    u8 keep;
    s32 i;
    u8 *src;
    u8 *dst;

    keep = D_8011AAD8.unk180;
    p->m_Data = D_800C201C->player;
    *(SetupBlob *)&D_8011AAD8 = D_800C201C->setup;
    D_8011AAD8.match[0] = D_800C201C->match[0];
    D_8011AAD8.match[1] = D_800C201C->match[1];
    D_8011AAD8.match[2] = D_800C201C->match[2];
    D_8011AAD8.match[3] = D_800C201C->match[3];
    D_80110060[0] = D_8011AAD8.warpId;
    D_8011AAD8.unk180 = keep;
    src = D_800C201C->text;
    dst = D_800EBAC0;
    for (i = 0; i < level->textLen; i++) {
        *dst++ = *src++;
    }
}
