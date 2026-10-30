#include "common.h"

/* When the player (D_800F1CE0) is one of the AI's two targets during anim state 0xE/0xF, knocks it back
 * away from the point (0, 0, 512) (and upward) and deals it 0x500 damage. */

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    /* 0x000 */ u8 kind;
    /* 0x001 */ u8 pad1[0x18 - 0x1];
    /* 0x018 */ Vec3f vel;
    /* 0x024 */ u8 pad24[0xD4 - 0x24];
    /* 0x0D4 */ s32 flags;
    /* 0x0D8 */ u8 padD8[0x140 - 0xD8];
    /* 0x140 */ u8 ai[1];
} Player;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ Vec3f pos;
} Obj;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u8 pad8[0x34 - 0x8];
    /* 0x34 */ u8 state;
    /* 0x35 */ u8 pad35[0x80 - 0x35];
    /* 0x80 */ Player *target;
    /* 0x84 */ u8 pad84[0x88 - 0x84];
    /* 0x88 */ Player *target2;
} CAI;

typedef struct {
    u8 data[0x18];
} DamageEvent;

extern Player *D_800F1CE0;
extern void func_0020EF2C(Vec3f *out, Vec3f *a, Vec3f *b); /* out = a - b */
extern void func_0020EEF8(Vec3f *out, Vec3f *a, Vec3f *b); /* out = a + b */
extern void func_0020EF60(Vec3f *out, Vec3f *in, f32 scale);
extern void func_0020F040(Vec3f *v); /* normalise */
extern void func_00245828(DamageEvent *event, void *source, s32 damage, f32 radius, s32 flags, void *shield);
extern void func_002465C8(Player *target, void *ai, DamageEvent *event);

void func_0040BFF0(Obj *obj, CAI *ai) {
    DamageEvent event;
    Vec3f dir;
    Player *player = D_800F1CE0;

    if (player != NULL && player->kind == 1 && (player->flags & 0x300000) && ai->unk4 != 0 &&
        (ai->state == 0xE || ai->state == 0xF) && (player == ai->target || player == ai->target2)) {
        dir.x = 0.0f;
        dir.y = 0.0f;
        dir.z = 512.0f;
        func_0020EF2C(&dir, &obj->pos, &dir);
        func_0020F040(&dir);
        func_0020EF60(&dir, &dir, 1536.0f);
        dir.y = 409.59998f;
        func_0020EEF8(&player->vel, &player->vel, &dir);
        func_00245828(&event, obj, 0x500, 102.399994f, 0x100000, NULL);
        func_002465C8(player, player->ai, &event);
    }
}
