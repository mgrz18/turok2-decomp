/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00259F88.c, func_00259F88.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

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
typedef struct M04Info {
               u8 pad0[6];
               u16 bits6;
               s16 envId;
               u8 padA[5];
               s8 b0F;
} M04Info;
typedef struct M04Tab {
    f32 key;
    f32 val;
} M04Tab;
extern f32 D_800A8110;
extern f32 D_800A8114;
extern f32 D_800A8118;
extern f32 D_800A811C;
extern f32 D_800A8120;
extern f32 D_800A8124;
extern f32 D_800B2A30;
extern M04Tab D_800B2750[];
extern u8 D_800F2940[];
f32 func_0026D210(u16 x);
u16 func_0026D200(f32 x);
s32 Cinema_PathPlaying();
M04Rec12 *func_00271778(M04Ctx *c, s16 id);
f32 func_0021149C(f32 x);
void func_00272278(M04Emitter *o, M04Info *rec) {
    M04VP *vp = &o->vp;
    M04Rec12 *env;
    u8 *pp;
    s32 i;
    s32 kind;
    s32 n;
    s32 t;
    f32 vf;
    f32 a;
    f32 b;
    f32 f;
    f32 fs;
    M04Vec3 *p;
    M04Vec3 *q;
    pp = o->ctx->tab2B88;
    o->unkC8 = D_800A8110;
    if (o->kind < 0x100) {
        for (i = 0; i < 0x1F; i++) {
            if (o->kind == (s32)D_800B2750[i].key) {
                o->unkC8 = D_800B2750[i].val;
                break;
            }
        }
    }
    if (Cinema_PathPlaying() != 0) {
        o->unkC8 = o->unkC8 * D_800A8114;
    }
    env = &o->env2;
    if (rec->envId != -1) {
        *env = *func_00271778(o->ctx, rec->envId);
    } else {
        env->w0 = func_0026D200((f32)rec->b0F);
    }
    if (!(rec->bits6 & 0x800) && o->resetFlag != 1 && o->ctx->tgt != 0) {
        a = (f32)pp[0] * D_800A8118;
        b = (f32)pp[1] * D_800A8118;
        if (a == D_800A811C) {
            vf = D_800A811C;
        } else if (b == a) {
            vf = a;
        } else {
            vf = a + func_0021149C(b - a);
        }
        p = &vp->pos;
        q = (M04Vec3 *)&o->ctx->tgt->x;
        {
            f32 d2 = (p->x - q->x) * (p->x - q->x) + (p->y - q->y) * (p->y - q->y) + (p->z - q->z) * (p->z - q->z);
            if (D_800B2A30 <= d2) {
                f = 0.0f;
            } else {
                f = D_800A8120 - d2 / D_800B2A30;
            }
        }
        switch (vp->mode) {
        case 2:
            vp->scale = f;
            break;
        case 0:
            f = f * f;
            f = f * f;
        case 1:
            f = f * f;
            vp->scale = f;
            break;
        default:
            f = 0.0f;
            vp->scale = 0.0f;
            break;
        }
        vp->f20 = f;
    } else {
        vf = D_800A8124;
        vp->scale = vf;
        vp->f20 = vf;
    }
    if (rec->bits6 & 4) {
        vp->vol = (s32)func_0026D210(env->w0);
        fs = func_0026D210(env->w1);
        fs -= func_0026D210(env->w2);
        if (env->w3 == 0) {
            o->env2Slope = 0.0f;
        } else {
            o->env2Slope = fs / (f32)env->w3;
        }
        o->env2A = func_0026D210(env->w1);
        n = (s32)func_0026D210(env->w0);
        if ((f32)n < 0.0f) {
            t = -n * 2;
        } else {
            t = n * 2;
        }
        vp->vol = (s32)((f32)(s32)((f32)*(s16 *)(D_800F2940 + t) * o->env2A) * vf);
        if ((f32)n < 0.0f) {
            t = -n * 2;
        } else {
            t = n * 2;
        }
        env->w0 = func_0026D200((f32)*(s16 *)(D_800F2940 + t) * vf);
        vp->wC = (s32)func_0026D210(env->w0 & 0xFFFF);
        return;
    }
    n = (s32)func_0026D210(env->w0);
    if ((f32)n < 0.0f) {
        t = -n * 2;
    } else {
        t = n * 2;
    }
    t = (s32)((f32)*(s16 *)(D_800F2940 + t) * vf);
    vp->vol = t;
    vp->wC = t;
}
