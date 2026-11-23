#include "common.h"
#include "structs/texture.h"
#include "globals/strings.h"
#include "protos/cache.h"

void Texture_DecompressCallback(void);

/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
sptr Texture_RequestSync(TextureRef *arg0) {
    return CCache__Request(0, arg0->unk0, arg0->unk4, 0, 0, &Texture_DecompressCallback, &sTextureCacheDesc_Texture, 1);
}
