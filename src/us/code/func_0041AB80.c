typedef signed int s32;
typedef unsigned char u8;

typedef struct {
    /* 0x00 */ s32 cursor;
    /* 0x04 */ s32 flag;
    /* 0x08 */ s32 len;
} TextEdit;

typedef struct {
    /* 0x00 */ char pad0[0x14];
    /* 0x14 */ u8 *key;
} KeyEvent;

extern TextEdit D_800C1F80;
extern u8 D_800C1F8C[];
extern s32 D_80130920;

/* Inserts the event's character at the cursor (max 22 chars). */
s32 func_0041AB80(KeyEvent *ev) {
    s32 i;

    D_80130920 = -1;
    D_800C1F80.flag = 0;
    if (D_800C1F80.len == 22) {
        return 0;
    }
    for (i = 22; D_800C1F80.cursor < i; i--) {
        D_800C1F8C[i] = D_800C1F8C[i - 1];
    }
    D_800C1F8C[D_800C1F80.cursor] = *ev->key;
    D_800C1F80.cursor++;
    D_800C1F80.len++;
    D_800C1F8C[D_800C1F80.len] = 0;
    return 0;
}
