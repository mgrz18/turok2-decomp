#include "common.h"
#include "globals/strings.h"
#include "protos/cache.h"

void Texture_DecompressCallback(void);

typedef struct {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 unk4;
} Unk0020798C;

/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
void * Texture_Request(Unk0020798C *arg0) {
    return CCache__Request(0, arg0->unk0, arg0->unk4, 0, 0, &Texture_DecompressCallback, &sTextureCacheDesc_Texture, 0);
}
