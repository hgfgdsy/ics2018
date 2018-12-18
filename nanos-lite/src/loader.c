#include "proc.h"
#include "unistd.h"

#define DEFAULT_ENTRY 0x4000000

extern size_t ramdisk_read(void* buf, size_t offset, size_t len);
extern size_t get_ramdisk_size();
extern size_t ramdisk_write(const void* buf, size_t offset, size_t len);
extern int fs_open(const char *pathname, int flags, int mode);
extern ssize_t  fs_read(int fd, void *buf, size_t len);
extern size_t fs_filesz(int fd);

//extern void vaddr_write(vaddr_t addr, uint32_t data, int len);
//extern uint8_t pmem[128*1024*1024];

static void *buf = (void* ) DEFAULT_ENTRY;

static uintptr_t loader(PCB *pcb, const char *filename) {
  //TODO();
  //void *buf=(void*)DEFAULT_ENTRY;
  int fd=fs_open(filename,0,0);
  size_t len = fs_filesz(fd);
  fs_read(fd,buf,len);
  void* my_temp1 = buf;
  buf+=len;
  return (uintptr_t )my_temp1 ;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  ((void(*)())entry) ();
}

void context_kload(PCB *pcb, void *entry) {
  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _kcontext(stack, entry, NULL);
}

void context_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);

  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
}
