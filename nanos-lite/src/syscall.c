#include "common.h"
#include "syscall.h"
#include "unistd.h"

//extern void _halt(int code) __attribute__((__noreturn__));
uintptr_t sys_write(uintptr_t a,uintptr_t b,uintptr_t c); 
intptr_t sys_brk(uintptr_t addr);
extern int fs_open(const char* pathname, int flags, int mode);
extern ssize_t fs_read(int fd, void *buf, size_t len);
extern ssize_t fs_write(int fd, const void *buf, size_t len);
extern off_t fs_lseek(int fd, off_t offset, int whence);
extern int fs_close(int fd);
extern void sys_execve(const char* fname);
extern _Context* schedule(_Context *prev);  

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;

  switch (a[0]) {
	 // case SYS_exit: sys_execve("/bin/init"); break;
	  case SYS_execve: sys_execve((char*)a[1]); break;
	  case SYS_lseek: c-> GPRx = fs_lseek((int) a[1], (off_t)a[2], (int) a[3]); break; 
	  case SYS_close: c->GPRx = fs_close((int)a[1]); break;
	  case SYS_read: c->GPRx = fs_read((int)a[1], (void*)a[2], (size_t)a[3]); break;
	  case SYS_open: c->GPRx = fs_open((const char*)a[1], (int) a[2], a[3]); break; 
	  case SYS_brk: c->GPRx=sys_brk(a[1]); break;
	  case SYS_write: /*if(a[1]==1||a[1]==2){
				  for(int i=0;i<a[3];i++)
					  _putc(*(char*)(a[2]+i));
				  c->GPRx=a[3];}
			  else if(a[1]>=3)*/
		          c->GPRx=fs_write((int)a[1],(const void*)a[2],(size_t)a[3]);
			  break;
	  case SYS_exit: _halt(c->GPRx);
			   
			  break;
	  case SYS_yield: _yield(); c->GPRx=0;
			  break;
	  default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}

uintptr_t sys_write(uintptr_t a,uintptr_t b,uintptr_t c) {
                            if(a==1||a==2){
				  for(int i=0;i<c;i++)
					  _putc(*(char*)(b+i));
			    }
			    Log("PPP");
			    return c;
}

intptr_t sys_brk(uintptr_t addr) {
	return 0;}


