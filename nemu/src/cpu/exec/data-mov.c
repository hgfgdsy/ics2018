#include "cpu/exec.h"

make_EHelper(mov) {
  operand_write(id_dest, &id_src->val);
  print_asm_template2(mov);
}

make_EHelper(push) {
	rtl_push(&(id_dest->val));

  print_asm_template1(push);
}

make_EHelper(pop) {
       rtl_pop(&(id_dest->val));

  print_asm_template1(pop);
}

make_EHelper(pusha) {
  TODO();

  print_asm("pusha");
}

make_EHelper(popa) {
  TODO();

  print_asm("popa");
}

make_EHelper(leave) {
	rtl_lr(&cpu.esp,5,4);
  rtl_pop(&cpu.ebp);
	//TODO();

  print_asm("leave");
}

make_EHelper(cltd) {
  if (decoding.is_operand_size_16) {
   
	 rtl_lr(&t2,0,2);
	 rtl_update_SF(&t2,2);
	 rtl_get_SF(&t1);
	 if(t1){
		 t2=0xffff;
		 rtl_sr(2,&t2,2);}
	 else{
		 t2=0;
		 rtl_sr(2,&t2,2);}
	  // TODO();
  }
  else {
    rtl_lr(&t2,0,4);
    rtl_update_SF(&t2,4);
    rtl_get_SF(&t1);
    if(t1){
	    t2=0xffff;
	    rtl_sr(2,&t2,4);}
    else{
	    t2=0;
	    rtl_sr(2,&t2,4);}
	  //TODO();
  }

  print_asm(decoding.is_operand_size_16 ? "cwtl" : "cltd");
}

make_EHelper(cwtl) {
  if (decoding.is_operand_size_16) {
    TODO();
  }
  else {
    TODO();
  }

  print_asm(decoding.is_operand_size_16 ? "cbtw" : "cwtl");
}

make_EHelper(movsx) {
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  rtl_sext(&t0, &id_src->val, id_src->width);
  operand_write(id_dest, &t0);
  print_asm_template2(movsx);
}

make_EHelper(movzx) {
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  operand_write(id_dest, &id_src->val);
  print_asm_template2(movzx);
}

make_EHelper(lea) {
  operand_write(id_dest, &id_src->addr);
  print_asm_template2(lea);
}
