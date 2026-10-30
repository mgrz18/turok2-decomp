#include "common.h"

/* AI chase update: turns toward the target and runs or walks at it; flying
 * modes (1, 2) also climb toward its height and bank into the turn. In range,
 * it attacks (path node 4 / special 7) or lets func_0025D030 pick a melee. */

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    /* 0x00 */ s32 kind;
    /* 0x04 */ s32 visible;
    /* 0x08 */ f32 angle;
    /* 0x0C */ Vec3f a;
    /* 0x18 */ Vec3f b;
    /* 0x24 */ f32 dist;
    /* 0x28 */ u8 pad28[0x44 - 0x28];
} Target;

typedef struct {
    /* 0x00 */ Target a;
    /* 0x44 */ Target b;
} Targets;

typedef struct {
    /* 0x00 */ Vec3f d;
    /* 0x0C */ Vec3f n;
    /* 0x18 */ f32 len;
    /* 0x1C */ Vec3f e;
    /* 0x28 */ Vec3f e2;
    /* 0x34 */ f32 elen;
    /* 0x38 */ f32 dy;
    /* 0x3C */ u8 pad3C[0x40 - 0x3C];
} Dir;

typedef struct {
    /* 0x00 */ u8 pad0[0x86];
    /* 0x86 */ u16 canRun;
} ActorInfo;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ ActorInfo info;
} ActorDef;

typedef struct {
    /* 0x000 */ u8 pad0[0x14];
    /* 0x014 */ ActorDef *def;
    /* 0x018 */ u8 pad18[0x1C - 0x18];
    /* 0x01C */ f32 velY;
    /* 0x020 */ u8 pad20[0xB8 - 0x20];
    /* 0x0B8 */ u16 type;
    /* 0x0BA */ u8 padBA[0xD4 - 0xBA];
    /* 0x0D4 */ u32 flags;
} Actor;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ u16 flags;
} PathNode;

typedef struct {
    /* 0x000 */ u8 pad0[0x34];
    /* 0x034 */ s8 state;
    /* 0x035 */ s8 prevState;
    /* 0x036 */ u8 unk36;
    /* 0x037 */ s8 unk37;
    /* 0x038 */ u8 pad38[0x3C - 0x38];
    /* 0x03C */ u32 flags;
    /* 0x040 */ u8 pad40[0x54 - 0x40];
    /* 0x054 */ f32 pushX;
    /* 0x058 */ u8 pad58[0x5C - 0x58];
    /* 0x05C */ f32 pushZ;
    /* 0x060 */ u8 pad60[0x94 - 0x60];
    /* 0x094 */ u8 path[0xC7 - 0x94];
    /* 0x0C7 */ s8 hits;
    /* 0x0C8 */ u8 padC8[0x10C - 0xC8];
    /* 0x10C */ f32 roll;
    /* 0x110 */ u8 pad110[0x133 - 0x110];
    /* 0x133 */ s8 modeIndex;
} CAI;

#define ABS(x) ((x) < 0.0f ? -(x) : (x))

extern void func_00246108(Actor *actor, CAI *ai, Targets *t, s32 f1, s32 f2);
extern void func_0024599C(Actor *actor, CAI *ai, Dir *dir);
extern void func_00245BAC(Actor *actor, CAI *ai, f32 angle, f32 dist);
extern void func_00243414(Actor *actor, CAI *ai, s32 anim);
extern f32 func_0021160C(f32 value, f32 delta, f32 limit);
extern f32 func_00211774(f32 value, f32 target, f32 rate);
extern s32 func_0025D030(Actor *actor, CAI *ai, Targets *t);
extern void func_0025D308(Actor *actor, CAI *ai);
extern void func_0025C1B0(Actor *actor, CAI *ai, Targets *t);
extern PathNode *func_0028479C(void *path);
extern void func_00284660(void *path);

