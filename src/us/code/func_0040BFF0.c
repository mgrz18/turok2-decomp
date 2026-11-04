#include "common.h"
#include "structs/game_object.h"
#include "structs/vector.h"

/* When the player (D_800F1CE0) is one of the AI's two targets during anim state 0xE/0xF, knocks it back
 * away from the point (0, 0, 512) (and upward) and deals it 0x500 damage. */

typedef struct {
    /* 0x000 */ u8 kind;
    /* 0x001 */ u8 pad1[0x18 - 0x1];
    /* 0x018 */ CVector3 vel;
    /* 0x024 */ u8 pad24[0xD4 - 0x24];
    /* 0x0D4 */ s32 flags;
    /* 0x0D8 */ u8 padD8[0x140 - 0xD8];
    /* 0x140 */ u8 ai[1];
} Player;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ CVector3 pos;
} Obj;

typedef struct {
    u8 data[0x18];
} DamageEvent;

extern Player *D_800F1CE0;
extern void func_0020EF2C(CVector3 *out, CVector3 *a, CVector3 *b); /* out = a - b */
extern void func_0020EEF8(CVector3 *out, CVector3 *a, CVector3 *b); /* out = a + b */
extern void func_0020EF60(CVector3 *out, CVector3 *in, f32 scale);
extern void func_0020F040(CVector3 *v); /* normalise */
extern void func_00245828(DamageEvent *event, void *source, s32 damage, f32 radius, s32 flags, void *shield);
extern void func_002465C8(Player *target, void *ai, DamageEvent *event);

void func_0040BFF0(Obj *obj, CAI *ai) {
    DamageEvent event;
    CVector3 dir;
    Player *player = D_800F1CE0;

    if (player != NULL && player->kind == 1 && (player->flags & 0x300000) && ai->m_Health != 0 &&
        (ai->m_Mode == 0xE || ai->m_Mode == 0xF) && (player == ai->m_pSightTarget || player == ai->m_pPathTarget)) {
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
