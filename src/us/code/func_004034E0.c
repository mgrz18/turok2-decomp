typedef signed int s32;

typedef struct {
    /* 0x00 */ s32 ptr;
    /* 0x04 */ s32 size;
} Chunk;

typedef struct {
    /* 0x000 */ char pad0[0x214];
    /* 0x214 */ Chunk *chunk;
    /* 0x218 */ s32 total;
    /* 0x21C */ s32 pos;
    /* 0x220 */ char pad220[0x30];
    /* 0x250 */ s32 valid;
} Reader;

void func_004034E0(Reader *r, Chunk *c) {
    r->chunk = c;
    r->total += c->size;
    r->pos = 0;
    if (c->ptr != 0 && c->size != 0) {
        r->valid = 1;
    } else {
        r->valid = 0;
    }
}
