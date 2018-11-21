#include "common.h"
#include "syscall.h"

//extern void _halt(int code) __attribute__((__noreturn__));



_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;

  switch (a[0]) {
	  case SYS_write: if(c->GPR2==1||c->GPR2==2){
				  for(int i=0;i<c->GPR4;i++)
					  _putc(*(char*)(c->GPR3+i));
				  c->GPRx=c->GPR4;}
			  break;
	  case SYS_exit: _halt(c->GPRx); break;
	  case SYS_yield: _yield(); c->GPRx=0;  break;
	  default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
