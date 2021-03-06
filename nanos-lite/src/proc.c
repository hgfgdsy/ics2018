#include "proc.h"

#define MAX_NR_PROC 4

extern void naive_uload(PCB *pcb, const char *filename);
extern void context_kload(PCB *pcb, void *entry); 
extern void context_uload(PCB *pcb, const char *filename); 

static PCB pcb[MAX_NR_PROC] __attribute__((used));
static PCB pcb_boot;
PCB *current;

void switch_boot_pcb() {
  current = &pcb_boot;
}

void hello_fun(void *arg) {
  int j = 1;
  while (1) {
    Log("Hello World from Nanos-lite for the %dth time!", j);
    j ++;
    _yield();
  }
}
int fg_pcb;
void init_proc() {
  //naive_uload(NULL,"/bin/init");
  //context_kload(&pcb[0],(void *)hello_fun);
  fg_pcb=0;
  context_uload(&pcb[0], "/bin/pal");
  context_uload(&pcb[1], "/bin/hello");
  context_uload(&pcb[2], "/bin/pal");
  context_uload(&pcb[3], "/bin/pal");
  switch_boot_pcb();
}

_Context* schedule(_Context *prev) {
  //return NULL;
  current -> cp = prev;
  //current = &pcb[0];
  current = (current == &pcb[fg_pcb] ? &pcb[1] : &pcb[fg_pcb]);
  return current -> cp;
}

void sys_execve(const char* fname){
	naive_uload(NULL,fname);
}
