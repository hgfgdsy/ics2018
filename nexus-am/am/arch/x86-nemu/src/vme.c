#include <x86.h>

#define PG_ALIGN __attribute((aligned(PGSIZE)))

static PDE kpdirs[NR_PDE] PG_ALIGN;
static PTE kptabs[PMEM_SIZE / PGSIZE] PG_ALIGN;
static void* (*pgalloc_usr)(size_t);
static void (*pgfree_usr)(void*);

_Area segments[] = {      // Kernel memory mappings
  {.start = (void*)0,          .end = (void*)PMEM_SIZE}
};

#define NR_KSEG_MAP (sizeof(segments) / sizeof(segments[0]))

int _vme_init(void* (*pgalloc_f)(size_t), void (*pgfree_f)(void*)) {
  pgalloc_usr = pgalloc_f;
  pgfree_usr = pgfree_f;

  int i;

  // make all PDEs invalid
  for (i = 0; i < NR_PDE; i ++) {
    kpdirs[i] = 0;
  }

  PTE *ptab = kptabs;
  for (i = 0; i < NR_KSEG_MAP; i ++) {
    uint32_t pdir_idx = (uintptr_t)segments[i].start / (PGSIZE * NR_PTE);
    uint32_t pdir_idx_end = (uintptr_t)segments[i].end / (PGSIZE * NR_PTE);
    for (; pdir_idx < pdir_idx_end; pdir_idx ++) {
      // fill PDE
      kpdirs[pdir_idx] = (uintptr_t)ptab | PTE_P;

      // fill PTE
      PTE pte = PGADDR(pdir_idx, 0, 0) | PTE_P;
      PTE pte_end = PGADDR(pdir_idx + 1, 0, 0) | PTE_P;
      for (; pte < pte_end; pte += PGSIZE) {
        *ptab = pte;
        ptab ++;
      }
    }
  }

  set_cr3(kpdirs);
  set_cr0(get_cr0() | CR0_PG);

  return 0;
}

int _protect(_Protect *p) {
  PDE *updir = (PDE*)(pgalloc_usr(1));
  p->pgsize = 4096;
  p->ptr = updir;
  // map kernel space
  for (int i = 0; i < NR_PDE; i ++) {
    updir[i] = kpdirs[i];
  }

  p->area.start = (void*)0x8000000;
  p->area.end = (void*)0xc0000000;
  return 0;
}

void _unprotect(_Protect *p) {
}

static _Protect *cur_as = NULL;
void get_cur_as(_Context *c) {
  c->prot = cur_as;
}

void _switch(_Context *c) {
  set_cr3(c->prot->ptr);
  cur_as = c->prot;
}

int _map(_Protect *p, void *va, void *pa, int mode) {
  uint32_t my_PDad = (uint32_t)p->ptr +((((uint32_t)va>>22)&0x3ff)<<2);
  uint32_t my_findPD = *(uint32_t *)((uintptr_t)my_PDad);
  if((my_findPD & PTE_P)==0){
	  void *my_newPT = pgalloc_usr(1);
	  *(uint32_t *)((uintptr_t)my_PDad) = ((uint32_t)my_newPT & ~0xfff) | PTE_P;
  }
  uint32_t my_findPT =( *(uint32_t *)((uintptr_t)my_PDad) ^ PTE_P)  +((((uint32_t)va>>12)&0x3ff)<<2);
  uint32_t my_final = ((uint32_t)pa & ~0xfff) | PTE_P;
  *(uint32_t *)((uintptr_t)my_findPT) = my_final;
   
  return 0;
}

_Context *_ucontext(_Protect *p, _Area ustack, _Area kstack, void *entry, void *args) {
  //return NULL;
  /*for(int i=1;i<=(1<<8);i++){
	  *(uintptr_t *)(ustack.end-(1<<2)) = 0;
  }*/
  void *my_end1 = ustack.end - (1<<10);
  _Context *my_context1 = (_Context *)my_end1;
  my_context1 -> cs =8;
  my_context1 -> eip = (uintptr_t)entry;
  my_context1 -> prot = p;
  my_context1 ->eflags = 0x200;
  return my_context1;
}
