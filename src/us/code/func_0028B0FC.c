/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8028DF90_de_7.c, func_8028FA60_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef float f32;
struct OSMesgQueue;
typedef struct OSMesgQueue OSMesgQueue;
struct OSMesgQueue {
    void **mtqueue;
    void **fullqueue;
    s32 validCount;
    s32 first;
    s32 msgCount;
    void **msg;
};
struct OSMesgQueue;
struct OSMesgQueue;
struct OSMesgQueue;
struct View_func_8028FA60_de;
typedef struct View_func_8028FA60_de View_func_8028FA60_de;
extern int D_800B6E60;
struct Scene_func_8028FA60_de;
struct Scene_func_8028FA60_de {
    s32 pad0;
    s32 flags;
    s32 options;
    s32 padC;
    s32 mode;
    char pad14[0x38 - 0x14];
    s32 music;
    s64 *time;
};
struct Scene_func_8028FA60_de;
struct View_func_8028FA60_de;
struct View_func_8028FA60_de {
    char pad0[0x78];
    struct OSMesgQueue messages;
    char pad90[0x2F4 - 0x90];
    struct Scene_func_8028FA60_de *current;
    struct Scene_func_8028FA60_de *next;
    char pad2FC[4];
    s32 pending;
};
struct Scene_func_8028FA60_de;
typedef struct Scene_func_8028FA60_de Scene_func_8028FA60_de;
struct Scene_func_8028FA60_de;
struct View_func_8028FA60_de;
extern void func_0028B0FC(struct View_func_8028FA60_de *, struct Scene_func_8028FA60_de *, struct Scene_func_8028FA60_de *);
struct OSMesgQueue;
struct OSMesgQueue;
extern s64 D_8011F170;
extern f32 D_8011F158;
extern s64 D_8011F168;
extern s32 D_800B6E64;
extern u64 D_8011F150;
extern u64 D_8011F160;
extern void func_0029E3B0();
extern u64 D_8011F150;
extern f32 D_8011F158;
extern u64 D_8011F160;
extern s64 D_8011F168;
extern s64 D_8011F170;
extern s32 D_800B6E64;
extern s64 func_0029B170(void);
extern void func_0029A248(s32 *mode);
extern void func_0029A34C(s32 *mode);
extern void func_00298D50(s32 music, s64 time);
void func_0028B0FC(View_func_8028FA60_de *view, Scene_func_8028FA60_de *current, Scene_func_8028FA60_de *next) {
    s64 now;
    if (next != 0 && (next->options & 0x20) && !(next->flags & 0x10)) {
        D_8011F168 = func_0029B170();
    }
    if (current != 0) {
        if (current->mode == 2) {
            func_0029E3B0();
            D_8011F150 = func_0029B170();
        } else if (current->options & 0x20) {
            now = func_0029B170();
            D_8011F160 = now;
            if (!(current->flags & 0x10)) {
                D_8011F170 = now;
                D_8011F158 = 0;
            }
        }
        current->flags &= ~0x30;
        func_0029A248(&current->mode);
        func_0029A34C(&current->mode);
        view->current = current;
        if (current == next) {
            view->next = next;
        }
    }
    if (next != 0 && next != current) {
        func_00298D50(next->music, *next->time);
        D_800B6E60 = 1;
        D_800B6E64 = 0;
        view->next = next;
    }
}
