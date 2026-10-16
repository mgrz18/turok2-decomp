/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802591A0_de_7.c, func_80259A50_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
struct NodeRecAtAC;
typedef struct NodeRecAtAC NodeRecAtAC;
struct NodeRecAtAC;
struct NodeRecAtAC {
    struct NodeRecAtAC *prev;
    struct NodeRecAtAC *next;
    char pad8[0xA4];
    s32 flags;
};
struct Lists;
struct Lists {
    char pad0[4];
    NodeRecAtAC first;
    char padB4[0x24];
    NodeRecAtAC second;
};
struct Lists;
typedef struct Lists Lists;
void func_00276790(Lists *arg0, s32 arg1) {
    NodeRecAtAC *var_a2;
    NodeRecAtAC *temp_a3;
    NodeRecAtAC *temp_v0;
    NodeRecAtAC *temp_t0;
    NodeRecAtAC *temp_t1;
    var_a2 = arg0->first.next;
    temp_v0 = &arg0->first;
    if (var_a2 != temp_v0) {
        temp_t1 = &arg0->second;
        temp_t0 = temp_v0;
        do {
            temp_a3 = var_a2->next;
            if ((var_a2->flags & arg1) != 0) {
                NodeRecAtAC *temp_tail;
                var_a2->prev->next = temp_a3;
                var_a2->next->prev = var_a2->prev;
                temp_tail = arg0->second.prev;
                var_a2->next = temp_t1;
                var_a2->prev = temp_tail;
                arg0->second.prev->next = var_a2;
                arg0->second.prev = var_a2;
            }
            var_a2 = temp_a3;
        } while (var_a2 != temp_t0);
    }
}
