/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/8043D1DC_11.CPP, func_8043D278_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef float f32;
struct TextBoundsText;
extern s32 D_800B57AC;
extern s32 D_800B57C8;
struct TextBoundsDescriptor;
typedef struct TextBoundsDescriptor TextBoundsDescriptor;
struct TextBoundsMetrics;
typedef struct TextBoundsMetrics TextBoundsMetrics;
struct TextBoundsText;
typedef struct TextBoundsText TextBoundsText;
struct TextBoundsDescriptor;
struct TextBoundsDescriptor {
    char pad0[0x44];
    s8 * unk_44;
};
struct TextBoundsMetrics;
struct TextBoundsMetrics {
    s32 field0;
    s32 width;
    s32 height;
    f32 scaleX;
    f32 scaleY;
    s32 left;
    s32 right;
    s32 top;
    s32 bottom;
};
struct TextBoundsText;
struct TextBoundsHandlers {
    char pad0[0x14];
    void (*draw)(TextBoundsText *, TextBoundsMetrics *, TextBoundsMetrics *, void *);
};
struct TextBoundsText {
    s32 value;
    s16 mode;
    s16 reserved;
    s32 flags;
    s16 x;
    s16 y;
    s32 field10;
    void *text;
    struct TextBoundsHandlers *field18;
    s32 spacing;
    s32 field20;
    struct TextBoundsDescriptor *descriptor;
};
extern s32 D_8011AAD8;
extern s32 D_8011AADC;
extern s32 D_800B57AC;
extern s32 D_800B57C8;
s32 func_0045414C(TextBoundsText *arg0) {
    if (D_8011AADC & 1) {
        arg0->flags |= 0x01000000;
    } else {
        arg0->flags &= 0xFEFFFFFF;
    }
    if (D_8011AAD8 & 1) {
        arg0->text = &D_800B57AC;
    } else {
        arg0->text = &D_800B57C8;
    }
    return 0;
}
extern s32 D_8011AAD8;
