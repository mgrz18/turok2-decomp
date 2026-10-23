/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/WEAPONS/80225D10_10.CPP, func_8022A5C0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;

/* Turok 2 player record (Rage Wars: controller 0x698, next 0x16E0). */
typedef struct PlayerRecord {
    char pad0[0xA50];
    s32 controller;             /* 0xA50 */
    char padA54[0x1320 - 0xA54];
    struct PlayerRecord *next;  /* 0x1320 */
} PlayerRecord;

typedef struct Round {
    char pad0[0x20];
    PlayerRecord *players;      /* 0x20 */
} Round;

/* Find the player record driven by controller arg1. */
void *func_0025342C(char *object, s32 arg1) {
    Round *round = (Round *)object;
    PlayerRecord *record = round->players;
    if (record != 0) {
        do {
            if (record->controller == arg1) {
                return record;
            }
            record = record->next;
        } while (record != 0);
    }
    return 0;
}
