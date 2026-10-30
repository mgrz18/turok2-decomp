#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
} Matrix18;

typedef struct {
    /* 0x000 */ u8 pad0[0x98];
    /* 0x098 */ s32 model;
    /* 0x09C */ u8 pad9C[0xB8 - 0x9C];
    /* 0x0B8 */ u16 type;
    /* 0x0BA */ u8 padBA[0x110 - 0xBA];
    /* 0x110 */ Matrix18 mtx[2];
    /* 0x140 */ u8 pad140[0x151 - 0x140];
    /* 0x151 */ s8 unk151;
} Actor;

typedef struct {
    /* 0x000 */ u8 pad0[0x34];
    /* 0x034 */ s8 anim;
    /* 0x035 */ u8 pad35[0x114 - 0x35];
    /* 0x114 */ s32 slot;
} CAI;

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 pad10[0x18 - 0x10];
    /* 0x18 */ s32 flag;
} DrawInfo;

typedef struct {
    /* 0x00 */ u8 pad0[0x64];
} Light;

extern Light D_8013023C[];
extern s32 D_800B6D1C;
extern s32 D_800AB8B0;

extern void func_00236324(Light *light);
extern void func_00235898(void);
extern void func_0020A33C(s32 a, s32 model, s32 c, Matrix18 *mtx, s32 e, s32 f, s32 g);

void func_0040EDB0(Actor *actor, CAI *ai, DrawInfo *info) {
    func_00236324(&D_8013023C[ai->slot]);
    func_00235898();
    switch (actor->type) {
        case 0x450:
            if (ai->anim == 9) {
                info->flag = 1;
            }
            break;
        case 0x455:
            if (ai->anim == 6) {
                info->flag = 1;
            }
            break;
    }
    func_0020A33C(info->unkC, actor->model, 1, &actor->mtx[D_800B6D1C], 0, info->flag, actor->unk151);
    D_800AB8B0 = 0;
}
