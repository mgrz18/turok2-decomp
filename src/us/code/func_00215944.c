/* One source file: CLevelActor__LoadGeometry and CLevelActor__TouchGeometry
 * name the same two literals (GCC shares a string only within a file), the
 * file's whole .rodata. */
#include "common.h"
#include "protos/iset.h"

/* Only the field read here: the key of the resource this loads. */
typedef struct {
    /* 0x00 */ u8 pad0[0x34];
    /* 0x34 */ s32 key;
} ResourceOwner;

s32 *CCache__Request(s32 heap, s32 src, s32 size, s32 type, s32 arg4, void *callback, void *name, s32 arg7);
void CCache__Unlock(s32 heap, s32 *block);
void Model_TouchTextures(s32 *);
extern void Model_DecompressCallbackTerminated();

/* Loads a simple object geometry: reads its index, then the geometry block it points at. */
s32 *CLevelActor__LoadGeometry(void *obj) {
    s32 *index;
    s32 *geometry;
    s32 src;
    s32 size;

    index = CCache__Request(0, *(s32 *) ((u8 *) obj + 0x34), 0x18, 4, 0, NULL, "simp object index", 0);
    if (index == NULL) {
        geometry = NULL;
    } else {
        src = CISet__GetROMAddressAndSize(*index, *(s32 *) ((u8 *) obj + 0x34), 1, &size);
        CCache__Unlock(0, index);
        geometry = CCache__Request(0, src, size, 4, 0, Model_DecompressCallbackTerminated, "simp geometry", 0);
    }
    return geometry;
}

void CLevelActor__TouchGeometry(ResourceOwner *arg0) {
    s32 sp20;
    s32 *temp_v0;
    s32 *var_s0;
    s32 temp_s0;

    temp_v0 = CCache__Request(0, arg0->key, 0x18, 4, 0, NULL, "simp object index", 0);
    if (temp_v0 != NULL) {
        temp_s0 = CISet__GetROMAddressAndSize(*temp_v0, arg0->key, 1, &sp20);
        CCache__Unlock(0, temp_v0);
        var_s0 = CCache__Request(0, temp_s0, sp20, 4, 0, Model_DecompressCallbackTerminated, "simp geometry", 0);
    } else {
        var_s0 = NULL;
    }
    if (var_s0 != NULL) {
        Model_TouchTextures(var_s0);
        CCache__Unlock(0, var_s0);
    }
}
