/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_004422BC.c, func_004422BC, with Turok 2's layouts. */
#include "common.h"

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ u8 padC[8];
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ u8 pad20[0x10];
    /* 0x30 */ f32 unk30;
    /* 0x34 */ u8 pad34[0x68];
} MenuBlob; /* 0x9C bytes */

typedef struct {
    /* 0x000 */ s32 id;
    /* 0x004 */ MenuBlob blob;
    /* 0x0A0 */ u8 padA0[0x328 - 0xA0];
    /* 0x328 */ s32 valid;
    /* 0x32C */ f32 unk32C;
} MenuItemExt; /* 0x330 bytes */

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ MenuItemExt *ext;
} MenuItem;

extern u8 D_800F7078[];
extern MenuBlob *func_00225EBC(void *pool, s32 id, s32 arg);

MenuBlob *func_00416214(MenuItem *a, s32 id, s32 arg) {
    MenuItemExt *p = a->ext;
    MenuBlob *v = func_00225EBC(D_800F7078, id, arg);

    if (v == NULL) {
        p->valid = 0;
        return NULL;
    }
    p->id = id;
    p->valid = 1;
    p->blob = *v;
    p->blob.unk0 = 0;
    p->blob.unk4 = 0;
    p->blob.unk14 = 0;
    p->blob.unk18 = 0;
    p->blob.unk1C = 0;
    p->blob.unk8 = 10240.0f;
    p->unk32C = p->blob.unk30;
    return &p->blob;
}
