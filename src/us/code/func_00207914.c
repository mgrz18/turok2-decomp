#include "common.h"
#include "m2c_macros.h"
#include "globals/scene.h"
#include "protos/scene.h"


/* arg0: &Material.texture (the ROM address, then its size at +4): an address, sptr on the host. */
void func_00207914(sptr arg0, s32 arg1) {
    CScene__GetTextureSet(&gScene, arg1, (s32 *)arg0, arg0 + 4);
}
