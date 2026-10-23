/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/8044ACCC_13.CPP, func_8044CFF8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;

typedef struct Shared_World {
    char pad0[0x18FE0];
    s32 previewState; /* 0x18FE0 (0x1B444 in Rage Wars) */
    s32 previewTimer; /* 0x18FE4 */
    s32 previewStart; /* 0x18FE8 */
} Shared_World;

typedef struct { s32 field_0; } Shape_8011AADC;
extern Shape_8011AADC D_8011AADC;
extern void func_0042C338(char *);
extern void func_0042C38C(char *);

void func_004652DC(Shared_World *world, s32 level) {
    func_0042C338((char *)world);
    func_0042C38C((char *)world);
    world->previewState = 0;
    world->previewTimer = 0x22;
    world->previewStart = D_8011AADC.field_0;
}
