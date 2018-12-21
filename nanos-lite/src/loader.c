#include "proc.h"
#include "unistd.h"

#define DEFAULT_ENTRY 0x8048000

extern size_t ramdisk_read(void* buf, size_t offset, size_t len);
extern size_t get_ramdisk_size();
extern size_t ramdisk_write(const void* buf, size_t offset, size_t len);
extern int fs_open(const char *pathname, int flags, int mode);
extern ssize_t  fs_read(int fd, void *buf, size_t len);
extern size_t fs_filesz(int fd);
//extern int _map(_Protect *p, void *va, void *pa, int mode);
//extern void *new_page(size_t nr_page);


//extern void vaddr_write(vaddr_t addr, uint32_t data, int len);
//extern uint8_t pmem[128*1024*1024];

//static void *buf = (void* ) DEFAULT_ENTRY;

static uintptr_t loader(PCB *pcb, const char *filename) {
  //TODO();
  /*void *buf=(void*)DEFAULT_ENTRY;
  int fd=fs_open(filename,0,0);
  size_t len = fs_filesz(fd);
  fs_read(fd,buf,len);
  //void* my_temp1 = buf;
  //buf+=len;
  //return (uintptr_t )my_temp1 ;
  return DEFAULT_ENTRY;*/
  int fd =fs_open(filename,0,0);
  size_t len = fs_filesz(fd);
  uint32_t my_ab=0,my_pages;
  my_ab = len&0xfff;
  my_pages = len>>12;
  void *my_phy,*my_vir;
  for(uint32_t i=0; i<my_pages; i++){
	  my_phy = new_page(1);
	  my_vir = (void *)(uintptr_t)(DEFAULT_ENTRY+(i<<12));
	  _map(&pcb->as,my_vir,my_phy,1);
	  printf("%d\n",my_phy);
	  fs_read(fd,my_phy,1<<12);
  }
  if(my_ab){
          my_phy = new_page(1);
	  my_vir = (void *)(uintptr_t)(DEFAULT_ENTRY+(my_pages<<12));
	  _map(&pcb->as,my_vir,my_phy,1);
	  fs_read(fd,my_phy,my_ab);
  }
  return DEFAULT_ENTRY;

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
  _protect(&pcb->as);
  uintptr_t entry = loader(pcb, filename);

  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
}
