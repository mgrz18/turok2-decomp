#include "common.h"
#include "structs/game.h"

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

typedef struct ActorDef {
    s32 type;          /* 0x00 */
    char pad4[0x12 - 0x4];
    s8 level;          /* 0x12 */
    char pad13[0x18 - 0x13];
    union {
        u32 u;
        s16 s;
    } health;          /* 0x18 */
    char pad1C[0x40 - 0x1C];
    u8 unk40;          /* 0x40 */
    u8 unk41;          /* 0x41 */
} ActorDef;

typedef struct Dest {
    char pad0[4];
    s32 health;        /* 0x04 */
    s32 maxHealth;     /* 0x08 */
    s32 handle;        /* 0x0C */
    s8 unk10;          /* 0x10 */
    s8 level;          /* 0x11 */
    char pad12[0xC8 - 0x12];
    u8 unkC8;          /* 0xC8 */
} Dest;

extern s32 D_8011AAE0;  /* difficulty: 8, 16 or 32 */

/* Initial health and level of an actor from its definition, scaled by difficulty. */
void func_002427E0(Dest *dst, ActorDef *def, Vec3 pos, void *actor) {
    GameSettings *settings;
    s32 level;
    f32 health;
    f32 a;
    f32 b;
    f32 t;

    level = 0;
    a = 100.0f;
    b = 128.0f;
    dst->handle = -1;
    health = a;
    if (def != NULL) {
        settings = &D_8011AAD8;
        level = def->level;
        if (settings->multiplayer != 0 && def->type != 1 && def->type != 11) {
            if (settings->match.tag != 0) {
                level = settings->tagSkill;
            } else {
                level = settings->dmSkill;
            }
        }
        switch (def->type) {
            case 1:
                health = def->health.u;
                a = def->unk41;
                b = def->unk40;
                break;
            case 4:
                health = def->health.u;
                break;
            case 7:
                health = def->health.s;
                break;
        }
        if (def->type == 1 || def->type == 4) {
            switch (D_8011AAE0) {
                case 8:
                    t = health * 0.5f;
                    b = 0.0f;
                    if (t < 1.0f) {
                        t = 1.0f;
                    }
                    health = t;
                    break;
                case 32:
                    b = 255.0f;
                    health *= 1.25f;
                    break;
            }
        }
    }
    dst->health = health;
    dst->unk10 = a;
    dst->level = level;
    dst->unkC8 = (s32)b;
    dst->health <<= 8;
    dst->maxHealth = dst->health;
}
