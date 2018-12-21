#include <am.h>
#include <x86.h>

static _Context* (*user_handler)(_Event, _Context*) = NULL;

void vectrap();
void vecnull();
void vecsys();
extern void get_cur_as(_Context *c);
extern void _switch(_Context *c);

_Context* irq_handle(_Context *tf) {
  get_cur_as(tf);
  _Context *next = tf;
  //printf("%d\n",tf->eflags);
  if (user_handler) {
    _Event ev = {0};
    switch (tf->irq) {
      case 0x80:ev.event = _EVENT_SYSCALL; break;
      case 0x81:ev.event = _EVENT_YIELD; break;
      default: ev.event = _EVENT_ERROR; break;
    }
    next = user_handler(ev, tf);
    if (next == NULL) {
      next = tf;
    }
  }
  _switch(next);
  return next;
}

static GateDesc idt[NR_IRQ];

int _cte_init(_Context*(*handler)(_Event, _Context*)) {
  // initialize IDT
  for (unsigned int i = 0; i < NR_IRQ; i ++) {
    idt[i] = GATE(STS_TG32, KSEL(SEG_KCODE), vecnull, DPL_KERN);
  }

  // -------------------- system call --------------------------
  idt[0x80] = GATE(STS_TG32, KSEL(SEG_KCODE), vecsys, DPL_KERN); 
  idt[0x81] = GATE(STS_TG32, KSEL(SEG_KCODE), vectrap, DPL_KERN);
  
  set_idt(idt, sizeof(idt));

  // register event handler
  user_handler = handler;

  return 0;
}

_Context *_kcontext(_Area stack, void (*entry)(void *), void *arg) {
  //return NULL;
  void *my_end = stack.end - sizeof(_Context);
  _Context *my_context = (_Context *)my_end;
  my_context->cs = 8;
  my_context->eip = (uintptr_t)entry;
  //void *my_end = stack.end - sizeof(my_context);
  //*(uintptr_t *)(stack.end-11) = my_context.cs;
  //*(uintptr_t *)(stack.end-2) = my_context.eip;
  return my_context;
}

void _yield() {
  asm volatile("int $0x81");
}

int _intr_read() {
  return 0;
}

void _intr_write(int enable) {
}
