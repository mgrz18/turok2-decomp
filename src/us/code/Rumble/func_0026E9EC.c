#include "common.h"
#include "structs/rumble.h"

/* Sets the pointer an effect clears when it ends (Rumble_Start's owner). Round 23: the member;
 * the old N64 view wrote an s32 at 0x38, amplitude.unk4 on the host. */
void Rumble_SetLink(RumbleEffect *effect, RumbleEffect **owner) {
    effect->owner = owner;
}
