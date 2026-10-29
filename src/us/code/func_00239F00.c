#include "common.h"
#include "structs/game_object.h"

#define MAX(a, b) ((a) < (b) ? (b) : (a))

/* A navigation region (see func_002399F8). */
typedef struct Region {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ u16 flags;               /* 1: under water */
} Region;

typedef struct RegionInfo {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ f32 waterY;
    /* 0x24 */ u8 pad24[0x20];
    /* 0x44 */ s32 flags;
} RegionInfo;

/* What moves: the start of any instance (actor, particle, pickup...). */
typedef struct Mover {
    /* 0x000 */ u8 type;
    /* 0x001 */ u8 pad1[3];
    /* 0x004 */ CVector3 pos;
    /* 0x010 */ Region *region;
    /* 0x014 */ u8 pad14[4];
    /* 0x018 */ CVector3 vel;
    /* 0x024 */ u8 pad24[0xC];
    /* 0x030 */ s32 attached;
    /* 0x034 */ u8 pad34[0x10C];
    /* 0x140 */ s32 aiFlags;
} Mover;

/* How an instance collides and moves. */
typedef struct CollisionDef {
    /* 0x00 */ s32 flags;
    /* 0x04 */ s8 unk4;
    /* 0x05 */ s8 unk5;
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ f32 gravity;
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ f32 friction;
    /* 0x14 */ f32 drag;
    /* 0x18 */ f32 waterDrag;
} CollisionDef;

/* One collision test's result (cleared by func_0023C0FC, copied out by
 * func_0023C120). */
typedef struct Hit {
    /* 0x00 */ f32 fraction;            /* of the move done */
    /* 0x04 */ s32 type;                /* 0 none, 1 slide, 2 and 3 hit */
    /* 0x08 */ CVector3 vel;
    /* 0x14 */ CVector3 slideTo;
    /* 0x20 */ CVector3 pos;
    /* 0x2C */ CVector3 *posOut;
    /* 0x30 */ s32 *unk30;
    /* 0x34 */ s32 *unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 *unk3C;
    /* 0x40 */ s32 unk40;
} Hit;

/* A segment swept from an instance's position to end (see func_0023BE24). */
typedef struct Sweep {
    /* 0x00 */ Mover *obj;
    /* 0x04 */ CollisionDef *def;
    /* 0x08 */ Hit hit;
    /* 0x4C */ CVector3 end;
    /* 0x58 */ CVector3 delta;
    /* 0x64 */ CVector3 unk64;
    /* 0x70 */ CVector3 unk70;
    /* 0x7C */ f32 unk7C;
    /* 0x80 */ f32 unk80;
    /* 0x84 */ s32 unk84;
    /* 0x88 */ s32 unk88;
    /* 0x8C */ s32 more;                /* another pass follows */
    /* 0x90 */ s32 physics;
    /* 0x94 */ s32 onGround;
    /* 0x98 */ s32 checkRegionInfo;
    /* 0x9C */ s32 stop;
    /* 0xA0 */ s32 unkA0;
    /* 0xA4 */ u8 padA4[4];
} Sweep;

typedef struct CollisionOut {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ CVector3 unk8;
    /* 0x14 */ u8 pad14[0x88];
    /* 0x9C */ s32 unk9C;
    /* 0xA0 */ CVector3 unkA0;
    /* 0xAC */ u8 padAC[0x2C];
    /* 0xD8 */ CVector3 vel;
} CollisionOut;

extern s32 D_800F7078[];
extern f32 D_800B6D28;                  /* frame time, in 15ths of a second */
extern CollisionOut *D_800F1ADC;
extern void (*D_800AD190[])(Sweep *);   /* the collision tests, NULL-ended */
extern CollisionDef D_800F1E10;
extern CollisionDef D_800F1CC0;
extern CollisionDef D_800F1AE0;
extern CollisionDef D_800F1C60;

RegionInfo *func_0022425C(s32 *list, Region *region);
void func_00241894(CollisionDef *def);
void func_0023BE24(Sweep *sweep);
void func_0020EF60(CVector3 *out, CVector3 *v, f32 k);
void func_0023C0FC(Hit *hit);
void func_0020EF2C(CVector3 *out, CVector3 *a, CVector3 *b);
void func_0023C120(Hit *hit, Mover *obj);
f32 func_0026872C(Mover *obj, s32 arg1);
s32 func_0023C188(Sweep *sweep, Region *region);

/* Moves obj to pos through the collision tests, in up to 100 slides, and
 * returns the number of hits. */
