/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/TEAM/80217388_4.CPP, func_80217B3C_de, with Turok 2's layouts. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;

#define ABS(a) (((a) < 0.0f) ? -(a) : (a))

typedef struct {
    /* 0x00 */ char pad0[0xC6];
    /* 0xC6 */ s8 stickX;
    /* 0xC7 */ s8 stickY;
} Pad;

typedef struct {
    /* 0x000 */ char pad0[0xA50];
    /* 0xA50 */ Pad *pad;
} Player;

extern u8 D_8011AAF1;

/* Returns the 8-way stick direction (0 = up, clockwise) or -1 inside the dead zone. */
s32 func_0024B88C(s32 unused, Player *player) {
    Pad *pad = player->pad;
    s8 x = pad->stickX;
    s8 y = pad->stickY;

    if (x * x + y * y < 3600) {
        return -1;
    }
    if (ABS(ABS(x) - ABS(y)) < 46 && x != 0 && y != 0 && !D_8011AAF1) {
        if (x * y > 0) {
            if (x > 0) {
                return 1;
            }
            /* Redundant re-read kept from the original (Rage Wars has it too): it shapes the branch layout. */
            if (pad->stickX != 0) {
                return 5;
            }
            return 5;
        }
        return y > 0 ? 7 : 3;
    }
    if (ABS(x) > ABS(y)) {
        return x <= 0 ? 6 : 2;
    }
    if (y > 0) {
        return 0;
    }
    return 4;
}
