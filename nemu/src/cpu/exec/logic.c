#include "cpu/exec.h"
#include "cpu/cc.h"

make_EHelper(rol) {
  t0=id_src->val;t3=1;
  while(t0--){
	rtl_msb(&t1,&id_dest->val,id_dest->width);
        rtl_shl(&id_dest->val,&id_dest->val,&t3);
	rtl_add(&id_dest->val,&id_dest->val,&t1);
  }
  t2=id_dest->val;
  rtl_update_ZFSF(&t2,id_dest->width);
  operand_write(id_dest,&t2);
}

make_EHelper(rcl) {
  t0=id_src->val;t3=1;
  while(t0--){
	  rtl_msb(&t1,&id_dest->val,id_dest->width);
	  rtl_get_CF(&t2);
	  rtl_set_CF(&t1);
	  rtl_shl(&id_dest->val,&id_dest->val,&t3);
	  rtl_add(&id_dest->val,&id_dest->val,&t2);
  }
  t2=id_dest->val;
  rtl_update_ZFSF(&t2,id_dest->width);
  operand_write(id_dest,&t2);
}

make_EHelper(test) {
  
  rtl_and(&t2,&id_dest->val,&id_src->val);
  rtl_update_ZFSF(&t2,id_dest->width);
  t0 = 0;
  rtl_set_CF(&t0);
  rtl_set_OF(&t0);
	
	//TODO();

  print_asm_template2(test);
}

make_EHelper(and) {
  rtl_and(&t2,&id_dest->val,&id_src->val);
  rtl_update_ZFSF(&t2,id_dest->width);
  operand_write(id_dest,&t2);
	//TODO();

  print_asm_template2(and);
}

make_EHelper(xor) {
  rtl_xor(&t0,&id_dest->val,&id_src->val);
  operand_write(id_dest,&t0); 

  print_asm_template2(xor);
}

make_EHelper(or) {
  rtl_or(&t2,&id_dest->val,&id_src->val);
  rtl_update_ZFSF(&t2,id_dest->width);
  operand_write(id_dest,&t2);
	//TODO();

  print_asm_template2(or);
}

make_EHelper(sar) {
  rtl_sar(&t2,&id_dest->val,&id_src->val);
  rtl_update_ZFSF(&t2,id_dest->width);
  operand_write(id_dest,&t2); 
       
	// TODO();
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(sar);
}

make_EHelper(shl) {
  rtl_shl(&t2,&id_dest->val,&id_src->val);
  rtl_update_ZFSF(&t2,id_dest->width);
  operand_write(id_dest,&t2);
	
	//TODO();
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(shl);
}

make_EHelper(shr) {
  rtl_shr(&t2,&id_dest->val,&id_src->val);
  rtl_update_ZFSF(&t2,id_dest->width);
  operand_write(id_dest,&t2);
	//TODO();
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(shr);
}

make_EHelper(setcc) {
  uint32_t cc = decoding.opcode & 0xf;

  rtl_setcc(&t2, cc);
  operand_write(id_dest, &t2);

  print_asm("set%s %s", get_cc_name(cc), id_dest->str);
}

make_EHelper(not) {
  rtl_not(&t2,&id_dest->val);
  operand_write(id_dest,&t2);
	
	
	//TODO();

  print_asm_template1(not);
}
