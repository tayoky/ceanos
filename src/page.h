#define PS 4096
#define PTE 1024
#define PDE 1024


struct page_t_ent {
    unsigned int present;
    unsigned int rw;
    unsigned int user;
    unsigned int accessed;
    unsigned int dirty;
    unsigned int frame_addr;
}__attribute__((aligned(4096)));

struct page_d_ent {
    unsigned int present;
    unsigned int user;
    unsigned int accessed;
    unsigned int reserved;
    unsigned int page_table_addr;
}__attribute__((aligned(4096)));

struct page_d_ent page_dir[PDE];
struct page_t_ent page_tab[PTE][PDE];
