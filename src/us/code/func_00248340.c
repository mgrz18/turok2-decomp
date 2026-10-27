#include "common.h"

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

typedef struct PlayerOptions {
    /* 0x00 */ u8 pad0[0x3C];
    /* 0x3C */ u8 dead;
} PlayerOptions;

/* The player fields this resets. */
typedef struct Player {
    /* 0x0000 */ u8 pad0[0x4];
    /* 0x0004 */ Vec3 pos;
    /* 0x0010 */ u8 pad10[0x514 - 0x10];
    /* 0x0514 */ s32 index;
    /* 0x0518 */ PlayerOptions *options;
    /* 0x051C */ u8 pad51C[0xA10 - 0x51C];
    /* 0x0A10 */ s32 unkA10;
    /* 0x0A14 */ u8 padA14[0xA20 - 0xA14];
    /* 0x0A20 */ s32 unkA20;
    /* 0x0A24 */ s32 unkA24;
    /* 0x0A28 */ u8 padA28[0xA40 - 0xA28];
    /* 0x0A40 */ u8 controls[0x38];       /* set up by func_002470E8 */
    /* 0x0A78 */ s32 unkA78;
    /* 0x0A7C */ s32 unkA7C;
    /* 0x0A80 */ s32 unkA80;
    /* 0x0A84 */ s32 unkA84;
    /* 0x0A88 */ s32 unkA88;
    /* 0x0A8C */ s32 unkA8C;
    /* 0x0A90 */ s32 unkA90;
    /* 0x0A94 */ s32 unkA94;
    /* 0x0A98 */ u8 padA98[0xA9C - 0xA98];
    /* 0x0A9C */ s32 unkA9C;
    /* 0x0AA0 */ Vec3 unkAA0;
    /* 0x0AAC */ f32 unkAAC;
    /* 0x0AB0 */ s32 unkAB0;
    /* 0x0AB4 */ s32 unkAB4;
    /* 0x0AB8 */ s32 unkAB8;
    /* 0x0ABC */ s32 unkABC;
    /* 0x0AC0 */ s32 unkAC0;
    /* 0x0AC4 */ s32 unkAC4;
    /* 0x0AC8 */ s32 unkAC8;
    /* 0x0ACC */ s32 unkACC;
    /* 0x0AD0 */ s32 unkAD0;
    /* 0x0AD4 */ s32 unkAD4;
    /* 0x0AD8 */ s32 unkAD8;
    /* 0x0ADC */ s32 unkADC;
    /* 0x0AE0 */ s32 unkAE0;
    /* 0x0AE4 */ f32 unkAE4;
    /* 0x0AE8 */ s32 unkAE8;
    /* 0x0AEC */ Vec3 unkAEC;
    /* 0x0AF8 */ f32 unkAF8;
    /* 0x0AFC */ s32 unkAFC;
    /* 0x0B00 */ s32 unkB00;
    /* 0x0B04 */ u8 padB04[0xB24 - 0xB04];
    /* 0x0B24 */ s32 unkB24;
    /* 0x0B28 */ f32 unkB28;
    /* 0x0B2C */ s32 unkB2C;
    /* 0x0B30 */ s32 unkB30;
    /* 0x0B34 */ s32 unkB34;
    /* 0x0B38 */ s32 unkB38;
    /* 0x0B3C */ s32 unkB3C;
    /* 0x0B40 */ s32 unkB40;
    /* 0x0B44 */ s32 unkB44;
    /* 0x0B48 */ s32 unkB48;
    /* 0x0B4C */ s32 unkB4C;
    /* 0x0B50 */ s32 unkB50;
    /* 0x0B54 */ s32 unkB54;
    /* 0x0B58 */ s32 unkB58;
    /* 0x0B5C */ s32 unkB5C;
    /* 0x0B60 */ u8 padB60[0xB64 - 0xB60];
    /* 0x0B64 */ f32 unkB64;
    /* 0x0B68 */ f32 unkB68;
    /* 0x0B6C */ f32 unkB6C;
    /* 0x0B70 */ f32 unkB70;
    /* 0x0B74 */ f32 unkB74;
    /* 0x0B78 */ f32 unkB78;
    /* 0x0B7C */ Vec3 unkB7C;
    /* 0x0B88 */ f32 unkB88;
    /* 0x0B8C */ s32 unkB8C;
    /* 0x0B90 */ s32 unkB90;
    /* 0x0B94 */ s32 unkB94;
    /* 0x0B98 */ u8 padB98[0xBB0 - 0xB98];
    /* 0x0BB0 */ s32 unkBB0;
    /* 0x0BB4 */ u8 padBB4[0xBDC - 0xBB4];
    /* 0x0BDC */ f32 unkBDC;
    /* 0x0BE0 */ f32 unkBE0;
    /* 0x0BE4 */ f32 unkBE4;
    /* 0x0BE8 */ u8 padBE8[0xBEC - 0xBE8];
    /* 0x0BEC */ s32 unkBEC;
    /* 0x0BF0 */ u8 padBF0[0x1284 - 0xBF0];
    /* 0x1284 */ s32 unk1284;
} Player;

