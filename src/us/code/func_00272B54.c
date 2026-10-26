/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_0025A864.c, func_0025A864, with Turok 2's layouts. */

typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
typedef struct M04Vec3 {
    f32 x, y, z;
} M04Vec3;
typedef union M04IF {
    s32 i;
    f32 f;
} M04IF;
typedef struct M04Rec12 {
    u16 w0, w1, w2;
    s16 w3;
    u16 w4, w5;
} M04Rec12;
typedef struct M04Tgt {
                u8 pad0[0x114];
                f32 x;          /* Turok 2: 0x114 (Rage Wars: 0x128) */
                f32 y;
                f32 z;
                u8 pad120[0x528 - 0x120];
                f32 f528;
} M04Tgt;
typedef struct M04Ctx {
                 u8 pad0[0x84];
                 u8 player[0x58];
                 s16 snd[16];
                 u8 padFC[8];
                 s32 unk104;
                 u8 pad108[0x2B88 - 0x108];
                 u8 *tab2B88;
                 u8 pad2B8C[8];
                 s8 defaultPan;
                 u8 pad2B95[3];
                 M04Tgt *tgt;
                 u8 pad2B9C[4];
                 f32 volA;
                 f32 volB;
                 f32 volC;
                 u8 pad2BAC[0xC];
                 s32 unk2BB8;
} M04Ctx;
typedef struct M04VP {
               s32 w0;
               s32 w4;
               s32 vol;
               s32 wC;
               s16 s10;
               u8 pad12[2];
               M04IF v14;
               s16 pan;
               u8 pad1A[2];
               f32 scale;
               f32 f20;
               f32 f24;
               s16 s28;
               s16 s2A;
               s16 s2C;
               u8 pad2E[6];
               M04Vec3 pos;
               s32 f40;
               s8 mode;
               u8 pad45[3];
               f32 f48;
               f32 f4C;
} M04VP;
typedef struct M04Emitter {
               s32 w0;
               s32 w4;
               s32 w8;
               s32 id;
               M04VP vp;
               M04Rec12 env;
               f32 envA;
               f32 envSlope0;
               M04Rec12 env2;
               f32 env2A;
               f32 env2Slope;
               s32 hasEnv;
               M04Rec12 rec;
               f32 env0;
               f32 envSlope;
               s32 unkA0;
               u32 flags;
               s32 kind;
               s32 unkAC;
               M04Ctx *ctx;
               s32 unkB4;
               f32 unkB8;
               s32 unkBC;
               s32 resetFlag;
               s32 unkC4;
               f32 unkC8;
} M04Emitter;
f32 func_0026D210(u16 x);                /* Trunc16ToF32 */
void func_00293E60(void *p, s16 id);     /* alSndpSetSound */
void func_00293D90(void *p, s32 pan);    /* alSndpSetPan */
s32 func_002718E0(M04Ctx *c);
s32 func_00271E20(f32 *pos, M04Tgt *t);

void func_00272B54(M04Emitter *o) {
    M04VP *vp;
    s32 neg;
    u8 pan;
    u8 *pl;
    M04Ctx *c;

    vp = &o->vp;
    if (func_002718E0(o->ctx) == 0) {
        c = o->ctx;
        func_00293E60(c->player, c->snd[o->w0]);
        func_00293D90(c->player, 0x40);
        return;
    }
    if (o->hasEnv != 0) {
        if (*(s16 *)&o->rec.w4 > 0) {
            o->rec.w4 = o->rec.w4 - 1;
        } else {
            neg = 0;
            if (o->envSlope < 0.0f) {
                neg = 1;
            }
            if (neg) {
                if (o->env0 < func_0026D210(o->rec.w2)) {
                    goto slide;
                }
            } else if (func_0026D210(o->rec.w2) < o->env0) {
slide:
                o->env0 = o->env0 - o->envSlope;
                if (neg) {
                    if (func_0026D210(o->rec.w2) < o->env0) {
                        o->env0 = func_0026D210(o->rec.w2);
                    }
                } else {
                    if (o->env0 < func_0026D210(o->rec.w2)) {
                        o->env0 = func_0026D210(o->rec.w2);
                    }
                }
            }
        }
        vp->pan = (s32)o->env0;
        goto setpan;
    }
    if ((o->flags & 0x400) && o->resetFlag == 0) {
        vp->pan = func_00271E20((f32 *)((u8 *)o + 0x44), o->ctx->tgt);
setpan:
        pan = ((u8 *)&vp->pan)[1];
        pl = o->ctx->player;
        func_00293E60(pl, o->ctx->snd[o->w0]);
        func_00293D90(pl, pan);
        return;
    }
    vp->pan = 0x40;
}
