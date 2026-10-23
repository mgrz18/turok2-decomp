/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER_4/CODE_8028C490.CPP, func_8028CC34_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py.
 * The draft had only a prototype; body written from the Turok 2 asm. */

typedef struct ActorRec {
    char pad[0x1A8];
    struct ActorRec *link; /* 0x1A8 (0x1D8 in Rage Wars) */
} ActorRec;

typedef struct ActorList {
    char pad[0xBF8];
    ActorRec *actors[0x80]; /* 0xBF8 */
    int count;              /* 0xDF8 */
} ActorList;

/* Detaches the first actor linked to `id` and relinks every other actor
 * linked to `id` onto it; returns the detached actor. */
ActorRec *func_00225AD4(ActorList *obj, ActorRec *id) {
    int i;
    ActorRec *found;
    int count;
    ActorRec **actors;

    i = 0;
    found = 0;
    count = obj->count;
    actors = obj->actors;
    for (; i < count; i++) {
        if (actors[i]->link == id) {
            found = actors[i];
            found->link = 0;
            break;
        }
    }
    for (i = 0; i < count; i++) {
        if (actors[i]->link == id) {
            actors[i]->link = found;
        }
    }
    return found;
}
