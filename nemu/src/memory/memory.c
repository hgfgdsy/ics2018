#include "nemu.h"

#define PMEM_SIZE (128 * 1024 * 1024)

#define pmem_rw(addr, type) *(type *)({\
    Assert(addr < PMEM_SIZE, "physical address(0x%08x) is out of bound", addr); \
    guest_to_host(addr); \
    })

extern int is_mmio(paddr_t addr);
extern uint32_t mmio_read(paddr_t addr, int len, int map_NO);
extern void mmio_write(paddr_t addr,int len, uint32_t data, int map_NO);

uint8_t pmem[PMEM_SIZE];

/* Memory accessing interfaces */

paddr_t page_translate(vaddr_t addr){
	uintptr_t my_PDA = (uintptr_t)((cpu.cr3.page_directory_base<<12) + (((addr>>22)&0x3ff)<<2));
	uint32_t my_pd = paddr_read(my_PDA,4);
	assert((my_pd&0x1)==1);
	uintptr_t my_PTA = (uintptr_t)((my_pd&~0xfff) +(((addr>>12)&0x3ff)<<2));
	uint32_t my_pt = paddr_read(my_PTA,4);
	assert((my_pt&0x1)==1);
	return ((my_pt & ~0xfff) | (addr&0xfff));
}

uint32_t paddr_read(paddr_t addr, int len) {
  if(is_mmio(addr)==-1)
	return pmem_rw(addr, uint32_t) & (~0u >> ((4 - len) << 3));
  else
	  return mmio_read(addr,len,is_mmio(addr));
}

void paddr_write(paddr_t addr, uint32_t data, int len) {
  if(is_mmio(addr)==-1)
	memcpy(guest_to_host(addr), &data, len);
  else
	  mmio_write(addr,len,data,is_mmio(addr));
}

uint32_t vaddr_read(vaddr_t addr, int len) {
  if(cpu.cr0.paging){
  uint32_t my_dig = 0x1000-(addr&0xfff);
  if(my_dig<len){
	  /*printf("???\n"); assert(0);*/
	  paddr_t paddr=page_translate(addr);
	  uint32_t low=paddr_read(paddr,my_dig);
	  uint32_t high=paddr_read((paddr+(1<<12))&~0xfff,len-my_dig);
	  return (high<<(my_dig*8))|low;
  }
  else{
  paddr_t paddr=page_translate(addr);
  return paddr_read(paddr, len);
  }
  }
  else return paddr_read(addr,len);
}

void vaddr_write(vaddr_t addr, uint32_t data, int len) {
  if(cpu.cr0.paging){
  uint32_t my_dig = 0x1000-(addr&0xfff);
  if(my_dig<len){
	 /* printf("???\n");assert(0);*/
	  uint32_t low =((1<<(my_dig*8))-1)&data;
	  uint32_t high = data ^ low;
          paddr_t paddr=page_translate(addr);
	  paddr_write(paddr,low,my_dig);
	  paddr_write((paddr+(1<<12))&~0xfff,high,len-my_dig);
  }
  else{
  paddr_t paddr=page_translate(addr);
  paddr_write(paddr, data, len);
  }
  }
  else paddr_write(addr,data,len);
}