void func_0025D474(Actor *actor, CAI *ai) {
    Targets t;
    Dir dir;
    s32 runAnim;
    s32 walkAnim;
    s32 dodge;
    s32 mode;
    s32 anim;
    f32 maxSpeed;
    f32 angle;
    f32 turn;
    f32 range;
    f32 bank;

    actor->flags |= 0x20000;
    actor->flags |= 0x20000000;
    func_00246108(actor, ai, &t, 1, 1);
    runAnim = 4;
    if (t.b.kind == 1 && actor->def->info.canRun) {
        dodge = 0;
        runAnim = 6;
    } else {
        dodge = 0;
    }
    walkAnim = 4;
    if (ai->unk37 == 0) {
        walkAnim = 3;
    }
    func_0024599C(actor, ai, &dir);
    mode = ai->modeIndex;
    if (mode == 1 || mode == 2) {
        maxSpeed = 81.92f;
        if (mode == 1) {
            maxSpeed = 163.84f;
        }
        if (t.b.dist > 10.24f) {
            turn = 0.261799425f;
            if (ai->unk36 == 1 || ai->unk36 == 2) {
                turn = 1.0471977f;
            }
            func_00245BAC(actor, ai, turn, t.b.angle);
        }
        if (t.b.kind == 4) {
            func_00243414(actor, ai, walkAnim);
        } else {
            func_00243414(actor, ai, runAnim);
        }
        actor->velY = func_0021160C(actor->velY, t.b.b.y, dir.len * 16.0f);
        if (maxSpeed < actor->velY) {
            actor->velY = maxSpeed;
        } else if (actor->velY < -maxSpeed) {
            actor->velY = -maxSpeed;
        }
        if (mode == 2) {
            bank = -actor->velY * 0.030543264f;
            if (bank > 0.43633235f) {
                bank = 0.43633235f;
            } else if (bank < -0.43633235f) {
                bank = -0.43633235f;
            }
            ai->roll = func_00211774(ai->roll, bank, 0.069813177f);
        }
    } else if (!(ai->flags & 0x400)) {
        angle = 0.261799425f;
        if (ai->unk36 == 1 || ai->unk36 == 2) {
            angle = 1.0471977f;
        }
        if (t.b.dist < 204.79999f) {
            angle *= 3.0f;
        }
        if (ai->state == 3) {
            angle *= 0.5f;
        }
        func_00245BAC(actor, ai, angle, t.b.angle);
    } else {
        turn = 0.261799425f;
        if (ai->unk36 == 1 || ai->unk36 == 2) {
            turn = 1.0471977f;
        }
        func_00245BAC(actor, ai, turn, t.a.angle);
    }
    if (t.a.visible) {
        angle = 0.261799425f;
        if (actor->type == 0x259) {
            angle = 1.5707965f;
        }
        if (ABS(t.a.angle) <= angle) {
            anim = func_0025D030(actor, ai, &t);
            if (anim != 0) {
                ai->prevState = -1;
                func_00243414(actor, ai, anim);
                return;
            }
        }
    }
    switch (t.b.kind) {
    case 7:
        range = 30.72f;
        break;
    case 4:
        range = 61.44f;
        if (walkAnim == 3) {
            range = 30.72f;
        }
        break;
    default:
        range = 102.399994f;
        break;
    }
    if (ai->flags & 0x400) {
        range *= 2.0f;
    }
    if (ai->state != 5 || ai->hits != 0) {
        if (t.b.dist <= range) {
            switch (t.b.kind) {
            case 4:
                if (func_0028479C(ai->path)->flags & 1) {
                    func_00243414(actor, ai, 0xB);
                } else {
                    func_00284660(ai->path);
                }
                break;
            case 7:
                func_00243414(actor, ai, 0xD);
                if (actor->flags & 0x400) {
                    ai->pushX += t.b.b.x * 0.5f;
                    ai->pushZ += t.b.b.z * 0.5f;
                }
                break;
            default:
                func_00243414(actor, ai, 0xA);
                break;
            }
        } else if (t.b.kind == 4) {
            func_00243414(actor, ai, walkAnim);
        } else if (dodge && ABS(t.b.angle) > 2.3561947f) {
            func_00243414(actor, ai, 5);
        } else {
            func_00243414(actor, ai, runAnim);
        }
    }
    func_0025D308(actor, ai);
    func_0025C1B0(actor, ai, &t);
}
