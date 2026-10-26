#include "common.h"

/* The input state the controller reader fills (D_8011AAD8, 0x184 bytes). */
typedef struct InputState {
    s32 buttons;            /* 0x00 */
    s32 pad4[2];
    s32 held;               /* 0x0C */
} InputState;

typedef struct World {
    u8 pad0[0xC8];
    s32 unkC8;              /* 0xC8 */
    s32 padCC;
    u8 unkD0[0x18B0 - 0xD0];/* 0xD0 */
    u8 unk18B0[0x10518 - 0x18B0];
    u8 unk10518[0x18EA4 - 0x10518];
    f32 skyAngle;           /* 0x18EA4 */
    f32 skyRot[4];          /* 0x18EA8: quaternion about Y */
    u8 pad18EB8[0x18FB0 - 0x18EB8];
    s32 paused;             /* 0x18FB0 */
} World;

extern s32 D_800ACAD8;      /* buttons this frame */
extern s32 D_800ACADC;      /* buttons changed */
extern s32 D_800ACDE0;
extern f32 D_800B6D28;      /* frame time scale */
extern f32 D_800DD78C;
extern InputState D_8011AAD8;
extern u8 D_801102B8[];
extern u8 D_80119870[];
extern u8 D_801198B0[];

void func_00206068(s32, s32);
void func_002237C4(World *);
void func_00226390(World *);
void func_00206920(void *);
void func_0020565C(s32, s32);
void func_00235FC4(void *);
void func_00253018(void *);
void func_00221210(World *);
void func_002216DC(World *);
void func_00221A24(World *);
void func_00252ADC(void *);
void func_0027F2C4(void *, World *);
void func_00230CB4(void *);
void func_00235F48(void *);
void func_00226490(World *);
void func_002263C0(World *);
void func_002274A4(void *);
f32 func_002982F0(f32);     /* sinf */
f32 func_002974C0(f32);     /* cosf */
void func_002253F8(World *);
void func_00226724(World *);
void func_00226024(World *);

void func_00220530(World *world) {
    InputState *input;
    s32 old;
    f32 s;

    old = D_800ACAD8;
    input = &D_8011AAD8;
    D_800ACDE0 = 0;
    D_800ACAD8 = input->held;
    if (input->buttons & 0x4000) {
        D_800ACAD8 |= 0x100;
    }
    if (input->buttons & 0x8000) {
        D_800ACAD8 |= 0x200;
    }
    if (old != D_800ACAD8) {
        D_800ACADC = 1;
        func_00206068(8, old);
    } else {
        D_800ACADC = 0;
    }
    func_002237C4(world);
    func_00226390(world);
    func_00206920(world->unkD0);
    func_0020565C(0, world->unkC8);
    func_00235FC4(D_801102B8);
    func_00253018(D_80119870);
    func_00221210(world);
    func_002216DC(world);
    func_00221A24(world);
    if (world->paused == 0) {
        func_00252ADC(D_80119870);
    }
    func_0027F2C4(D_801198B0, world);
    func_00230CB4(world->unk18B0);
    if (world->paused == 0 && *(s32 *)(D_801198B0 + 0x13F4) == 0) {
        func_00235F48(D_801102B8);
        func_00226490(world);
        func_002263C0(world);
        func_002274A4(world->unk10518);
        world->skyAngle += D_800B6D28 * 0.087266475f;
        s = func_002982F0(world->skyAngle * 0.5f);
        D_800DD78C = s;
        world->skyRot[0] = 0;
        world->skyRot[1] = s;
        world->skyRot[2] = 0;
        world->skyRot[3] = func_002974C0(world->skyAngle * 0.5f);
        func_002253F8(world);
    }
    func_00226724(world);
    func_00226024(world);
}
