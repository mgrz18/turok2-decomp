/* NON-MATCHING: 5 of 151 words differ: the ROM copies the aimed actor to $v0 before checking kind == 1 against a register holding 1 (bne); here lbu+xori */
#include "common.h"

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

/* The creature to mount (an AI actor). */
typedef struct {
    /* 0x000 */ u8 kind;
    /* 0x001 */ u8 pad1[3];
    /* 0x004 */ Vec3 pos;
    /* 0x010 */ u8 pad10[0x50 - 0x10];
    /* 0x050 */ f32 yaw;
    /* 0x054 */ u8 pad54[0xB8 - 0x54];
    /* 0x0B8 */ u16 type;
    /* 0x0BA */ u8 padBA[0x140 - 0xBA];
    /* 0x140 */ u8 ai[4];
} Mount;

typedef struct {
    /* 0x00 */ u8 pad0[0x50];
    /* 0x50 */ f32 yaw;
} Body;

typedef struct Player {
    /* 0x0000 */ u8 pad0[0x4];
    /* 0x0004 */ Vec3 pos;
    /* 0x0010 */ u8 pad10[0x514 - 0x10];
    /* 0x0514 */ s32 index;
    /* 0x0518 */ u8 pad518[0xA18 - 0x518];
    /* 0x0A18 */ u32 held;
    /* 0x0A1C */ u8 padA1C[0xA40 - 0xA1C];
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
    /* 0x0B2C */ u8 padB2C[0xBB0 - 0xB2C];
    /* 0x0BB0 */ Mount *mount;
    /* 0x0BB4 */ s32 mountType;
    /* 0x0BB8 */ Vec3 seat;
    /* 0x0BC4 */ u8 padBC4[0x1284 - 0xBC4];
    /* 0x1284 */ s32 unk1284;
} Player;

extern Mount *D_800F1CE0;           /* what the player is aiming at */
extern Vec3 D_800ADCBC;             /* rider seat */
extern u8 D_800F5EC0[][0x224];      /* controller settings, one per player */

f32 func_00246310(Body *body, Vec3 point);   /* angle to a point */
void func_0020EF60(Vec3 *out, Vec3 *in, f32 s);
void func_002470E8(void *controls, void *settings);
f32 func_00253CFC(Player *player);
void func_00206068(s32);
void func_0025EB80(Mount *mount, void *ai, Player *player);
s32 func_0024E5F0(Player *player, Body *body, s32 arg2);

#define ABS(x) ((x) < 0.0f ? -(x) : (x))

static __inline__ s32 isActor(Mount *m) {
    return m->kind == 1;
}

static __inline__ void setVec3(Vec3 *v, f32 x, f32 y, f32 z) {
    v->z = z;
    v->y = y;
    v->x = x;
}

/* Mounts the creature of type 0x136 in front, if within 45 degrees of the
 * heading and the player is not holding 0x8000: the player takes its seat
 * and heading, and the movement state is reset. */
s32 func_0024EAF8(Player *p, Body *body) {
    Mount *aim;
    Mount *mount;

    if (p->mount == NULL && !(p->held & 0x8000)) {
        aim = D_800F1CE0;
        if (aim != NULL && isActor(aim)) {
            mount = aim;
            if (mount->type == 0x136 && ABS(func_00246310(body, mount->pos)) < 0.785398245f) {
                p->mount = mount;
                p->mountType = 0x136;
                p->seat = D_800ADCBC;
                func_0020EF60(&p->seat, &p->seat, 0.03f);
                body->yaw = mount->yaw;
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
                func_00206068(8);
                func_0025EB80(mount, mount->ai, p);
                return func_0024E5F0(p, body, 0x21);
            }
        }
    }
    return 0;
}
