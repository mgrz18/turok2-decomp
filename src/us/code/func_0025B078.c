#include "common.h"

/* Picks an actor's death animation from what killed it (weapon, projectile, hit direction). */

typedef struct {
    /* 0x00 */ u8 pad0[0x4F];
    /* 0x4F */ s8 deathExplode;
    /* 0x50 */ s8 deathBlast;
    /* 0x51 */ s8 deathSide;
    /* 0x52 */ s8 deathBack;
    /* 0x53 */ s8 deathBackAlt;
    /* 0x54 */ s8 deathFront;
    /* 0x55 */ u8 pad55;
    /* 0x56 */ s16 gibModel;
} DeathAnims; /* at actor def + 0x14 */

typedef struct {
    /* 0x000 */ u8 pad0[0x996];
    /* 0x996 */ s16 weapon;
} WeaponPlayer;

typedef struct {
    /* 0x000 */ u8 kind;
    /* 0x001 */ u8 pad1;
    /* 0x002 */ u16 type;
    /* 0x004 */ u8 pad4[0xD4 - 0x4];
    /* 0x0D4 */ s32 flags;
    /* 0x0D8 */ u8 padD8[0x1A8 - 0xD8];
    /* 0x1A8 */ WeaponPlayer *owner;
} Source;

typedef struct {
    /* 0x00 */ u8 pad0[0x65];
    /* 0x65 */ u8 direction;
} HitPoint;

typedef struct {
    /* 0x00 */ Source *source;
    /* 0x04 */ u8 pad4[0xC - 0x4];
    /* 0x0C */ s32 flags;
    /* 0x10 */ HitPoint *point;
} HitInfo;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ u8 *def;
} Actor;

extern u8 D_8011AAEF;

extern s32 func_002113A4(void); /* rand */
extern s32 func_00219F74(Actor *actor, s32 model, s32 arg2);

s32 func_0025B078(Actor *actor, s32 unused, HitInfo *hit) {
    DeathAnims *anims;
    Source *src;
    s32 front = 0;
    s32 side = 0;
    s32 back = 0;
    s32 blast = 0;

    anims = (DeathAnims *)(actor->def + 0x14);
    if (D_8011AAEF != 0 && (src = hit->source) != NULL) {
        switch (src->kind) {
            case 2:
                switch (src->type) {
                    case 0xC:
                    case 0x22:
                    case 0x23:
                    case 0x24:
                    case 0x57:
                    case 0x58:
                        blast = 1;
                        break;
                    case 0x2:
                    case 0x6:
                    case 0x7:
                    case 0x8:
                    case 0x9:
                    case 0x1A:
                    case 0x4F:
                    case 0x68:
                        back = side = front = 1;
                        break;
                }
                break;
            case 1:
                if ((src->flags & 0x300000) && src->owner->weapon == 1) {
                    back = side = front = 1;
                }
                break;
        }
    }
    if (blast && anims->deathBlast != -1) {
        return 0x37;
    }
    if (func_002113A4() % 1000 < 500) {
        back = side = front = 0;
    }
    if (hit->point != NULL) {
        switch (hit->point->direction & 3) {
            case 0:
                if (front && anims->deathFront != -1) {
                    return 0x3B;
                }
                break;
            case 1:
                if (side && anims->deathSide != -1) {
                    return 0x38;
                }
                break;
            case 2:
                if (back) {
                    if (!(hit->point->direction & 4)) {
                        if (anims->deathBack != -1) {
                            return 0x39;
                        }
                    } else if (anims->deathBackAlt != -1) {
                        return 0x3A;
                    }
                }
                break;
        }
    }
    if (D_8011AAEF != 0 && (hit->flags & 0x800000) && anims->deathExplode != -1) {
        if (func_00219F74(actor, anims->gibModel + 1, -1) != -1) {
            return 0x35;
        }
        return 0x36;
    }
    return 0x34;
}
