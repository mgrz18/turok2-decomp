/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80258800_de.c, func_80259038_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed short s16;
typedef signed int s32;
struct SceneRecAt2B5C;
struct SceneRecAt2B5C {
    char pad0[0x130];
    s32 lastPick;
    char pad134[0x2B54 - 0x134];
    s32 count;
    s32 ids;
    s32 *weights;
};
struct SceneRecAt2B5C;
typedef struct SceneRecAt2B5C SceneRecAt2B5C;
extern s32 func_00271BEC(SceneRecAt2B5C *scene, s16 id, s16 avoid);
extern s32 func_00266D8C(s32, s32, s32, s32 *, s32 *);
extern s32 func_002113A4(void);
s32 func_00271BEC(SceneRecAt2B5C *scene, s16 id, s16 avoid) {
    s32 first;
    s32 last;
    s32 total;
    s32 entry;
    s32 draw;
    s32 i;
    total = 0;
    if (scene->count != 0) {
        if (func_00266D8C(scene->ids, scene->count, id, &first, &last) == 0) {
            return -1;
        }
        entry = first;
        if (entry != last) {
            for (i = first; i <= last; i++) {
                total += scene->weights[i] * 10;
            }
            draw = func_002113A4() % total;
            total = 0;
            for (entry = first; entry < last; entry++) {
                total += scene->weights[entry] * 10;
                if (total >= draw) {
                        break;
                }
            }
            if (avoid != 0 && entry == scene->lastPick) {
                if (entry == last) {
                        entry--;
                } else {
                        entry++;
                }
            }
        }
        return entry;
    }
    return -1;
}
