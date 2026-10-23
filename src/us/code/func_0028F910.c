typedef unsigned int u32;
typedef int s32;

typedef struct {
    char pad0[0x4];
    u32 mask;                   /* 0x04 */
    char pad8[0x10 - 0x8];
    u32 elapsed;                /* 0x10 */
    s32 pending;                /* 0x14 */
    char pad18[0xB8 - 0x18];
    u32 timers[16];             /* 0xB8 */
} Timers;

s32 func_0028F910(Timers *t, u32 *out) {
    u32 elapsed = t->elapsed;
    u32 min = -1;
    u32 i;

    if (t->mask == 0) {
        return 0;
    }
    for (i = 0; i < 16; i++) {
        if ((t->mask >> i) & 1) {
            if (t->pending != 0) {
                t->timers[i] -= elapsed;
            }
            if (t->timers[i] < min) {
                min = t->timers[i];
            }
        }
    }
    t->pending = 0;
    *out = min;
    return 1;
}