s32 func_00239F00(Mover *obj, CVector3 pos, CollisionDef *def) {
    Sweep sweep;
    CVector3 accel;
    RegionInfo *info;
    s32 iter;
    s32 i;
    s32 hits;
    s32 passes;
    s32 physics;
    s32 more;
    f32 remaining;
    f32 dt;
    f32 startY;
    f32 drag;
    f32 max;
    f32 half;
    f32 ground;
    f32 dy;

    info = func_0022425C(D_800F7078, obj->region);
    if (def != &D_800F1E10 && def != &D_800F1CC0 && def != &D_800F1AE0 && def != &D_800F1C60) {
        obj->attached = 0;
        if (obj->type == 1) {
            obj->aiFlags &= ~0x30000;
        }
    }
    func_00241894(def);
    iter = 0;
    remaining = 1.0f;
    hits = 0;
    sweep.obj = obj;
    sweep.def = def;
    sweep.end = pos;
    sweep.stop = 0;
    sweep.unkA0 = 0;
    sweep.unk84 = 0;
    dt = D_800B6D28 * (1.0f / 15);
    if (def->flags & 4) {
        passes = 1;
    } else if (def->flags & 0x200000) {
        passes = 3;
    } else {
        passes = 2;
    }
    physics = 0;
    if ((def->flags & 0x4000) && dt != 0.0f) {
        physics = 1;
    }
    sweep.physics = physics;
    startY = obj->pos.y;
    if (physics) {
        drag = def->drag;
        if (def->waterDrag != drag && obj->region != NULL && (obj->region->flags & 1) && info != NULL
            && startY < info->waterY) {
            drag = def->waterDrag;
        }
        drag *= 15.0f;
        max = 1.0f / dt;
        if (!(drag <= max)) {
            drag = max;
        }
        if (drag == 0.0f) {
            accel.x = 0.0f;
            accel.y = def->gravity;
            accel.z = 0.0f;
        } else {
            accel.x = -obj->vel.x * drag;
            accel.y = -obj->vel.y * drag + def->gravity;
            accel.z = -obj->vel.z * drag;
        }
        half = dt * 0.5f;
        sweep.end.x += (obj->vel.x + accel.x * half) * dt;
        sweep.end.y += (obj->vel.y + accel.y * half) * dt;
        sweep.end.z += (obj->vel.z + accel.z * half) * dt;
        obj->vel.x += accel.x * dt;
        obj->vel.y += accel.y * dt;
        obj->vel.z += accel.z * dt;
        D_800F1ADC->vel = obj->vel;
    }
    if (obj->pos.x == sweep.end.x && obj->pos.y == sweep.end.y && obj->pos.z == sweep.end.z) {
        return 0;
    }
    if (info != NULL) {
        sweep.checkRegionInfo = info->flags & 0x4000;
    } else {
        sweep.checkRegionInfo = 0;
    }
    sweep.more = hits + 1 < passes;
    func_0023BE24(&sweep);
    if (sweep.onGround && sweep.physics) {
        func_0020EF60(&obj->vel, &obj->vel, (def->friction > 1.0f) ? 0.0f : 1.0f - def->friction);
    }
    do {
        func_0023C0FC(&sweep.hit);
        func_0020EF2C(&sweep.delta, &sweep.end, &obj->pos);
        if (sweep.physics) {
            sweep.hit.vel = obj->vel;
        }
        if (D_800AD190[0] != NULL && !sweep.stop) {
            i = 0;
            do {
                D_800AD190[i](&sweep);
                i++;
            } while (D_800AD190[i] != NULL && !sweep.stop);
        }
        switch ((u32) sweep.hit.type) {
            case 2:
            case 3:
                hits++;
            case 1:
                obj->pos = sweep.hit.pos;
                func_0023C120(&sweep.hit, obj);
                if (sweep.physics) {
                    obj->vel = sweep.hit.vel;
                }
                sweep.unk84 = sweep.unk88;
                sweep.unk64 = sweep.unk70;
                sweep.unk7C = sweep.unk80;
                break;
            case 0:
                obj->pos = sweep.end;
                break;
        }
        if ((def->flags & 0x800000) && D_800F1ADC->unk9C != 0) {
            return hits;
        }
        remaining -= sweep.hit.fraction * remaining;
        if (sweep.onGround) {
            ground = func_0026872C(obj, 0);
            obj->pos.y = (obj->pos.y <= ground) ? ground : obj->pos.y;
        }
        more = 0;
        if (sweep.hit.type == 1 || sweep.hit.type == 3) {
            more = 1;
        }
        if (more) {
            sweep.end = sweep.hit.slideTo;
            sweep.more = hits + 1 < passes;
            func_0023BE24(&sweep);
        }
        iter++;
    } while (more && iter < 100 && hits < passes);

    if (def->unk5 != 0 && !(def->flags & 0x01000000) && func_0023C188(&sweep, obj->region)) {
        if (remaining > 0.01f) {
            if (sweep.hit.posOut == &D_800F1ADC->unk8) {
                more = def->unk4 != 1;
            } else if (sweep.hit.posOut == &D_800F1ADC->unkA0) {
                more = def->unk5 != 1;
            } else {
                more = 0;
            }
        } else {
            more = 0;
        }
        ground = func_0026872C(obj, 0);
        obj->pos.y = MAX(ground, more ? sweep.end.y : obj->pos.y);
    }
    if (sweep.onGround && sweep.physics && dt != 0.0f && def->gravity != 0.0f) {
        dy = obj->pos.y - startY;
        if (dy < 204.79999f) {
            obj->vel.y = dy / dt;
        }
    }
    return hits;
}
