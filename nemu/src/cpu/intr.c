#include "cpu/exec.h"
#include "memory/mmu.h"

void raise_intr(uint8_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */

  //TODO();
  rtl_push(&cpu.EFLAGS);
  rtl_push(&cpu.CS);
  rtl_push(&ret_addr);
  assert(0);
  cpu.eip=vaddr_read(cpu.base+NO*8,2)+(vaddr_read(cpu.base+NO*8+4,2)<<16);
  //assert(0);
  decoding_set_jmp(true);
}

void dev_raise_intr() {
}
