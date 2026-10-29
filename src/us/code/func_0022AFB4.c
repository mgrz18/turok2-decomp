/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/particle/8027A0F4_3.CPP, func_8027B720_de, with Turok 2's layouts. */

#include "common.h"

typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;

typedef struct Box {
    /* 0x00 */ Vec3f min;
    /* 0x0C */ Vec3f max;
} Box;

typedef struct ParticleDef {
    /* 0x00 */ u32 flags;
} ParticleDef;

typedef struct Target {
    /* 0x000 */ u8 kind;
    /* 0x001 */ u8 pad1[0x10 - 1];
    /* 0x010 */ s32 unk10;
    /* 0x014 */ u8 pad14[0xD4 - 0x14];
    /* 0x0D4 */ u32 flags;
    /* 0x0D8 */ u8 padD8[0x140 - 0xD8];
    /* 0x140 */ s32 unk140;
    /* 0x144 */ s32 health;
    /* 0x148 */ u8 pad148[0x174 - 0x148];
    /* 0x174 */ s8 state;
} Target;

typedef struct Player Player;
struct Player {
    /* 0x0000 */ u8 pad0[4];
    /* 0x0004 */ Vec3f pos;
    /* 0x0010 */ u8 pad10[0x54 - 0x10];
    /* 0x0054 */ f32 height;
    /* 0x0058 */ u8 pad58[0x524 - 0x58];
    /* 0x0524 */ s32 health;
    /* 0x0528 */ u8 pad528[0xA08 - 0x528];
    /* 0x0A08 */ s16 mode;
    /* 0x0A0A */ u8 padA0A[0x1320 - 0xA0A];
    /* 0x1320 */ Player *next;
};

typedef struct Particle {
    /* 0x000 */ u8 pad0[2];
    /* 0x002 */ u16 type;
    /* 0x004 */ Vec3f pos;
    /* 0x010 */ s32 unk10;
    /* 0x014 */ u8 pad14[4];
    /* 0x018 */ Vec3f velocity;
    /* 0x024 */ u8 pad24[0x40 - 0x24];
    /* 0x040 */ u32 flags;
    /* 0x044 */ u8 pad44[0xF8 - 0x44];
    /* 0x0F8 */ ParticleDef *def;
    /* 0x0FC */ u8 padFC[0x114 - 0xFC];
    /* 0x114 */ Target *target;
    /* 0x118 */ u8 pad118[4];
    /* 0x11C */ f32 time;
    /* 0x120 */ f32 frame;
    /* 0x124 */ f32 rate;
    /* 0x128 */ s16 life;
    /* 0x12A */ u8 pad12A[2];
    /* 0x12C */ Vec3f size;
    /* 0x138 */ Vec3f growth;
    /* 0x144 */ Vec3f prevPos;
    /* 0x150 */ Vec3f direction;
    /* 0x15C */ Vec3f rot;
    /* 0x168 */ Vec3f rotSpeed;
    /* 0x174 */ u8 pad174[0x190 - 0x174];
    /* 0x190 */ s32 hitFlags;
    /* 0x194 */ u8 pad194;
    /* 0x195 */ u8 hitSurface;
    /* 0x196 */ u8 pad196[2];
    /* 0x198 */ f32 fall;
    /* 0x19C */ u8 pad19C[0x1AC - 0x19C];
    /* 0x1AC */ s8 space;
    /* 0x1AD */ s8 shake;
} Particle;

/* The result of the last collision test. */
typedef struct CollisionResult {
    /* 0x00 */ Target *current;
    /* 0x04 */ u8 pad4[4];
    /* 0x08 */ Vec3f origin;
    /* 0x14 */ u8 pad14[0x88 - 0x14];
    /* 0x88 */ s32 unk88;
    /* 0x8C */ u8 pad8C[4];
    /* 0x90 */ Vec3f unk90;
    /* 0x9C */ s32 unk9C;
    /* 0xA0 */ Vec3f unkA0;
    /* 0xAC */ u8 padAC[4];
    /* 0xB0 */ s32 unkB0;
    /* 0xB4 */ s32 unkB4;
    /* 0xB8 */ Vec3f unkB8;
    /* 0xC4 */ s32 unkC4;
    /* 0xC8 */ Vec3f unkC8;
} CollisionResult;

typedef struct ContactHandler ContactHandler;
struct ContactHandler {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s16 type;
    /* 0x06 */ u8 pad6[0x10 - 6];
    /* 0x10 */ s32 (*func)(Player *player, ContactHandler *handler);
};

typedef struct Listener {
    /* 0x00 */ u8 pad0[0xD8];
    /* 0xD8 */ Vec3f direction;
} Listener;

extern f32 D_800B6D28;                  /* the frame's time step */
extern f32 D_800B6D30;
extern CollisionResult D_800F1CE0;
extern Player *D_80119890;              /* current player */
extern ContactHandler D_800ACE18[4];
extern Listener *D_800F1ADC;
extern u8 D_800F8928[];

