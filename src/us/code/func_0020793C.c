#include "common.h"
#include "protos/cache.h"

extern s32 Texture_DecompressCallback;
extern s32 D_800A51F0;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
} Pair;

#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
sptr Texture_RequestSync(Pair *arg0) {
    return CCache__Request(0, arg0->unk0, arg0->unk4, 0, 0, &Texture_DecompressCallback, &D_800A51F0, 1);
}
#else
void Texture_RequestSync(Pair *arg0) {
    CCache__Request(0, arg0->unk0, arg0->unk4, 0, 0, &Texture_DecompressCallback, &D_800A51F0, 1);
}
#endif
