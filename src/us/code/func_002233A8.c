/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028A258.c, func_0028A258.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
typedef struct ActorInfo {
               u32 flags;
               s32 unk4;
               u8 pad8[4];
               s16 unkC;
               u8 padE[6];
               f32 unk14;
               u8 pad18[0xC];
               u32 flags24;
               u8 pad28[0x28];
               f32 unk50;
} ActorInfo;
typedef struct ActorDef {
               s32 type;
               u8 pad4[0x10];
               ActorInfo info;
} ActorDef;
typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;
/* Turok 2 layouts: only the fields used here (Rage Wars: pos 0x8, def 0x18, list at 0xF5C). */
typedef struct Entity {
    u8 pad0[0x4];
    Vec3f pos;          /* 0x4 */
    u8 pad10[0x4];
    ActorDef *def;      /* 0x14 */
} Entity;
typedef struct Level {
    u8 pad0[0xF04];
    Entity *arrF04[32]; /* 0xF04 */
    s32 cntF84;         /* 0xF84 */
} Level;
typedef struct {
    s32 type;
    u8 pad4[0x10];
    u32 flags14;
    u8 pad18[8];
    f32 f20;
    f32 f24;
    f32 f28;
} DefQ;
f32 func_0026841C(Entity *e);
f32 func_002671B4(Entity *e);
s32 func_00265F90(Entity *e, u8 *p, Entity *me);
void func_00243414(Entity *e, u8 *p, s32 a);
void func_002233A8(Level *lv, Entity *me, Vec3f *p0, Vec3f *p1) {
    s32 isB;
    s32 is1;
    f32 myBot;
    f32 myTop;
    s32 i;
    Entity *e;
    f32 eBot;
    f32 eTop;
    s32 kind;
    s32 s1;
    s32 s2;
    s32 s3;
    s32 s4;
    s32 a0;
    s32 v1;
    s32 a2;
    s32 a1;
    s32 hit;
    s32 hit2;
    f32 r1;
    register f32 r2 asm("$f20");
    f32 dx;
    f32 dz;
    f32 d0;
    f32 d1;
    f32 ex;
    f32 ez;
    s32 ok;
    u32 fl;
    if (p0->x != p1->x || p0->y != p1->y || p0->z != p1->z) {
        kind = me->def->type;
        isB = kind == 0xB;
        is1 = kind == 1;
        myBot = me->pos.y + func_0026841C(me);
        myTop = myBot + func_002671B4(me);
        for (i = 0; i < lv->cntF84; i++) {
            e = lv->arrF04[i];
            if (e != me) {
                s4 = 0;
                s1 = 0;
                eBot = e->pos.y + func_0026841C(e);
                eTop = eBot + func_002671B4(e);
                switch (((DefQ *)e->def)->type) {
                case 6:
                    s2 = 0;
                    s3 = s4 = 1;
                    fl = ((DefQ *)e->def)->flags14;
                    r1 = ((DefQ *)e->def)->f20;
                    r2 = ((DefQ *)e->def)->f24;
                    a0 = fl & 4;
                    v1 = fl & 8;
                    a2 = fl & 0x10;
                    a1 = fl & 0x20;
                    break;
                case 10:
                    s2 = 1;
                    s3 = 0;
                    a2 = s3;
                    a1 = s3;
                    s1 = s2;
                    fl = ((DefQ *)e->def)->flags14;
                    r1 = ((DefQ *)e->def)->f24;
                    r2 = 0.0f;
                    a0 = fl & 4;
                    v1 = fl & 8;
                    break;
                case 7:
                    s2 = 1;
                    s3 = 0;
                    a2 = s3;
                    a1 = s3;
                    s1 = s2;
                    fl = ((DefQ *)e->def)->flags14;
                    r1 = ((DefQ *)e->def)->f28;
                    r2 = 0.0f;
                    a0 = fl & 8;
                    v1 = fl & 0x10;
                    break;
                default:
                    s2 = 0;
                    r1 = 0.0f;
                    a0 = 0;
                    v1 = 0;
                    s3 = 0;
                    a2 = 0;
                    a1 = 0;
                    r2 = 0.0f;
                    break;
                }
                if (s1 != 0 && (myTop < eBot || eTop < myBot)) {
                    hit2 = hit = 0;
                } else {
                    hit = 0;
                    if (isB != 0 && a0 != 0) {
                        hit = 1;
                    } else if (is1 != 0 && v1 != 0) {
                        hit = 1;
                    }
                    hit2 = 0;
                    if ((isB != 0 && a2 != 0) || (is1 != 0 && a1 != 0)) {
                        hit2 = 1;
                    }
                }
                if (hit != 0 || hit2 != 0) {
                    d0 = (p0->x - e->pos.x) * (p0->x - e->pos.x) + (p0->z - e->pos.z) * (p0->z - e->pos.z);
                    d1 = (p1->x - e->pos.x) * (p1->x - e->pos.x) + (p1->z - e->pos.z) * (p1->z - e->pos.z);
                    if (hit != 0) {
                        if (s4 != 0) {
                            ok = 0;
                            if (r1 * r1 < d0) {
                                if (d1 <= r1 * r1) {
                                    ok = 1;
                                }
                            }
                        } else {
                            ok = 1;
                            if (!(d1 <= r1 * r1)) {
                                ok = 0;
                            }
                        }
                        if (ok != 0) {
                            if (e->def->type == 10 && s2 == 1) {
                                ok = func_00265F90(e, (u8 *)e + 0x140, me);
                            }
                            if (ok != 0) {
                                func_00243414(e, (u8 *)e + 0x140, s2);
                            }
                        }
                    }
                    if (hit2 != 0) {
                        if (s4 != 0) {
                            ok = 0;
                            if (d0 <= r2 * r2) {
                                if (r2 * r2 < d1) {
                                    ok = 1;
                                }
                            }
                        } else {
                            ok = 1;
                            if (!(r2 * r2 < d1)) {
                                ok = 0;
                            }
                        }
                        if (ok != 0) {
                            func_00243414(e, (u8 *)e + 0x140, s3);
                        }
                    }
                }
                __asm__ volatile("" : : "f"(r2));
            }
        }
    }
}
