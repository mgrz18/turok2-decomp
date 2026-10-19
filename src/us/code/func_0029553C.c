/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/libultra/alEnvmixerNew.c, alEnvmixerNew.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef struct {
    u8 *base;
    u8 *cur;
    s32 len;
    s32 count;
} ALHeap;
typedef struct ALFilter_s {
    struct ALFilter_s *source;
    void *(*handler)(struct ALFilter_s *filter, s16 *outp, s32 outCount, s32 sampleOffset, void *cmdp);
    s32 (*setParam)(struct ALFilter_s *filter, s32 type, void *param);
    s16 inp;
    s16 outp;
    s32 type;
} ALFilter;
typedef struct {
    ALFilter filter;
    void *state;
    s16 f18;
    s16 f1A;
    s16 f1C;
    s16 f1E;
    s16 f20;
    s16 f22;
    union {
        s16 a;
        volatile s16 b;
    } f24;
    s16 f26;
    s16 f28;
    s16 f2A;
    s16 f2C;
    s16 f2E;
    s32 f30;
    s32 f34;
    s32 first;
    s32 f3C;
    s32 f40;
    s32 f44;
    s32 motion;
} ALEnvMixer;
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
