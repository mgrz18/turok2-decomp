#include "common.h"
#include "structs/pager.h"
#include "globals/pager.h"

/* Four words of memory stats ending at D_801206BC. */
extern s32 D_801206BC;
extern u32 D_80120FCC;

/* Byte-offset field access: a struct member reference here reorders the
 * stores below. */
#define FIELD_AT(ptr, type, T, field) (*(type *)((s8 *)(ptr) + (s32) &((T *)0)->field))

void VM_Age(void) {
    PageRegion *var_v1;
    s32 var_a0_2;
    s32 var_a1;
    u16 temp_v0;
    u32 *var_a0;
    u32 temp_v1;
    u32 var_a2;
    u32 var_a3;

    var_a2 = 0;
    var_a3 = 0x80000000;
    var_a1 = 0;
    var_a0 = &D_80120FCC;
    do {
        temp_v1 = *var_a0;
        if (var_a2 < temp_v1) {
            var_a2 = temp_v1;
        }
        if (temp_v1 < var_a3) {
            var_a3 = temp_v1;
        }
        *var_a0 = temp_v1 + 1;
        var_a1 += 1;
        var_a0 = (__typeof__(var_a0))((s8 *)var_a0 + 0x10);
    } while (var_a1 < 0x18);
    var_v1 = &gPager_regions;
    var_a0_2 = 0;
    if (&gPager_regions != NULL) {
        do {
            temp_v0 = var_v1->pages;
            var_v1 = var_v1->next;
            var_a0_2 += temp_v0 << 0xC;
        } while (var_v1 != NULL);
    }
    *(s32 *)((s8 *)&D_801206BC + 0) = var_a0_2;     /* total pages << 12 */
    *(s32 *)((s8 *)&D_801206BC - 0xC) = var_a2;   /* largest counter */
    *(s32 *)((s8 *)&D_801206BC - 0x8) = var_a3;   /* smallest counter */
    *(s32 *)((s8 *)&D_801206BC - 0x4) = FIELD_AT(gPager_chain_head, s32, PageMapping, age);
}