s32 func_002113A4(void);
s32 func_00239F00(Particle *p, Vec3f pos, s32 *flags);
f32 func_002671B4(Player *player);
f32 func_002672C8(Player *player);
f32 func_00298470(f32 x);
void func_002324E8(Particle *p);
void func_0022F1C8(void *list, Particle *p);
void func_00232B40(Particle *p);
void func_002323E0(Particle *p);
void func_0022A4C8(Particle *p);
void func_0020EF2C(Vec3f *out, Vec3f *a, Vec3f *b);
void func_0020F040(Vec3f *v);
void func_0022AAE0(Particle *p);
void func_0022BD10(Particle *p);
void func_0022BA88(Particle *p);
void func_0022CA90(Particle *p);
void func_0022C488(Particle *p);
void func_0022BFA0(Particle *p);
void func_0022C228(Particle *p);
void func_00232934(Particle *p);

#define MIN(a, b) ((a) > (b) ? (b) : (a))

/* Updates a particle for one frame: measures its bounding box, kills it or bounces it off its
 * target when its type's rule says so, lets the players it touches react to it, advances its
 * size and spin (dying when a size goes negative) and, when it moves or falls, probes the
 * ground, orients itself to the surface it hit and runs the collision responses its
 * descriptor allows. */
void func_0022AFB4(Particle *p) {
    Box bounds;
    Target *target;
    Player *player;
    ContactHandler *handler;
    f32 extent;
    f32 scaled;
    f32 half;
    f32 top;
    f32 dist;
    f32 step;
    f32 grow;
    s32 die;
    s32 bounce;
    s32 i;
    s8 fade;

    if (p->time < 0.0f) {
        return;
    }
    extent = p->size.z;
    scaled = p->size.y * extent;
    if (!(p->size.x * extent <= scaled)) {
        /* FAKEMATCH: an always-true test (folded only after CSE) keeps CSE from carrying the
         * 0.0f of the time check into the code below, as in the ROM. */
        if (p != NULL) {
            scaled = p->size.x * extent;
        } else {
            scaled = p->size.x * extent;
        }
    }
    if (!(scaled <= extent)) {
        extent = scaled;
    }
    scaled = extent * 1.4142135f;
    half = scaled * 0.5f;
    bounds.min.x = p->pos.x - half;
    bounds.max.x = p->pos.x + half;
    bounds.min.y = p->pos.y - half;
    bounds.max.y = p->pos.y + scaled;
    bounds.min.z = p->pos.z - half;
    bounds.max.z = p->pos.z + half;
    if (p->flags & 0x20000) {
        p->time = 0.0f;
        p->life = p->rate * 32.0f;
        half = p->shake - D_800B6D28 * 6.0f;
        fade = (half < 0.0f) ? 0 : (s32) half;
        p->shake = fade;
    }
    if (p->flags & 0x10000) {
        target = p->target;
        switch (p->type) {
            default:
                p->time = 0.0f;
                p->life = p->rate * 32.0f;
            case 0x2D:
            case 0x4F:
                die = 0;
                break;
            case 0xB:
                die = 0;
                break;
        }
        bounce = die;
        switch (p->type) {
            case 0x12A:
            case 0x132:
                die = 0;
                if (target->health == 0) {
                    bounce = 1;
                }
                break;
            case 2:
            case 0xF:
            case 0x56:
            case 0x126:
                if (p->flags & 0x4000000) {
                    die = 1;
                } else if (!(target->flags & 0x100)) {
                    die = 1;
                } else if (target->flags & 0x300000) {
                    die = (f32) target->health <= 0.0f;
                } else {
                    die = target->unk140 & 0x20;
                }
                bounce = 0;
                break;
            case 0xB:
                die = 0;
                if (target->state != 0x21 || !(target->flags & 0x100)) {
                    bounce = 1;
                }
                break;
            case 4:
                die = 0;
                if (target->state != 0x20 || !(target->flags & 0x100)) {
                    bounce = 1;
                }
                break;
            case 0x2D:
            case 0x4F:
                bounce = !(target->flags & 0x100);
                break;
        }
        if (die || bounce) {
            p->time = 0.0f;
            p->space = -7;
            p->flags = (p->flags & ~0x30000) | 0x40000;
            p->rot.z = (f32) (func_002113A4() % 360) * 0.017453294f;
            p->velocity.y = 0.0f;
            if (p->flags & 0x4000000) {
                p->velocity.x = -p->direction.x;
                p->velocity.z = -p->direction.z;
            } else {
                p->velocity.x = 0.0f;
                p->velocity.z = 0.0f;
            }
            p->unk10 = target->unk10;
            p->hitFlags &= ~0x1000000;
            if (bounce) {
                p->life = 1;
            }
        }
    }
    if ((p->flags & 0x40000) && func_00239F00(p, p->pos, &p->hitFlags) && D_800F1CE0.unkB4 != 0) {
        p->space = -3;
    }
    if (p->flags & 0x60000) {
        for (player = D_80119890; player != NULL; player = player->next) {
            if (player->health != 0 && player->mode != 2) {
                top = player->pos.y + player->height;
                if (top - 20.479999f <= p->pos.y && p->pos.y <= top + func_002671B4(player) + 20.479999f) {
                    dist = func_00298470((player->pos.x - p->pos.x) * (player->pos.x - p->pos.x)
                                         + (player->pos.z - p->pos.z) * (player->pos.z - p->pos.z));
                    if (dist < func_002672C8(player) + 51.199997f) {
                        handler = D_800ACE18;
                        for (i = 3; i != -1; handler++, i--) {
                            if (handler->type == p->type && handler->func(player, handler)) {
                                p->flags |= 0x1000;
                                return;
                            }
                        }
                    }
                }
            }
        }
    }
    p->prevPos = p->pos;
    p->frame += D_800B6D28 * p->rate * (D_800B6D30 * 0.06666667f);
    if (p->def->flags & 0x1000) {
        step = D_800B6D28 * 10.24f;
        p->size.x += step * p->growth.x;
        p->size.y += step * p->growth.y;
        p->size.z += D_800B6D28 * p->growth.z;
        p->size.x = (p->size.x > 4096.0f) ? 4096.0f : p->size.x;
        p->size.y = (p->size.y > 4096.0f) ? 4096.0f : p->size.y;
        p->size.z = (p->size.z > 400.0f) ? 400.0f : p->size.z;
    } else {
        grow = MIN(p->size.x * p->growth.x, 4096.0f);
        p->size.x += (grow - p->size.x) * D_800B6D28;
        grow = MIN(p->size.y * p->growth.y, 4096.0f);
        p->size.y += (grow - p->size.y) * D_800B6D28;
        grow = MIN(p->size.z * p->growth.z, 400.0f);
        p->size.z += (grow - p->size.z) * D_800B6D28;
        if (p->size.x < 0.0f || p->size.y < 0.0f || p->size.z < 0.0f) {
            func_002324E8(p);
            func_0022F1C8(D_800F8928, p);
            func_00232B40(p);
            return;
        }
    }
    p->rot.x += p->rotSpeed.x * D_800B6D28;
    p->rot.y += p->rotSpeed.y * D_800B6D28;
    p->rot.z += p->rotSpeed.z * D_800B6D28;
    D_800F1ADC->direction = p->velocity;
    func_002323E0(p);
    func_0022A4C8(p);
    if ((p->fall == 0.0f || (p->pos.y == 0.0f && p->fall < 0.0f))
        && p->velocity.x * p->velocity.x + p->velocity.y * p->velocity.y + p->velocity.z * p->velocity.z < 0.001f) {
        return;
    }
    if (!(p->flags & 0x72000)) {
        die = func_00239F00(p, p->pos, &p->hitFlags);
        if (p->type == 0x68 && D_800F1CE0.current != NULL) {
            p->target = D_800F1CE0.current;
        }
        if (die) {
            if (D_800F1CE0.current != NULL) {
                func_0020EF2C(&p->direction, &p->pos, &D_800F1CE0.origin);
            } else if (D_800F1CE0.unk88 != 0) {
                func_0020EF2C(&p->direction, &p->pos, &D_800F1CE0.unk90);
            } else if (D_800F1CE0.unk9C != 0) {
                func_0020EF2C(&p->direction, &p->pos, &D_800F1CE0.unkA0);
            } else if (D_800F1CE0.unkB4 != 0) {
                func_0020EF2C(&p->direction, &p->pos, &D_800F1CE0.unkB8);
                if (p->space == -7) {
                    p->space = -3;
                }
            } else if (D_800F1CE0.unkC4 != 0) {
                func_0020EF2C(&p->direction, &p->pos, &D_800F1CE0.unkC8);
            }
            func_0020F040(&p->direction);
        }
        func_0022AAE0(p);
        if (D_800F1CE0.unkC4 != 0) {
            func_0022BD10(p);
        }
        if (p->flags & 0x100) {
            if (D_800F1CE0.unk9C != 0) {
                if (!(p->def->flags & 0x10000000)) {
                    func_0022BA88(p);
                }
                if (p->def->flags & 0x800) {
                    p->hitSurface = 1;
                }
            }
            if (p->flags & 0x100) {
                if (D_800F1CE0.current != NULL) {
                    if ((p->hitFlags & 0x1000000) && D_800F1CE0.current->kind == 0) {
                        func_0022CA90(p);
                    } else {
                        func_0022C488(p);
                    }
                }
                if ((p->flags & 0x100) && D_800F1CE0.unkB0 != 0 && !(p->def->flags & 0x20000000)) {
                    if (D_800F1CE0.unkB0 == 1) {
                        func_0022BFA0(p);
                    } else {
                        func_0022C228(p);
                    }
                }
            }
        }
    }
    if ((p->flags & 0x8100) == 0x100) {
        func_00232934(p);
    }
}
