/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80254D44_de_16.c, func_80255B2C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef unsigned int u32;
struct Block80255720;
struct Block80255720 {
    struct Block80255720 *prev;
    struct Block80255720 *next;
    struct Block80255720 *prev_phys;
    struct Block80255720 *next_phys;
    s32 offset;
    s32 size;
};
struct Block80255720;
typedef struct Block80255720 Block80255720;
struct Pool80255ACC;
struct Pool80255ACC {
    s32 unk0;
    s32 unk4;
    Block80255720 *head;
    Block80255720 *tail;
};
struct Block80255720;
struct Pool80255ACC;
typedef struct Pool80255ACC Pool80255ACC;
s32 func_002026EC(Pool80255ACC *arg0, char *arg1) {
    Block80255720 *block;
    Block80255720 *header;
    Block80255720 *other;
    Block80255720 *scan;
    u32 next;
    header = (Block80255720 *)(arg1 - 0x20);
    block = header->prev_phys;
    if (block->size == 0) {
        other = header->prev;
        if (other != 0) {
            block->prev = other;
        } else {
            scan = block;
            while (scan != 0) {
                if (scan->size != 0) {
                    break;
                }
                scan = scan->prev_phys;
            }
            if (scan != 0) {
                block->prev = scan;
            } else {
                goto insert_head;
            }
            other = scan;
        }
        next = (u32)other->next;
        block->next = (Block80255720 *)next;
        if (other->next != 0) {
            *(Block80255720 **)next = block;
        }
        other->next = block;
        if (arg0->tail == other) {
            arg0->tail = block;
        }
        goto merge;
insert_head:
        block->next = arg0->head;
        if (arg0->head != 0) {
            arg0->head->prev = block;
        } else {
            arg0->tail = block;
        }
        arg0->head = block;
    }
merge:
    block->size += header->offset + header->size;
    if (header->next_phys != 0) {
        header->next_phys->prev_phys = header->prev_phys;
    }
    header->prev_phys->next_phys = header->next_phys;
    if (header->size != 0) {
        if (header->prev != 0) {
            header->prev->next = header->next;
        }
        if (header->next != 0) {
            header->next->prev = header->prev;
        }
        if (arg0->tail == header) {
            arg0->tail = header->prev;
        }
    }
    return block->size - 0x20;
}
