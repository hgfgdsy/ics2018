#include "common.h"
#include "syscall.h"

//extern void _halt(int code) __attribute__((__noreturn__));



_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;

  switch (a[0]) {
	  case SYS_write: if(a[1]==1||a[1]==2){
				  for(int i=0;i<a[3];i++)
					  _putc(*(char*)(a[2]+i));
				  c->GPRx=a[3];}
			  break;
	  case SYS_exit: _halt(c->GPRx); break;
	  case SYS_yield: _yield(); c->GPRx=0;  break;
	  default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
