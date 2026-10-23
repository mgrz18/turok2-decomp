/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU_2/80408AC4_7.CPP, func_8043CC10_de, with Turok 2's layouts. */
typedef signed int s32;
typedef unsigned char u8;

typedef struct {
    /* 0x00 */ s32 value; /* cursor */
    /* 0x04 */ s32 timer;
    /* 0x08 */ s32 count;
    /* 0x0C */ u8 text[24];
} Shared_Entry;

typedef struct {
    /* 0x00 */ char pad0[0x14];
    /* 0x14 */ u8 *chr;
} ObjRecAt14;

extern s32 D_80130920;
extern Shared_Entry D_800C1F80;

/* Inserts the selected character at the cursor of the name entry. */
s32 func_00453AE8(ObjRecAt14 *arg0) {
    s32 index;
    s32 count;

    D_80130920 = -1;
    D_800C1F80.timer = 0;
    if (D_800C1F80.count == 0x16) {
        return 0;
    }
    index = 0x16;
    if (D_800C1F80.value < 0x16) {
        do {
            D_800C1F80.text[index] = D_800C1F80.text[index - 1];
            index--;
        } while (D_800C1F80.value < index);
    }
    D_800C1F80.text[D_800C1F80.value] = *arg0->chr;
    count = D_800C1F80.count + 1;
    D_800C1F80.value++;
    D_800C1F80.count = count;
    D_800C1F80.text[count] = 0;
    return 0;
}
