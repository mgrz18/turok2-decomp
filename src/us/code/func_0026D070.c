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

s32 func_00207914(void *, s32);                 /* extern */

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
    func_00207914(&arg0->texture, temp_a1);
}
