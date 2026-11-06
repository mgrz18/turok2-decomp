#include "common.h"
#include "structs/cinema.h"

extern Cinema *D_800C1BB0;

f32 Cinema_GetFarClip(void) {
    return D_800C1BB0->spec.scale * 10.24f;
}
