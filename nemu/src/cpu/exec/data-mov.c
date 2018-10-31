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
       rtl_pop(&t1);
       operand_write(id_dest,&t1);
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
	rtl_lr(&t1,5,4);
	rtl_sr(4,&t1,4);
  rtl_pop(&t2);
  rtl_sr(5,&t2,4);
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
	    t2=0xffffffff;
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
    //TODO();
    rtl_lr(&t2,0,1);
    rtl_sext(&t0,&t2,1);
    rtl_sr(0,&t0,2);
  }
  else {
    //TODO();
    rtl_lr(&t2,0,2);
    rtl_sext(&t0,&t2,2);
    rtl_sr(0,&t0,4);
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
