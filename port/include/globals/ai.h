/* Port shadow of include/globals/ai.h. Its mode tables are arrays of
 * struct CAIModeInfo, which it only forward-declares: SN64 GCC 2.8 accepts an
 * extern array of an incomplete type, clang does not. Pull the full type in
 * first, then the real header. */
#include "structs/game_object.h"
#include_next "globals/ai.h"
