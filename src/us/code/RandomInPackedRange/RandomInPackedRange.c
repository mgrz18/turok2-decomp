#include "common.h"

/* A material as the model file stores it. */
typedef struct {
    /* 0x00 */ s32 texture;   /* -1 = none */
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 flags;
    /* 0x0C */ u8 prim[4];
    /* 0x10 */ u8 env[4];
    /* 0x14 */ u16 maskS;
    /* 0x16 */ u16 maskT;
} MaterialRec;
/* The runtime material (see Render_SetRenderMode). */
typedef struct {
    /* 0x00 */ u32 flags;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 texture;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 prim[4];
    /* 0x14 */ u8 env[4];
    /* 0x18 */ u16 maskS;
    /* 0x1A */ u16 maskT;
} Material;
s32 Material_GetTextureSet(void *, s32);                 /* extern */

void Material_Unpack(Material *arg0, MaterialRec *arg1) {
    s32 temp_a1;
    s32 var_v0;

    arg0->flags = (s32) arg1->flags;
    arg0->prim[0] = (u8) arg1->prim[0];
    arg0->prim[1] = (u8) arg1->prim[1];
    arg0->prim[2] = (u8) arg1->prim[2];
    arg0->prim[3] = (u8) arg1->prim[3];
    arg0->env[0] = (u8) arg1->env[0];
    arg0->env[1] = (u8) arg1->env[1];
    arg0->env[2] = (u8) arg1->env[2];
    arg0->env[3] = (u8) arg1->env[3];
    arg0->maskS = (u16) arg1->maskS;
    arg0->maskT = (u16) arg1->maskT;
    if (arg1->flags & 0x40) {
        var_v0 = (arg1->unk4 * 2) | 1;
    } else {
        var_v0 = arg1->unk4 * 2;
    }
    arg0->unk4 = var_v0;
    temp_a1 = arg1->texture;
    if (temp_a1 == -1) {
        arg0->texture = 0;
        return;
    }
    Material_GetTextureSet(&arg0->texture, temp_a1);
}

#include "globals/math.h"
#include "globals/engine.h"

/* Two floats packed as their upper 16 bits (see FloatToHalf / HalfToFloat). */
typedef struct {
    /* 0x0 */ u16 base;
    /* 0x2 */ u16 range;
} PackedRange;
s32 Demo_GetPlayerCount(void);
#ifdef TARGET_PORT
void T2Port_RngTrace(void *ra); /* DEBUG-ONLY: T2_RNG_TRACE (port/game/t2det.c) */
#endif

/* base + random(0..1) * range, both unpacked from half-floats. */
f32 RandomInPackedRange(PackedRange r) {
    u32 base = r.base << 16;
    u32 range;

    if (r.range == 0) {
        return *(f32 *) &base;
    }
    range = r.range << 16;
#ifdef TARGET_PORT
    T2Port_RngTrace(__builtin_return_address(0));
#endif
    if (Demo_GetPlayerCount()) {
        gRandomValue = gFrameCountGameplay * 0xA84A5B53 + 0x58348C2D;
    } else {
        gRandomValue = gRandomValue * 0xA84A5B53 + 0x58348C2D;
    }
    return *(f32 *) &base + (f32) ((gRandomValue >> 16) & 0x7FFF) * *(f32 *) &range * (1.0f / 32767);
}
