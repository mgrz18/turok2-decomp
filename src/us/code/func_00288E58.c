/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER/80293A04.CPP, func_80293A20_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef signed int s32;
typedef float f32;

/* Turok 2: the mode block sits at 0x23FD0 (Rage Wars 0x26DB0). */
typedef struct GameSession {
    char pad0[0x23FD0];
    f32 transitionTime;        /* 0x23FD0 */
    s32 previousMode;          /* 0x23FD4 */
    s32 mode;                  /* 0x23FD8 */
    s32 target;                /* 0x23FDC */
    char pad23FE0;
    s8 phase;                  /* 0x23FE1 */
    char pad23FE2[2];
    f32 timer;                 /* 0x23FE4 */
} GameSession;

extern s32 D_800F5D2C;
extern s32 func_00284204(s32 *flags);
extern s32 func_0028D0E0(void);

void func_00288E58(GameSession *session, s32 arg1, s32 target) {
    s32 pending;
    s32 *flags;
    flags = &D_800F5D2C;
    pending = *flags & 0x1000;
    if ((unsigned)(session->mode - 0xB) < 2) {
        pending = 0;
    }
    if (pending != 0 && target != -1) {
        session->phase = 2;
        session->target = target;
        *flags = 0;
        return;
    }
    if (func_00284204(flags) != 0) {
        s32 mode = session->mode;
        session->phase = 1;
        session->timer = 0;
        session->mode = 0x11;
        session->target = arg1;
        session->previousMode = mode;
        if (func_0028D0E0() != 0) {
            session->timer = (1.0f);
        }
    }
}
