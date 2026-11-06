/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/8044ACCC_19.CPP, func_8044A62C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/list.h"

struct func_8044B27C_S1;
struct func_8044B27C_S1 {
    s32 unk0;
    char pad0[0x4 - 0x0 - sizeof(s32)];
    s32 unk4;
    char pad4[0x8 - 0x4 - sizeof(s32)];
    s32 unk8;
    char pad8[0xC - 0x8 - sizeof(s32)];
    CList unkC;
    CList unk20;
    char pad34[0x38 - 0x34];
    s32 unk38;
    char pad38[0x40 - 0x38 - sizeof(s32)];
    char unk40;
    char pad40[0xF00 - 0x40 - sizeof(char)];
    s32 unkF00;
    char padF00[0xF04 - 0xF00 - sizeof(s32)];
    s32 unkF04;
    char padF04[0xF08 - 0xF04 - sizeof(s32)];
    s32 unkF08;
    char padF08[0xF0C - 0xF08 - sizeof(s32)];
    CList unkF0C;
    char padF0C[0x11E8 - 0xF0C - sizeof(CList)];
    s32 unk11E8; /* Turok 2 layout: 0x18 smaller than Rage Wars from 0xF00 on */
};
struct func_8044B27C_S1;
typedef struct func_8044B27C_S1 func_8044B27C_S1;
extern void CList__Construct(CList *list, s32 prevOffset, s32 nextOffset);
extern void * memset(void *arg0, int arg1, u32 arg2);
extern void CCamera__Construct(void *);
extern void CCamera__Advance(void *);
void CCameraPool__Construct(func_8044B27C_S1 *actor) {
    char *parts;
    memset(actor, 0, 0x11F0);
    CList__Construct(&actor->unkC, 0, 4);
    CList__Construct(&actor->unk20, 0, 4);
    CList__Construct(&actor->unkF0C, 0, 4);
    parts = &actor->unk40;
    actor->unk11E8 = 1;
    actor->unk0 = 0;
    actor->unk4 = 0;
    actor->unk8 = 0;
    actor->unkF00 = 0;
    actor->unkF04 = 0;
    actor->unkF08 = 0;
    actor->unk38 = 2;
    CCamera__Construct(parts);
    CCamera__Advance(parts);
}
