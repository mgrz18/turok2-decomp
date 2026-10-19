/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/libultra/alEnvmixerNew.c, alEnvmixerNew.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

void func_00296340(ALFilter *f, void *handler, void *setParam, s32 type);
void *func_002912A0(u8 *file, s32 line, ALHeap *hp, s32 num, s32 size);
void *D_00295690(void);
s32 D_00296118(void *filter, s32 paramID, void *param);
void func_0029553C(ALEnvMixer *e, ALHeap *hp) {
    func_00296340(&e->filter, D_00295690, D_00296118, 4);
    e->state = func_002912A0(0, 0, hp, 1, 0x50);
    e->first = 1;
    e->motion = 0;
    e->f1A = 1;
    e->f28 = 1;
    e->f2E = 1;
    e->f1C = 1;
    e->f1E = 1;
    e->f20 = 0;
    e->f22 = 0;
    e->f26 = 1;
    e->f24.a = 0;
    e->f24.b = 0;
    e->f30 = 0;
    e->f34 = 0;
    e->f18 = 0;
    e->f3C = 0;
    e->f40 = 0;
    e->f44 = 0;
}
