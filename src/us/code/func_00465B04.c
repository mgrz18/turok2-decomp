#include "structs/vector.h"
#include "common.h"
#include "structs/world.h"

/* The second VM module's copy of func_0042CBE4, from the same source file as
 * func_002261A8.c: the engine's func_002261A8 and the VM's func_0042CBE4.
 * Its code reads the first module's pool (versions/shared_rodata.us.txt):
 * func_002261A8's -1.0f at 0x800A6010, then this function's 1.0f and its jump
 * table at 0x800A6018, so the file's .rodata has to start with that -1.0f
 * for the table to land 8-aligned where the ROM has it. The engine half is
 * compiled into .text_engine, which no output section names: the linker
 * script's /DISCARD/ drops it. docs/VMASM.md. */

void func_0020EF2C(CVector3 *out, CVector3 *a, u8 *b);
void *func_002017D4();
static s32 func_002261A8(CWorld *lv, s32 unused, CVector3 *pt) __attribute__((section(".text_engine")));

static s32 func_002261A8(CWorld *lv, s32 unused, CVector3 *pt) {
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
            p += 0x1C;
        } while (i < n);
    }
    return (s32)bestIdx;
}

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

void func_00465B04(s32 unused, void ***resource) {
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
