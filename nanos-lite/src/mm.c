#include "memory.h"
#include "proc.h"

static void *pf = NULL;

void* new_page(size_t nr_page) {
  void *p = pf;
  pf += PGSIZE * nr_page;
  assert(pf < (void *)_heap.end);
  return p;
}

void free_page(void *p) {
  panic("not implement yet");
}

/* The brk() system call handler. */
int mm_brk(uintptr_t new_brk) {
  if(current -> max_brk < new_brk && (current -> max_brk & ~0xfff) != (new_brk & ~0xfff)){
	  uint32_t my_glory = (current -> max_brk & ~0xfff) + (1<<12);
	  uint32_t my_difference = (uint32_t)(new_brk - my_glory);
	  uint32_t my_count1 = my_difference & 0xfff;
	  uint32_t my_count2 = my_difference >> 12;
	  void *my_adp,*my_adv;
	  for(uint32_t i = 0; i < my_count2; i++){
		  my_adp = new_page(1);
		  my_adv = (void *)(uintptr_t)(my_glory + (i<<12));
		  _map(&current->as,my_adv,my_adp,1);
	  }
	  if(my_count1){
                  my_adp = new_page(1);
		  my_adv = (void *)(uintptr_t)(my_glory + (my_count2<<12));
		  _map(&current->as,my_adv,my_adp,1);
	  }
	  current -> max_brk = new_brk;
  }
  if(current -> max_brk < new_brk) current -> max_brk = new_brk;
  current -> cur_brk = new_brk;
  return 0;
}

void init_mm() {
  pf = (void *)PGROUNDUP((uintptr_t)_heap.start);
  Log("free physical pages starting from %p", pf);

  _vme_init(new_page, free_page);
}
