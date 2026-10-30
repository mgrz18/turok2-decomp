/* NON-MATCHING: 4 of 358 words differ: GCC keeps &state (0x8012F9D8) in s0 from the first state test and copies dest relative to it; the ROM copies it relative to s1 (0x8012F9D4). */
#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x00 */ s32 kind;
    /* 0x04 */ CGameObjectInstance *obj;
    /* 0x08 */ f32 angle;
    /* 0x0C */ CVector3 a;
    /* 0x18 */ CVector3 b;
    /* 0x24 */ f32 dist;
    /* 0x28 */ u8 pad28[0x88 - 0x28];
} Targets;

/* One step of a boss attack script (0x14 bytes). */
typedef struct {
    /* 0x00 */ u8 type;
    /* 0x04 */ s32 arg;
    /* 0x08 */ f32 animSpeed;
    /* 0x0C */ s32 chance;
    /* 0x10 */ f32 delay;
} BossStep;

/* Boss module state from 0x8012F604 (offsets below are from 0x8012F608). */
typedef struct {
    /* -0x04 */ s32 padF604;
    /* 0x000 */ s32 stage;
    /* 0x004 */ u8 pad004[0x8];
    /* 0x00C */ f32 speed;                  /* 0x8012F614 */
    /* 0x010 */ u8 pad010[0x35C];
    /* 0x36C */ CGameObjectInstance *partA; /* 0x8012F974 */
    /* 0x370 */ CGameObjectInstance *partB; /* 0x8012F978 */
    /* 0x374 */ u8 pad374[0x58];
    /* 0x3CC */ s32 unk3CC;                 /* 0x8012F9D4 */
    /* 0x3D0 */ s32 state;                  /* 0x8012F9D8 */
    /* 0x3D4 */ s32 mode;
    /* 0x3D8 */ u8 pad3D8;
    /* 0x3D9 */ u8 scriptOn;                /* 0x8012F9E1 */
    /* 0x3DA */ u8 pad3DA[0x1A];
    /* 0x3F4 */ BossStep *script;           /* 0x8012F9FC */
    /* 0x3F8 */ s32 cursor;                 /* 0x8012FA00 */
    /* 0x3FC */ s32 index;                  /* 0x8012FA04 */
    /* 0x400 */ u8 pad400[0x28];
    /* 0x428 */ CVector3 dest;              /* 0x8012FA30 */
} BossModule;

extern void func_00246108(CGameObjectInstance *obj, CAI *ai, Targets *t, s32 f1, s32 f2);
extern void func_00243414(CGameObjectInstance *obj, CAI *ai, s32 mode);
extern f32 func_00246310(CGameObjectInstance *obj, CVector3 point);       /* angle to a point */
extern void func_00245BAC(CGameObjectInstance *obj, CAI *ai, f32 turnRate, f32 angle);
extern void func_0020EF2C(CVector3 *out, CVector3 *a, CVector3 *b);       /* out = a - b */
extern f32 func_0020EAA0(CVector3 *v);                                    /* heading */
extern f32 func_0021160C(f32 value, f32 target, f32 rate);
extern f32 func_00298470(f32 x);                                          /* sqrt */
extern f32 func_002982F0(f32 angle);                                      /* sin */
extern f32 func_002974C0(f32 angle);                                      /* cos */
extern f32 func_002119FC(f32 lo, f32 hi);
extern s32 func_00404C44(CGameObjectInstance *obj, CAI *ai, f32 dist);

extern s32 D_800F1F50[];
extern CVector3 *D_800C18F0[];  /* boss paths, ended by a point with y < 0 */
extern s32 D_8012FB48;          /* path */
extern BossModule D_8012F604;
extern s32 D_8012F600;          /* point on the path */
extern f32 D_8012F9F8;          /* cooldown */
extern BossStep *D_8012F9FC;    /* script */
extern s32 D_8012FA00;          /* cursor */
extern s32 D_8012F608;          /* stage, as the script selection reads it */
extern BossStep D_00431008[];
extern BossStep D_004310E4[];
extern BossStep D_00431238[];