extern u8 D_800F5EC0[][0x224];      /* controller settings, one per player */

void func_002470E8(void *controls, void *settings);
f32 func_00253CFC(Player *player);
void func_0024E5F0(Player *player, Player *arg1, s32 arg2);

static __inline__ void setVec3(Vec3 *v, f32 x, f32 y, f32 z) {
    v->z = z;
    v->y = y;
    v->x = x;
}

/* Resets a player's movement, camera and crosshair state. */
void func_00248340(Player *p) {
    f32 zero;

    func_002470E8(p->controls, D_800F5EC0[p->index]);
    p->unkA78 = 0;
    p->unkA7C = 0;
    p->unkA80 = 0;
    p->unkA84 = 0;
    p->unkA88 = 1;
    p->unkA8C = 0;
    p->unkA90 = 0;
    p->unkA94 = 0;
    p->unkA9C = 0;
    p->unkAFC = 0;
    p->unkB00 = 0;
    p->unk1284 = 0;
    p->unkAB0 = 0;
    p->unkAB4 = 0;
    p->unkAB8 = 0;
    p->unkABC = 0;
    p->unkAC8 = 0;
    p->unkACC = 0;
    p->unkAD0 = 0;
    p->unkAD4 = 0;
    p->unkAD8 = 0;
    p->unkADC = 0;
    p->unkAE0 = 0;
    p->unkAE4 = func_00253CFC(p);
    p->unkAE8 = 0;
    setVec3(&p->unkAEC, 0.0f, 0.0f, 0.0f);
    p->unkB24 = 0;
    p->unkAF8 = 1.0f;
    p->unkB28 = 1.0f;
    p->unkAA0 = p->pos;
    p->unkB54 = 0;
    zero = *(f32 *) &p->unkB54;   /* read back as a float: the ROM reloads it */
    p->unkA10 = 0;
    p->unkA20 = 0;
    p->unkA24 = 0;
    p->unkAC0 = 0;
    p->unkAC4 = 0;
    p->unkB90 = 0;
    p->unkB94 = 0;
    p->unkB8C = 0;
    p->unkB2C = 0;
    p->unkB30 = 0;
    p->unkB34 = 0;
    p->unkB38 = 0;
    p->unkB3C = -1;
    p->unkB40 = 0;
    p->unkB44 = 0;
    p->unkB48 = 0;
    p->unkB4C = 0;
    p->unkB50 = 0;
    setVec3(&p->unkB7C, zero, zero, zero);
    p->unkB88 = 1.0f;
    p->unkB58 = 0;
    p->unkB5C = -1;
    p->unkB64 = zero;
    p->unkB68 = zero;
    p->unkB6C = zero;
    p->unkB70 = zero;
    p->unkB74 = zero;
    p->unkB78 = zero;
    func_0024E5F0(p, p, 2);
    p->unkBB0 = 0;
    p->unkBDC = zero;
    p->unkBE0 = zero;
    p->unkBE4 = zero;
    p->unkBEC = 0;
    p->options->dead = 0;
    p->unkAAC = zero;
}
