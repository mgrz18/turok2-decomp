#include "common.h"
#include "structs/world.h"

/* The input state the controller reader fills (D_8011AAD8, 0x184 bytes). */
typedef struct InputState {
    s32 buttons;            /* 0x00 */
    s32 pad4[2];
    s32 held;               /* 0x0C */
} InputState;

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
void func_002237C4(CWorld *);
void func_00226390(CWorld *);
void func_00206920(void *);
void func_0020565C(s32, void **);
void func_00235FC4(void *);
void func_00253018(void *);
void func_00221210(CWorld *);
void func_002216DC(CWorld *);
void func_00221A24(CWorld *);
void func_00252ADC(void *);
void func_0027F2C4(void *, CWorld *);
void func_00230CB4(void *);
void func_00235F48(void *);
void func_00226490(CWorld *);
void func_002263C0(CWorld *);
void func_002274A4(void *);
f32 func_002982F0(f32);     /* sinf */
f32 func_002974C0(f32);     /* cosf */
void func_002253F8(CWorld *);
void func_00226724(CWorld *);
void func_00226024(CWorld *);

void func_00220530(CWorld *world) {
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
    func_00206920(world->m_Heap);
    func_0020565C(0, world->m_hHeap);
    func_00235FC4(D_801102B8);
    func_00253018(D_80119870);
    func_00221210(world);
    func_002216DC(world);
    func_00221A24(world);
    if (world->m_WarpState == 0) {
        func_00252ADC(D_80119870);
    }
    func_0027F2C4(D_801198B0, world);
    func_00230CB4(world->m_ParticleSystem);
    if (world->m_WarpState == 0 && *(s32 *)(D_801198B0 + 0x13F4) == 0) {
        func_00235F48(D_801102B8);
        func_00226490(world);
        func_002263C0(world);
        func_002274A4(world->m_ActorPool);
        world->m_SkyAngle += D_800B6D28 * 0.087266475f;
        s = func_002982F0(world->m_SkyAngle * 0.5f);
        D_800DD78C = s;
        world->m_qSky.x = 0;
        world->m_qSky.y = s;
        world->m_qSky.z = 0;
        world->m_qSky.w = func_002974C0(world->m_SkyAngle * 0.5f);
        func_002253F8(world);
    }
    func_00226724(world);
    func_00226024(world);
}