void func_00406000(CGameObjectInstance *obj, CAI *ai) {
    Targets t;
    CVector3 d;
    CVector3 dest;
    CGameObjectInstance *target;
    f32 angle;
    f32 speed;
    f32 heading;

    D_800F1F50[0] |= 0x8000;
    func_00246108(obj, ai, &t, 0, 1);
    D_8012F604.unk3CC = 3;
    D_800F1F50[0] &= ~0x8000;
    do {
        target = t.obj;
        angle = 0.0f;
    } while (0);
    if (target != NULL) {
        if (D_8012F604.state == 4) {
            dest = target->header.m_vPos;
            angle = func_00246310(obj, dest);
        }
        dest = D_8012F604.dest;
        if (D_8012F604.state == 3 || D_8012F604.state == 11) {
            angle = func_00246310(obj, dest);
        }
        func_0020EF2C(&d, &obj->header.m_vPos, &dest);
        if (func_00298470(d.x * d.x + d.z * d.z) <= 50.0f) {
            if (D_8012F604.state == 3) {
                if (D_8012F608 == 1) {
                    D_8012F604.script = D_00431008;
                }
                if (D_8012F608 == 2) {
                    D_8012F604.script = D_004310E4;
                }
                if (D_8012F608 == 3) {
                    D_8012F604.script = D_00431238;
                }
                D_8012F604.cursor = -1;
                D_8012F604.index = -1;
                func_00243414(D_8012F604.partB, &D_8012F604.partB->m_AI, 0);
                func_00243414(D_8012F604.partA, &D_8012F604.partA->m_AI, 0);
                func_00243414(obj, ai, 0x1A);
                return;
            }
            if (D_8012F604.state == 11) {
                if (D_8012F608 == 1) {
                    D_8012F604.script = D_00431008;
                }
                if (D_8012F608 == 2) {
                    D_8012F604.script = D_004310E4;
                }
                if (D_8012F608 == 3) {
                    D_8012F604.script = D_00431238;
                }
                D_8012F604.cursor = -1;
                D_8012F604.index = -1;
                func_00243414(obj, ai, 0x17);
                return;
            }
            if (D_8012F604.state == 4) {
                D_8012F600++;
                if (D_800C18F0[D_8012FB48][D_8012F600].y < 0.0f) {
                    D_8012F600 = 0;
                    if (D_8012F604.stage != 1) {
                        D_8012FB48++;
                        if (D_8012FB48 >= 3) {
                            D_8012FB48 = 0;
                        }
                    }
                }
                D_8012F604.dest = D_800C18F0[D_8012FB48][D_8012F600];
            }
        }
    }
    speed = 0.0f;
    func_00245BAC(obj, ai, 0.261799425f, angle);
    dest = D_8012F604.dest;
    func_0020EF2C(&d, &obj->header.m_vPos, &dest);
    heading = func_0020EAA0(&d);
    if (D_8012F604.state == 3 || D_8012F604.state == 11) {
        if (obj->header.m_vPos.y < 921.6f) {
            obj->m_vVelocity.y = 460.8f;
        } else {
            obj->m_vVelocity.y = 0.001f;
        }
        speed = 500.0f;
        obj->m_vVelocity.x = func_002982F0(heading) * speed;
        obj->m_vVelocity.z = func_002974C0(heading) * speed;
    } else {
        speed = func_0021160C(speed, D_8012F604.speed, 5120.0f);
        obj->m_vVelocity.x += speed * func_002982F0(heading);
        obj->m_vVelocity.z += speed * func_002974C0(heading);
        obj->m_vVelocity.y = 0.001f;
    }
    if (D_8012F604.scriptOn && D_8012F604.state != 3 && D_8012F604.state != 11) {
        if (D_8012F604.index == -1) {
            if (D_8012FA00 != -1) {
                D_8012F9F8 = D_8012F9FC[D_8012FA00].delay;
            } else {
                D_8012F9F8 = 0.0f;
            }
            do {
                D_8012FA00++;
                if (D_8012F9FC[D_8012FA00].type == 5) {
                    D_8012FA00 = 0;
                }
                D_8012F604.index = D_8012FA00;
            } while ((f32) D_8012F9FC[D_8012FA00].chance <= func_002119FC(0.0f, 100.0f));
        }
        func_00404C44(obj, ai, t.dist);
    }
}
