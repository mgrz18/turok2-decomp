typedef signed int s32;
typedef float f32;

typedef struct {
    /* 0x000 */ char pad0[0x114];
    /* 0x114 */ f32 x;
    /* 0x118 */ f32 y;
    /* 0x11C */ f32 z;
} Target;

typedef struct {
    /* 0x0000 */ char pad0[0x2B98];
    /* 0x2B98 */ Target *target;
} Level;

typedef struct {
    /* 0x00 */ char pad0[0x34];
    /* 0x34 */ f32 volume;
    /* 0x38 */ char pad38[0xC];
    /* 0x44 */ f32 x;
    /* 0x48 */ f32 y;
    /* 0x4C */ f32 z;
    /* 0x50 */ char pad50[0x8];
    /* 0x58 */ f32 distSq;
    /* 0x5C */ f32 prevDistSq;
    /* 0x60 */ char pad60[0x48];
    /* 0xA8 */ s32 id;
    /* 0xAC */ char padAC[0x4];
    /* 0xB0 */ Level *level;
    /* 0xB4 */ char padB4[0xC];
    /* 0xC0 */ s32 reset;
} Sound;

extern f32 D_800B2848; /* volume step */

void func_00272D3C(Sound *s) {
    f32 dx;
    f32 dy;
    f32 dz;
    f32 d;
    f32 v;
    f32 vol;
    Target *t;

    if (s->id != 0x1B2 && s->id != 0x1B3) {
        s->volume = 1.0f;
        return;
    }
    if (s->reset != 0) {
        s->volume = 1.0f;
    }
    t = s->level->target;
    dx = s->x - t->x;
    dy = s->y - t->y;
    dz = s->z - t->z;
    d = dx * dx + dy * dy + dz * dz;
    s->distSq = d;
    if (s->prevDistSq < d) {
        s->volume -= D_800B2848;
    } else if (d < s->prevDistSq) {
        s->volume += D_800B2848;
    } else {
        v = s->volume;
        if (v > 1.0f) {
            vol = v - D_800B2848;
            if (vol < 1.0f) {
                vol = 1.0f;
            }
            s->volume = vol;
        } else if (v < 1.0f) {
            vol = v + D_800B2848;
            if (vol > 1.0f) {
                vol = 1.0f;
            }
            s->volume = vol;
        }
        return;
    }
    if (s->volume > 1.4f) {
        s->volume = 1.4f;
    } else if (s->volume < 0.6f) {
        s->volume = 0.6f;
    }
    s->prevDistSq = s->distSq;
}
