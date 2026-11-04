#include "structs/vector.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028D268.c, func_0028D268.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/world.h"

void func_0020EF2C(CVector3 *out, CVector3 *a, u8 *b);
void *func_002017D4();
s32 func_002261A8(CWorld *lv, s32 unused, CVector3 *pt) {
    CVector3 d;
    f32 bestIdx = -1.0f;
    f32 bestD = 0.0f;
    s32 i = 0;
    s32 *t;
    s32 n;
    u8 *p;
    t = func_002017D4(func_002017D4(lv->unk_68), 1);
    n = t[1];
    p = (u8 *)(t + 2);
    if (n > 0) {
        do {
            f32 dd;
            func_0020EF2C(&d, pt, p);
            dd = d.x * d.x + d.y * d.y + d.z * d.z;
            if (bestIdx < 0.0f || dd < bestD) {
                bestIdx = (f32)i;
                bestD = dd;
            }
            i++;
            p += 0x1C; /* 0x38 in Rage Wars */
        } while (i < n);
    }
    return (s32)bestIdx;
}

/* func_0042CBE4 is VM code (the `virtual` segment) from this same source file:
 * its 1.0f and jump table follow func_002261A8's -1.0f in one .rodata
 * (0x800A6010: -1.0f, 1.0f, then the table at 0x800A6018, 8-aligned in the
 * section), which only one object can give. Its .text goes to a section of its
 * own, .text_vm, which the yaml links into `virtual` (linker_section), while
 * .text stays in the engine. The second module's copy is func_00465B04.c.
 * docs/VMASM.md. */

s32 func_00200B00(void ***resource, s32 index);
void *func_002017D4(s32 *table, s32 index);

typedef struct {
    /* 0x00 */ s32 type;
    /* 0x04 */ u8 pad4[0x24];
    /* 0x28 */ f32 x;
    /* 0x2C */ f32 y;
    /* 0x30 */ f32 z30;
    /* 0x34 */ f32 z34;
    /* 0x38 */ u8 pad38[0x14];
    /* 0x4C */ f32 x2;
    /* 0x50 */ f32 y2;
    /* 0x54 */ f32 z2;
} Element;

void func_0042CBE4(s32 unused, void ***resource) __attribute__((section(".text_vm")));

void func_0042CBE4(s32 unused, void ***resource) {
    f32 scale = 1.0f;
    s32 *table;
    s32 count;
    s32 i;
    Element *e;

    if (func_00200B00(resource, 0)) {
        table = (s32 *) **resource;
        count = *table;
        for (i = 0; i < count; i++) {
            e = func_002017D4(table, i);
            switch (e->type) {
                case 1:
                    e->x *= scale;
                    e->z30 *= scale;
                    e->z34 *= scale;
                    e->x2 *= scale;
                    e->y2 *= scale;
                    e->z2 *= scale;
                    break;
                case 11:
                    e->x *= scale;
                    e->y *= scale;
                    break;
                case 0:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                    break;
            }
        }
    }
}
