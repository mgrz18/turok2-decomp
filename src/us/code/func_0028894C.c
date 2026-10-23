/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_002935A4.c, func_002935A4, with Turok 2's layouts. */
typedef unsigned char u8;
typedef signed int s32;
typedef float f32;

typedef struct {
    /* 0x00000 */ u8 pad0[0x23FD4];
    /* 0x23FD4 */ s32 prevMode;
    /* 0x23FD8 */ s32 mode;
    /* 0x23FDC */ s32 unk23FDC;
    /* 0x23FE0 */ u8 fast;
    /* 0x23FE1 */ u8 fadeState; /* 0 idle, 1 fading in, 2 fading out */
    /* 0x23FE2 */ u8 pad23FE2[2];
    /* 0x23FE4 */ f32 fade;
} Game28894C;

extern f32 D_800B6D28; /* frame delta */

void func_0028894C(Game28894C *g) {
    f32 speed;
    f32 kk;

    if (g->fadeState != 0) {
        speed = 0.1f;
        if (g->fast != 0) {
            /* Redundant branches kept: they split the blocks so fadeState is reloaded. */
            if (g || D_800B6D28) {
                speed = 0.5f;
            } else {
                speed = 0.5f;
            }
        }
        if (g) {
            kk = speed;
        } else {
            kk = speed;
        }
        if (g->fadeState != 1) {
            if (g->fadeState == 2) {
                if (0.0f < g->fade) {
                    g->fade = g->fade - (kk * D_800B6D28);
                    if (g->fade < 0.0f) {
                        g->fade = 0.0f;
                    }
                } else {
                    g->fade = g->fade - 1.0f;
                }
                if (g->fade < -4.0f) {
                    s32 old = g->mode;
                    g->mode = 0x11;
                    g->fade = 0.0f;
                    g->fadeState = 1;
                    g->prevMode = old;
                }
            }
        } else {
            g->fade = g->fade + (speed * ((D_800B6D28 < 0.5f) ? 0.5f : D_800B6D28));
            if (1.0f <= g->fade) {
                g->fadeState = 0;
                g->fade = 1.0f;
                g->fast = 0;
            }
        }
    }
}
