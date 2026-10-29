typedef int s32;
typedef signed char s8;
typedef unsigned char u8;

typedef struct {
    char pad0[0x2F];
    s8 style;                   /* 0x2F */
    char pad30[0x33 - 0x30];
    u8 colors[8];               /* 0x33 */
} Widget;

extern u8 D_800B5E80[12];
extern u8 D_800B5E8C[12];
extern u8 D_800B5E98[12];
extern u8 D_800B5EA4[12];

void func_0041BC50(Widget *w) {
    s32 i;

    switch (w->style) {
    case 0:
    default:
        for (i = 0; i < 8; i++) {
            w->colors[i] = D_800B5E80[i];
        }
        break;
    case 1:
        for (i = 0; i < 8; i++) {
            w->colors[i] = D_800B5E8C[i];
        }
        break;
    case 2:
        for (i = 0; i < 8; i++) {
            w->colors[i] = D_800B5E98[i];
        }
        break;
    case 3:
        for (i = 0; i < 8; i++) {
            w->colors[i] = D_800B5EA4[i];
        }
        break;
    }
}
