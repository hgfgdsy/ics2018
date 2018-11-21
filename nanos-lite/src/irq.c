#include "common.h"

extern _Context* do_syscall(_Context *c); 

static _Context* do_event(_Event e, _Context* c) {
 // assert(0);
  switch (e.event) {
    case  _EVENT_SYSCALL: printf("nanshou!!!!!");
			  do_syscall(c);
			  break; 
    case  _EVENT_YIELD: printf("nanshou!!\n"); break;
    default: panic("Unhandled event ID = %d", e.event);
  }
 // assert(0);

  return NULL;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  _cte_init(do_event);
}
