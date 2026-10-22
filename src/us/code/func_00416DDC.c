/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80442BC8.CPP, func_80442D10_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
struct WidgetDescriptor;
struct WidgetDescriptor {
    short type;
    short pad;
    int flags;
    unsigned short x;
    unsigned short y;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
    int value;
};
struct ObjectRecAt478;
struct ObjectRecAt478 {
    char pad[0x328];
    s32 flag;
};
struct ObjectRecAt478;
struct WidgetDescriptor;
struct CMenuItem_t;
struct CMenuItem_t {
    int id;
    unsigned short type;
    unsigned short pad;
    int flags;
    unsigned short x;
    unsigned short y;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
    int value;
    struct WidgetDescriptor *desc;
    int index;
    void *extra;
    int m_pMenu;
};
struct WidgetDescriptor;
typedef struct WidgetDescriptor WidgetDescriptor;
struct CMenuItem_t;
typedef struct CMenuItem_t CMenuItem;
extern void func_00416DDC(CMenuItem *w, WidgetDescriptor *d, char **arena, int id, int arg);
void func_00416DDC(CMenuItem *w, WidgetDescriptor *d, char **arena, int id, int arg) {
 int size; char *extra;
 w->id=id; w->type=d->type; w->flags=d->flags|0x1800000;
 w->x=d->x; w->y=d->y; w->r=d->r; w->g=d->g; w->b=d->b; w->a=d->a;
 w->value=d->value; w->desc=d; w->index=-1; w->m_pMenu=arg;
 size=0; if(d->type==3) size=0x330;
 if(size) {
 w->extra=*arena;
 extra=*arena; if(d->type==3) ((struct ObjectRecAt478 *)(extra))->flag=1;
 { int step=0; if(d->type==3) step=0x330;
 *arena+=step; }
 }
}
